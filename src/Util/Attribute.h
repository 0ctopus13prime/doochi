#ifndef LUCENE_CORE_UTIL_ATTRIBUTE_H_
#define LUCENE_CORE_UTIL_ATTRIBUTE_H_

#include <unordered_map>
#include <vector>
#include <unordered_set>
#include <functional>
#include <memory>
#include <string>

namespace lucene { namespace core { namespace util {

using type_id = size_t;

class Attribute {
  public:
    virtual ~Attribute() {}
};

using AttributeReflector = std::function<void(const std::string&/*Attribute class name*/,
                                              const std::string&/*Key*/,
                                              const std::string&/*Value*/)>;

class AttributeImpl: public Attribute {
  public:
    virtual ~AttributeImpl() { }
    virtual void ReflectWith(AttributeReflector& reflector) = 0;
    virtual void Clear() = 0;
    virtual void End();
    virtual std::vector<type_id> Attributes() = 0;
    virtual void ShallowCopyTo(AttributeImpl& attr_impl) = 0;
    virtual AttributeImpl* Clone() = 0;
    std::string ReflectAsString(const bool prepend_att_class);
};

using AttributeImplGenerator = std::function<AttributeImpl*(void)>;

class AttributeFactory {
  private:
    static std::unordered_map<type_id, AttributeImplGenerator> ATTR_IMPL_TABLE;

  public:
    AttributeFactory();
    virtual ~AttributeFactory() {}

    virtual AttributeImpl* CreateAttributeInstance(type_id attr_type_id) = 0;

    static AttributeImplGenerator FindAttributeImplGenerator(type_id attr_type_id);

    template<typename ATTR_FACTORY, typename ATTR_IMPL>
    static AttributeFactory* GetStaticImplementation() {
      return new StaticImplementationAttributeFactory<ATTR_FACTORY, ATTR_IMPL>();
    }

    template<typename ATTR, typename ATTR_IMPL_CLASS>
    static void RegisterAttributeImpl() {
      ATTR_IMPL_TABLE[typeid(ATTR).hash_code()] = [](){
        return new ATTR_IMPL_CLASS();
      };
    }

  public:
    class DefaultAttributeFactory;

    template<typename ATTR_FACTORY, typename ATTR_IMPL>
    class StaticImplementationAttributeFactory;
};

class AttributeFactory::DefaultAttributeFactory: public AttributeFactory {
  public:
    DefaultAttributeFactory();
    virtual ~DefaultAttributeFactory();
    AttributeImpl* CreateAttributeInstance(type_id attr_type_id) override;
};

template<typename ATTR_FACTORY, typename ATTR_IMPL>
class AttributeFactory::StaticImplementationAttributeFactory: public AttributeFactory {
  private:
    ATTR_FACTORY delegate;
    static std::unordered_set<type_id> DEFAULT_ATTR_TYPE_IDS;

  public:
    StaticImplementationAttributeFactory() {
    }
    virtual ~StaticImplementationAttributeFactory() {
    }
    AttributeImpl* CreateAttributeInstance(type_id attr_type_id) override {
      if(DEFAULT_ATTR_TYPE_IDS.find(attr_type_id) != DEFAULT_ATTR_TYPE_IDS.end()) {
        return delegate.CreateAttributeInstance(attr_type_id);
      } else {
        return new ATTR_IMPL();
      }
    }
};

template<typename ATTR_FACTORY, typename ATTR_IMPL>
std::unordered_set<type_id>
AttributeFactory::StaticImplementationAttributeFactory<ATTR_FACTORY, ATTR_IMPL>::DEFAULT_ATTR_TYPE_IDS
= [](){
  std::unordered_set<type_id> ret(AttributeFactory::ATTR_IMPL_TABLE.size());
  for(const auto& p : AttributeFactory::ATTR_IMPL_TABLE) {
    ret.insert(p.first);
  }
  return ret;
}();

class AttributeSource {
  public:
    class State final {
      public:
        AttributeImpl* attribute;
        State* next;
        bool delete_attribute;

      public:
        State(bool delete_attribute = false);
        State(const State& other);
        State(State&& other);
        ~State();
        State& operator=(const State& other);
        State& operator=(State&& other);
        void CleanAttribute();
    };

  private:
    std::unique_ptr<State> current_state;
    // Mapping Attribute's type_id to AttributeImpl instance
    std::unordered_map<type_id, std::shared_ptr<AttributeImpl>> attributes;
    // Mapping AttributeImpl's type_id to AttributeImpl instance
    std::unordered_map<type_id, std::shared_ptr<AttributeImpl>> attribute_impls;
    std::shared_ptr<AttributeFactory> factory;

  private:
    State* GetCurrentState();

  public:
    AttributeSource();
    AttributeSource(const AttributeSource& other);
    /**
     * AttributeSource consturctor.
     * AttributeSource owns a given factory
     */
    AttributeSource(AttributeFactory* factory);
    AttributeFactory& GetAttributeFactory() const;
    void AddAttributeImpl(AttributeImpl* attr_impl);

    template <typename ATTR>
    std::shared_ptr<ATTR> AddAttribute() {
      auto attr_it = attributes.find(typeid(ATTR).hash_code());
      if(attr_it == attribute_impls.end()) {
        AddAttributeImpl(factory->CreateAttributeInstance(typeid(ATTR).hash_code()));
        attr_it = attributes.find(typeid(ATTR).hash_code());
      }

      return std::dynamic_pointer_cast<ATTR>(attr_it->second);
    }

    bool HasAttributes();

    template<typename ATTR>
    bool HasAttribute() {
      return (attributes.find(typeid(ATTR).hash_code()) != attributes.end());
    }

    void ClearAttributes();
    void EndAttributes();
    void RemoveAllAttributes();
    State* CaptureState();
    void RestoreState(State* state);
    std::string ReflectAsString(const bool prepend_att);
    void ReflectWith(AttributeReflector& reflector);
    AttributeSource& operator=(const AttributeSource& other);
};

}}} // End of namespace

#endif
