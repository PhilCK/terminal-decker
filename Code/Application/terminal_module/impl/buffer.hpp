#ifndef BUFFER_INCLUDED_75922F2F_8BC3_4D2B_8F47_5E369DA209B8
#define BUFFER_INCLUDED_75922F2F_8BC3_4D2B_8F47_5E369DA209B8


#include <stdint.h>
#include <string>
#include <array>
#include <vector>
#include <mutex>
#include <atomic>


namespace buffer_utils
{

std::vector<uint32_t>     string_to_buffer(const std::string &str);
std::string               debug_buffer_to_string(const std::vector<uint32_t> &content);

} // namespace


class buffer
{
public:
  
	void                  push(const std::vector<uint32_t> &str);
	void                  pop();
	void                  clear();

	std::vector<uint32_t>	get_data() const;

private:

	using buffer_array = 	std::array<uint32_t, 512>;

	buffer_array          m_buffer{};
	std::size_t           m_buffer_boundry = 0;
	int32_t               m_buffer_trim = static_cast<int32_t>(m_buffer.size());
	mutable std::mutex    m_lock;
  
}; // class


#endif // include guard