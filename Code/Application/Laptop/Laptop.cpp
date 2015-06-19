#include <Application/Laptop/Laptop.hpp>
#include <assert.h>


Laptop::Laptop()
{
}


void
Laptop::set_input_prompt(const std::vector<uint32_t> &input)
{
  m_prompt = input;
}


void
Laptop::add_content(std::vector<uint32_t> &content)
{
  m_tasks.at(m_active_task).push(content);
}


void
Laptop::remove_content()
{

}


void
Laptop::switch_active_buffer(const uint32_t task)
{
  assert(task < m_tasks.size());
}


void
Laptop::enable_input(const bool input)
{
  m_user_input = input;
}