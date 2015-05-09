#ifndef TERMINAL_INCLUDED_5C68C166_D5E8_4359_8678_4F59ED0CBCFD
#define TERMINAL_INCLUDED_5C68C166_D5E8_4359_8678_4F59ED0CBCFD


#include <vector>
#include <string>
#include <stdint.h>
#include <memory>
#include <iostream>


using namespace std;

namespace detail {


template<typename T>
void on_connection(T &self) { }

template<typename T>
void on_think(T &self, const float dt) { }

template<typename T>
void on_input_str(T &self, const std::string &input) {}



class terminal_program_interface final
{
public:

	template<typename T>
	terminal_program_interface(T t) : object_(new program_model<T>(t)) { std::cout << "ctor" << std::endl; }

	terminal_program_interface(const terminal_program_interface& a) : object_(a.object_->copy_()) {std::cout << "copy" << std::endl; }

	terminal_program_interface(terminal_program_interface&&) = default;

	terminal_program_interface& operator=(const terminal_program_interface& a)
	{
		std::cout << "ass" << std::endl; 
		terminal_program_interface tmp(a);
		*this = std::move(tmp);
		return *this;
	}

#ifdef __APPLE__
    terminal_program_interface& operator=(terminal_program_interface&&) noexcept = default;
#else
	terminal_program_interface& operator=(terminal_program_interface&&) {} /*noexcept = default;*/
#endif


	// Module Interface
	friend void on_connection(terminal_program_interface &t)                        { t.object_->on_connection_();    }
  friend void on_think(terminal_program_interface &t, const float dt)             { t.object_->on_think_(dt);       }
  //friend void on_input_str(terminal_program_interface &t, const std::string &str) { t.object_->on_input_str_(str);  }


private:

	struct program_concept
	{
		virtual ~program_concept() = default;
		virtual program_concept* copy_() const = 0;
		
    virtual void on_connection_() = 0;
    virtual void on_think_(const float dt) = 0;
    //virtual void on_input_str_(const std::string &str) = 0;

	};

	template<typename T>
	struct program_model : program_concept
	{
		program_model(T t) : data_(std::move(t)) {}
		program_concept* copy_() const { return new program_model(*this); }

		void on_connection_()                       { on_connection(data_);     }
    void on_think_(const float dt)              { on_think(data_, dt);      }
    void on_input_str_(const std::string &str)  { on_input_str(data_, str); }

		T data_;
		
	};

	std::unique_ptr<program_concept> object_;
};

} // namespace



class terminal
{
public:

  
  explicit              terminal(const uint32_t max_number_of_systems);

  void                  think_systems(const float dt);
  void                  input_string(const std::string &str);
  
  
private:

  std::vector<std::vector<detail::terminal_program_interface>>      m_systems;

}; // class

#endif // include guard