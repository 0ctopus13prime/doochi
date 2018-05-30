#ifndef LUCENE_CORE_ANALYSIS_TOKEN_ATTRIBUTES_ATTRIBUTES_IMPL_H
#define LUCENE_CORE_ANALYSIS_TOKEN_ATTRIBUTES_ATTRIBUTES_IMPL_H

#include <vector>
#include <string>
#include <Analysis/Attribute.h>
#include <Util/Bytes.h>
#include <Util/Attribute.h>

using namespace lucene::core::util;

namespace lucene { namespace core { namespace analysis { namespace tokenattributes {

class BytesTermAttributeImpl: public AttributeImpl, public BytesTermAttribute {
  private:
    BytesRef bytes;

  public:
    BytesTermAttributeImpl();
    BytesTermAttributeImpl(const BytesTermAttributeImpl& other);
    BytesRef& GetBytesRef() override;
    virtual ~BytesTermAttributeImpl();
    void SetBytesRef(BytesRef& bytes) override;
    void Clear() override;
    void ReflectWith(AttributeReflector& reflector) override;
    bool operator==(BytesTermAttributeImpl& other);
    std::vector<size_t> Attributes() override;
    void ShallowCopyTo(AttributeImpl& attr_impl) override;
    BytesTermAttributeImpl& operator=(const BytesTermAttributeImpl& other);
};

#define CHAR_TERM_ATTRIBUTE_IMPL_MIN_BUFFER_SIZE 10
class CharTermAttributeImpl: public AttributeImpl, public CharTermAttribute, public TermToBytesRefAttribute {
  private:
    char* term_buffer;
    uint32_t term_capacity;
    uint32_t term_length;
    BytesRefBuilder builder;

  private:
    void GrowTermBuffer(const uint32_t new_size);

  public:
    CharTermAttributeImpl();
    CharTermAttributeImpl(const CharTermAttributeImpl& other);
    virtual ~CharTermAttributeImpl();
    BytesRef& GetBytesRef() override;
    void Clear() override;
    void CopyBuffer(const char* buffer, const uint32_t offset, const uint32_t length) override;
    char* Buffer() const override;
    char* ResizeBuffer(const uint32_t new_size) override;
    uint32_t Length() const override;
    std::string SubSequence(const uint32_t start, const uint32_t end) override;
    CharTermAttributeImpl& SetLength(const uint32_t length) override;
    CharTermAttributeImpl& SetEmpty() override;
    CharTermAttribute& Append(const std::string& csq) override;
    CharTermAttribute& Append(const std::string& csq, const uint32_t start, const uint32_t end) override;
    CharTermAttribute& Append(const char c) override;
    CharTermAttribute& Append(const CharTermAttribute& term_att) override;
    void ReflectWith(AttributeReflector& reflector) override;
    char& operator[](const uint32_t idx) override;
    bool operator==(CharTermAttributeImpl& other);
    std::vector<size_t> Attributes() override;
    void ShallowCopyTo(AttributeImpl& attr_impl) override;
    CharTermAttributeImpl& operator=(const CharTermAttributeImpl& other);
};

class FlagsAttributeImpl: public AttributeImpl, public FlagsAttribute {
  private:
    int32_t flags;

  public:
    FlagsAttributeImpl();
    FlagsAttributeImpl(const FlagsAttributeImpl& other);
    virtual ~FlagsAttributeImpl();
    int32_t GetFlags() override;
    void SetFlags(const int32_t flags) override;
    void ReflectWith(AttributeReflector& reflector) override;
    void Clear() override;
    bool operator==(const FlagsAttributeImpl& other);
    std::vector<size_t> Attributes() override;
    void ShallowCopyTo(AttributeImpl& attr_impl) override;
    FlagsAttributeImpl& operator=(const FlagsAttributeImpl& other);
};

class KeywordAttributeImpl: public AttributeImpl, public KeywordAttribute {
  private:
    bool keyword;

  public:
    KeywordAttributeImpl();
    KeywordAttributeImpl(const KeywordAttributeImpl& other);
    virtual ~KeywordAttributeImpl();
    bool IsKeyword() override;
    void SetKeyword(const bool is_keyword) override;
    void ReflectWith(AttributeReflector& reflector) override;
    void Clear() override;
    bool operator==(const KeywordAttributeImpl& other);
    std::vector<size_t> Attributes() override;
    void ShallowCopyTo(AttributeImpl& attr_impl) override;
    KeywordAttributeImpl& operator=(const KeywordAttributeImpl& other);
};

class OffsetAttributeImpl: public AttributeImpl, public OffsetAttribute {
  private:
    uint32_t start_offset;
    uint32_t end_offset;

  public:
    OffsetAttributeImpl();
    OffsetAttributeImpl(const OffsetAttributeImpl& other);
    virtual ~OffsetAttributeImpl();
    uint32_t StartOffset() override;
    void SetOffset(const uint32_t start_offset, const uint32_t end_offset) override;
    uint32_t EndOffset() override;
    void ReflectWith(AttributeReflector& reflector) override;
    void Clear() override;
    bool operator==(const OffsetAttributeImpl& other);
    std::vector<size_t> Attributes() override;
    void ShallowCopyTo(AttributeImpl& attr_impl) override;
    OffsetAttributeImpl& operator=(const OffsetAttributeImpl& other);
};

class PackedTokenAttributeImpl:
  public CharTermAttributeImpl, public TypeAttribute, public PositionIncrementAttribute,
  public PositionLengthAttribute, public OffsetAttribute, public TermFrequencyAttribute {

  private:
    uint32_t start_offset;
    uint32_t end_offset;
    std::string type;
    uint32_t position_increment;
    uint32_t position_length;
    uint32_t term_frequency;

  public:
    PackedTokenAttributeImpl();
    PackedTokenAttributeImpl(const PackedTokenAttributeImpl& other);
    virtual ~PackedTokenAttributeImpl();
    std::string& Type() override;
    void SetType(const std::string& new_type) override;
    void SetPositionIncrement(const uint32_t new_position_increment) override;
    uint32_t GetPositionIncrement() override;
    void SetPositionLength(const uint32_t new_position_length) override;
    uint32_t GetPositionLength() override;
    uint32_t StartOffset() override;
    void SetOffset(const uint32_t new_start_offset, const uint32_t new_end_offset) override;
    uint32_t EndOffset() override;
    void SetTermFrequency(const uint32_t new_term_frequency) override;
    uint32_t GetTermFrequency() override;
    std::vector<size_t> Attributes() override;
    void ShallowCopyTo(AttributeImpl& attr_impl) override;
    PackedTokenAttributeImpl& operator=(const PackedTokenAttributeImpl& other);
};

class PayloadAttributeImpl: public AttributeImpl, public PayloadAttribute {
  private:
    BytesRef payload;

  public:
    PayloadAttributeImpl();
    PayloadAttributeImpl(const PayloadAttributeImpl& other);
    virtual ~PayloadAttributeImpl();
    void ReflectWith(AttributeReflector& reflector) override;
    void Clear() override;
    bool operator==(PayloadAttributeImpl& other);
    BytesRef& GetPayload() override;
    void SetPayload(BytesRef& payload) override;
    std::vector<size_t> Attributes() override;
    void ShallowCopyTo(AttributeImpl& attr_impl) override;
    PayloadAttributeImpl& operator=(const PayloadAttributeImpl& other);
};

class PositionIncrementAttributeImpl: public AttributeImpl, public PositionIncrementAttribute {
  private:
    uint32_t position_increment;

  public:
    PositionIncrementAttributeImpl();
    PositionIncrementAttributeImpl(const PositionIncrementAttributeImpl& other);
    virtual ~PositionIncrementAttributeImpl();
    void SetPositionIncrement(const uint32_t position_increment) override;
    uint32_t GetPositionIncrement() override;
    void ReflectWith(AttributeReflector& reflector) override;
    void End() override;
    void Clear() override;
    bool operator==(PositionIncrementAttributeImpl& other);
    std::vector<size_t> Attributes() override;
    void ShallowCopyTo(AttributeImpl& attr_impl) override;
    PositionIncrementAttributeImpl& operator=(const PositionIncrementAttributeImpl& other);
};

class PositionLengthAttributeImpl: public AttributeImpl, public PositionLengthAttribute {
  private:
    uint32_t position_length;

  public:
    PositionLengthAttributeImpl();
    PositionLengthAttributeImpl(const PositionLengthAttributeImpl& other);
    virtual ~PositionLengthAttributeImpl();
    void ReflectWith(AttributeReflector& reflector) override;
    void Clear() override;
    bool operator==(PositionLengthAttributeImpl& other);
    void SetPositionLength(const uint32_t position_length) override;
    uint32_t GetPositionLength() override;
    std::vector<size_t> Attributes() override;
    void ShallowCopyTo(AttributeImpl& attr_impl) override;
    PositionLengthAttributeImpl& operator=(const PositionLengthAttributeImpl& other);
};

class TermFrequencyAttributeImpl: public AttributeImpl, public TermFrequencyAttribute {
  private:
    uint32_t term_frequency;

  public:
    TermFrequencyAttributeImpl();
    TermFrequencyAttributeImpl(const TermFrequencyAttributeImpl& other);
    virtual ~TermFrequencyAttributeImpl();
    void ReflectWith(AttributeReflector& reflector) override;
    void Clear() override;
    bool operator==(TermFrequencyAttributeImpl& other);
    void SetTermFrequency(const uint32_t term_frequency) override;
    uint32_t GetTermFrequency() override;
    std::vector<size_t> Attributes() override;
    void ShallowCopyTo(AttributeImpl& attr_impl) override;
    TermFrequencyAttributeImpl& operator=(const TermFrequencyAttributeImpl& other);
};

class TypeAttributeImpl: public AttributeImpl, public TypeAttribute {
  private:
    std::string type;

  public:
    TypeAttributeImpl();
    TypeAttributeImpl(const TypeAttributeImpl& other);
    virtual ~TypeAttributeImpl();
    void ReflectWith(AttributeReflector& reflector) override;
    void Clear() override;
    bool operator==(TypeAttributeImpl& other);
    std::string& Type() override;
    void SetType(const std::string& type) override;
    std::vector<size_t> Attributes() override;
    void ShallowCopyTo(AttributeImpl& attr_impl) override;
    TypeAttributeImpl& operator=(const TypeAttributeImpl& other);
};

}}}} // End of namespace

#endif
