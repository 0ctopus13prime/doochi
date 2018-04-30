#ifndef LUCENE_CORE_UTIL_ATTRIBUTE_FACTORY_H_
#define LUCENE_CORE_UTIL_ATTRIBUTE_FACTORY_H_

namespace lucene {
  namespace core {
    namespace util {

      class AttributeReflector {
        public:
          virtual ~AttributeReflector() {}
          virtual void reflect() = 0;
      };

    } // End of namespace util
  } // End of namespace core
} // End of namespace lucene

#endif
