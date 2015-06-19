#ifndef LAPTOP_INCLUDED_41FC02D0_3490_45C4_970A_A6CDEC7AE6B9
#define LAPTOP_INCLUDED_41FC02D0_3490_45C4_970A_A6CDEC7AE6B9


#include <Application/Laptop/Buffer.hpp>
#include <stdint.h>
#include <string>


class Laptop
{
public:

  explicit            Laptop();
  
  //void                insert_input(const std::string &input);
  void                set_input_prompt(const std::vector<uint32_t> &input);
  void                add_content(std::vector<uint32_t> &content);
  void                remove_content();
  void                switch_active_buffer(const uint32_t task);
  inline uint32_t     number_of_tasks() const { return static_cast<uint32_t>(m_tasks.size()); }
  void                enable_input(const bool input);
  inline buffer&      get_active_buffer() { return m_tasks.at(m_active_task); }

private:

  std::array<buffer, 4> m_tasks;
  std::vector<uint32_t> m_prompt;
  uint32_t              m_active_task = 0;
  bool                  m_user_input = true;
  
};


#endif // include guard