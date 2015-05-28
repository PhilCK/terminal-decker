#ifndef TERMINAL_MODULE_INCLUDE_5DFDDB47_FC50_45AD_A2CE_7CCA07047B9E
#define TERMINAL_MODULE_INCLUDE_5DFDDB47_FC50_45AD_A2CE_7CCA07047B9E


#include <Application/Terminal/TerminalScreen.hpp>
#include <string>
#include <stdint.h>
#include <memory>
#include <iostream>


using namespace std;

namespace detail {


template<typename T>
void on_connection(terminal_controller& controller, T &self) {}

template<typename T>
void on_disconnection(terminal_controller& controller, T &self) {}

template<typename T>
void on_think(terminal_controller& controller, T &self, const float dt) {}

template<typename T>
void on_input_str(terminal_controller& controller, T &self, const std::string &input) {}


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
	friend void on_connection(terminal_controller& screen, terminal_program_interface &t)                        { t.object_->on_connection_(screen);    }
  friend void on_disconnection(terminal_controller& screen, terminal_program_interface &t)                        { t.object_->on_connection_(screen);    }
  friend void on_think(terminal_controller& screen, terminal_program_interface &t, const float dt)             { t.object_->on_think_(screen, dt);       }
  friend void on_input_str(terminal_controller& screen, terminal_program_interface &t, const std::string &str) { t.object_->on_input_str_(screen, str);  }


private:

	struct program_concept
	{
		virtual ~program_concept() = default;
		virtual program_concept* copy_() const = 0;
		
    virtual void on_connection_(terminal_controller& screen) = 0;
    virtual void on_disconnection_(terminal_controller& screen) = 0;
    virtual void on_think_(terminal_controller& screen, const float dt) = 0;
    virtual void on_input_str_(terminal_controller& screen, const std::string &str) = 0;
	};


	template<typename T>
	struct program_model : program_concept
	{
		program_model(T t) : data_(std::move(t)) {}
		program_concept* copy_() const { return new program_model(*this); }

		void on_connection_(terminal_controller& screen)                         { on_connection(screen, data_);     }
    void on_disconnection_(terminal_controller& screen)                      { on_disconnection(screen, data_);  }
    void on_think_(terminal_controller& screen, const float dt)              { on_think(screen, data_, dt);      }
    void on_input_str_(terminal_controller& screen, const std::string &str)  { on_input_str(screen, data_, str); }

		T data_;
		
	};

	std::unique_ptr<program_concept> object_;
};

} // namespace


#endif // include guard