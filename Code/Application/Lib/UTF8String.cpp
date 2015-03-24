#include <Application/Lib/UTF8String.hpp>

TerminalString::TerminalString()
: m_string()
{

}


TerminalString::~TerminalString()
{

}


TerminalChar& TerminalString::at(const std::size_t i)
{
  return m_string.at(i);
}


const TerminalChar& TerminalString::at(const std::size_t i) const
{
  return m_string.at(i);
}
