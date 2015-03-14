#ifndef UTF8_STRING_INCLUDED_197A1D01_D0C6_4308_BD35_69C620D68FDE
#define UTF8_STRING_INCLUDED_197A1D01_D0C6_4308_BD35_69C620D68FDE


#include <Caffeine/Common/Platform.hpp>
#include <vector>


class UTF8Char
{
public:

  inline uint32_t     value() const { return m_value; }

private:

  const uint32_t      m_value;

};


template<typename CharType>
class UTF8String
{
public:
  
  explicit                  UTF8String();
  explicit                  UTF8String(const std::string &str);
  explicit                  UTF8String(const char* str);
  explicit                  UTF8String(const CharType* str);

  UTF8String<CharType>      substr(const std::size_t start, const std::size_t size) const;
  void                      append(CharType &charType);
  void                      append(UTF8String<CharType> &str);

  CharType&                 at(const std::size_t i)         { return m_stringData.at(i);  }
  const CharType&           at(const std::size_t i) const   { return m_stringData.at(i); }
  std::size_t               size() const                    { return m_stringData.size(); }
  inline std::size_t        length() const                  { return size();              }

private:

  std::vector<CharType>     m_stringData;

};

#endif