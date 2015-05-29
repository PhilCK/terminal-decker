
#include "TerminalBuffer.hpp"
#include <iostream>

#include <algorithm>


namespace TerminalBufferUtils
{


std::vector<uint32_t>
string_to_buffer(const std::string &str)
{
	return std::vector<uint32_t>(std::begin(str), std::end(str));
}


std::string
debug_buffer_to_string(const std::vector<uint32_t> &content)
{
	return std::string(std::begin(content), std::end(content));
}


} // namespace


void
TerminalBuffer::push(const std::vector<uint32_t> &content)
{
	std::lock_guard<std::mutex> lock(m_lock);

	for(const auto &c : content)
	{
		const std::size_t i = m_buffer_boundry % m_buffer.size();

		m_buffer.at(i) = c;

		++m_buffer_boundry;
		m_buffer_trim = std::max(0, m_buffer_trim - 1);
	}
}


void
TerminalBuffer::pop()
{
	std::lock_guard<std::mutex> lock(m_lock);

	m_buffer_boundry    = std::max(std::size_t(0), m_buffer_boundry - 1);
	m_buffer_trim       = std::max(0, m_buffer_trim - 1);
  
	const std::size_t i = m_buffer_boundry % m_buffer.size();
	m_buffer.at(i)      = ' ';
}


void
TerminalBuffer::clear()
{
	m_buffer_trim = static_cast<int32_t>(m_buffer.size());
}


std::vector<uint32_t>
TerminalBuffer::get_data() const
{
	std::lock_guard<std::mutex> lock(m_lock);

	buffer_array content(m_buffer);
	const std::size_t rotation_pos  = m_buffer_boundry % m_buffer.size();
	const std::size_t trim          = std::max(0, m_buffer_trim);

	m_lock.unlock();
	
	std::rotate(std::begin(content), std::begin(content) + rotation_pos, std::end(content));

	return std::vector<uint32_t>(std::begin(content) + trim, std::end(content));
}