#ifndef UTF8_STRING_INCLUDED_197A1D01_D0C6_4308_BD35_69C620D68FDE
#define UTF8_STRING_INCLUDED_197A1D01_D0C6_4308_BD35_69C620D68FDE


#include <Caffeine/Common/Platform.hpp>
#include <array>
#include <iterator>


typedef uint32_t TerminalChar;


class TerminalString
{
public:

  explicit                          TerminalString();
                                    ~TerminalString();
  
  std::array<TerminalChar, 80>::iterator begin()              { return m_string.begin(); }
  std::array<TerminalChar, 80>::const_iterator begin() const  { return m_string.begin(); }
  std::array<TerminalChar, 80>::iterator end()                { return m_string.end(); }
  std::array<TerminalChar, 80>::const_iterator end() const    { return m_string.end(); }

  TerminalChar&                     at(const std::size_t i);
  const TerminalChar&               at(const std::size_t i) const;

  inline std::size_t                length() { return m_string.size(); }

private:

  std::array<TerminalChar, 80>   m_string;

};

#endif