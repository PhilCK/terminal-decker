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
void on_connection(TerminalController& controller, T &self) {}

template<typename T>
void on_disconnection(TerminalController& controller, T &self) {}

template<typename T>
void on_think(TerminalController& controller, T &self, const float dt) {}

template<typename T>
void on_input_str(TerminalController& controller, T &self, const std::string &input) {}


class TerminalProgramInterface final
{
public:

	template<typename T>
	TerminalProgramInterface(T t) : object_(new ProgramModel<T>(t)) { std::cout << "ctor" << std::endl; }

	TerminalProgramInterface(const TerminalProgramInterface& a) : object_(a.object_->copy_()) {std::cout << "copy" << std::endl; }

	//TerminalProgramInterface(TerminalProgramInterface&&) = default;

	TerminalProgramInterface& operator=(const TerminalProgramInterface& a)
	{
		std::cout << "ass" << std::endl; 
		TerminalProgramInterface tmp(a);
		*this = std::move(tmp);
		return *this;
	}

#ifdef __APPLE__
    TerminalProgramInterface& operator=(TerminalProgramInterface&&) noexcept = default;
#else
	TerminalProgramInterface& operator=(TerminalProgramInterface&&) {} /*noexcept = default;*/
#endif


	// Module Interface
	friend void on_connection(TerminalController& screen, TerminalProgramInterface &t)                        { t.object_->on_connection_(screen);    }
  friend void on_disconnection(TerminalController& screen, TerminalProgramInterface &t)                        { t.object_->on_connection_(screen);    }
  friend void on_think(TerminalController& screen, TerminalProgramInterface &t, const float dt)             { t.object_->on_think_(screen, dt);       }
  friend void on_input_str(TerminalController& screen, TerminalProgramInterface &t, const std::string &str) { t.object_->on_input_str_(screen, str);  }


private:

	struct program_concept
	{
		virtual ~program_concept() = default;
		virtual program_concept* copy_() const = 0;
		
    virtual void on_connection_(TerminalController& screen) = 0;
    virtual void on_disconnection_(TerminalController& screen) = 0;
    virtual void on_think_(TerminalController& screen, const float dt) = 0;
    virtual void on_input_str_(TerminalController& screen, const std::string &str) = 0;
	};


	template<typename T>
	struct ProgramModel : program_concept
	{
		ProgramModel(T t) : data_(std::move(t)) {}
		program_concept* copy_() const { return new ProgramModel(*this); }

		void on_connection_(TerminalController& screen)                         { on_connection(screen, data_);     }
    void on_disconnection_(TerminalController& screen)                      { on_disconnection(screen, data_);  }
    void on_think_(TerminalController& screen, const float dt)              { on_think(screen, data_, dt);      }
    void on_input_str_(TerminalController& screen, const std::string &str)  { on_input_str(screen, data_, str); }

		T data_;
		
	};

	std::unique_ptr<program_concept> object_;
};

} // namespace


#endif // include guard