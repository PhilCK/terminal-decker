#ifndef TERMINAL_INCLUDED_72DFCE65_AA4C_4CB2_A0CD_262B9A4D2F7C
#define TERMINAL_INCLUDED_72DFCE65_AA4C_4CB2_A0CD_262B9A4D2F7C


#include <vector>
#include <stdint.h>


namespace Terminal
{

uint32_t              create_terminal(const uint32_t width, const uint32_t height);
void                  push_content_to_terminal(const uint32_tTerminalID, std::vector<uint32_t> &content);
void                  push_content_to_overlay(const uint32_tTerminalID, std::vector<uint32_t> &content);
void                  remove_content(const uint32_t id);
void                  clear_content(const uint32_t id);
std::vector<float>    get_terminal_data(const uint32_t id);


} // namespace


#endif // include guard