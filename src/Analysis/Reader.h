#ifndef LUCENE_CORE_ANALYSIS_READER_H_
#define LUCENE_CORE_ANALYSIS_READER_H_

#include <string>
#include <sstream>

namespace lucene { namespace core { namespace analysis {

class Reader {
  public:
    virtual ~Reader();
    virtual void SetValue(std::string& value) = 0;
    virtual void SetValue(char* buf, const unsigned int len) = 0;
    virtual void SetValue(char* buf, const unsigned int off, const unsigned int len) = 0;
    virtual char Read() = 0;
    virtual int Read(char* buf, const unsigned int off, const unsigned int len) = 0;
    virtual unsigned long Skip(const unsigned long n) = 0;
    virtual bool MarkSupported() = 0;
    virtual void Mark(unsigned int read_ahead_limit) = 0;
    virtual void Reset() = 0;
};

class StringReader: public Reader {
  private:
    std::istringstream iss;
    unsigned int mark;

  public:
    StringReader();
    StringReader(StringReader& other);
    StringReader(const std::string& str);
    StringReader(const char* buf, const unsigned len);
    StringReader(const char* buf, const unsigned int off, const unsigned len);
    StringReader(std::istringstream&& iss);
    StringReader& operator=(StringReader& other);
    StringReader& operator=(std::istringstream& new_iss);
    virtual ~StringReader();
    void SetValue(std::string& value) override;
    void SetValue(char* buf, const unsigned int len) override;
    void SetValue(char* buf, const unsigned int off, const unsigned int len) override;
    char Read() override;
    int Read(char* buf, const unsigned int off, const unsigned int len) override;
    unsigned long Skip(const unsigned long n) override;
    bool MarkSupported() override;
    void Mark(unsigned int read_ahead_limit) override;
    void Reset() override;
};

}}}

#endif