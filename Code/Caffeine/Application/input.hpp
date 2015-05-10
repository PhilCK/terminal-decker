#ifndef INPUT_INCLUDED_F011F6B9_25E1_4AE5_AE5E_73E6829553F8
#define INPUT_INCLUDED_F011F6B9_25E1_4AE5_AE5E_73E6829553F8


#include <Caffeine/Application/ApplicationFwd.hpp>
#include <Caffeine/Common/Platform.hpp>
#include <functional>
#include <map>


namespace caffeine {
namespace application {


enum class key_state {
	DOWN = 0,
	UP,
};


enum class key_id
{
	KB_A = SDLK_a,
	KB_B = SDLK_b,
	KB_C = SDLK_c,
	KB_D = SDLK_d,
	KB_E = SDLK_e,
	KB_F = SDLK_f,
	KB_G = SDLK_g,
	KB_H = SDLK_h,
	KB_I = SDLK_i,
	KB_J = SDLK_j,
	KB_K = SDLK_k,
	KB_L = SDLK_l,
	KB_M = SDLK_m,
	KB_N = SDLK_n,
	KB_O = SDLK_o,
	KB_P = SDLK_p,
	KB_Q = SDLK_q,
	KB_R = SDLK_r,
	KB_S = SDLK_s,
	KB_T = SDLK_t,
	KB_U = SDLK_u,
	KB_V = SDLK_v,
	KB_W = SDLK_w,
	KB_X = SDLK_x,
	KB_Y = SDLK_y,
	KB_Z = SDLK_z,
	
	KB_1 = SDLK_1,
	KB_2 = SDLK_2,
	KB_3 = SDLK_3,
	KB_4 = SDLK_4,
	KB_5 = SDLK_5,
	KB_6 = SDLK_6,
	KB_7 = SDLK_7,
	KB_8 = SDLK_8,
	KB_9 = SDLK_9,
	KB_0 = SDLK_0,
	
	KB_UP     = SDLK_UP,
	KB_DOWN   = SDLK_DOWN,
	KB_LEFT   = SDLK_LEFT,
	KB_RIGHT  = SDLK_RIGHT,
	
	KB_ESCAPE     = SDLK_ESCAPE,
	KB_RSHIFT     = SDLK_RSHIFT,
	KB_LSHIFT     = SDLK_LSHIFT,
	KB_SPACE      = SDLK_SPACE,
	KB_RCTRL      = SDLK_RCTRL,
	KB_LCTRL      = SDLK_LCTRL,
	KB_RALT       = SDLK_RALT,
	KB_LALT       = SDLK_LALT,
	KB_TAB        = SDLK_TAB,
	KB_CAPS       = SDLK_CAPSLOCK,
	KB_NUM        = SDLK_NUMLOCKCLEAR,
	KB_ENTER      = SDLK_KP_ENTER,
	KB_RETURN     = SDLK_RETURN,
	KB_BACKSPACE  = SDLK_BACKSPACE,
	KB_DELETE     = SDLK_DELETE,
	
  MS_LEFT,
  MS_CENTER,
  MS_RIGHT,
	
	GP_LEFT_XAXIS,
	GP_LEFT_YAXIS,

	GP_RIGHT_XAXIS,
	GP_RIGHT_YAXIS,
};


class input
{

	input(const input &) = delete;
	input& operator=(const input&) = delete;
  

public:

  explicit            input();
                      ~input();
	
	
	// ** Mouse Control ** //

	uint32_t            get_mouse_x() const;
	uint32_t            get_mouse_y() const;
	inline float				get_mouse_x_delta() const { return m_x_delta; }
	inline float				get_mouse_y_delta() const { return m_y_delta; }
	void                set_mouse_hold(const bool is_held);


  // ** Keyboard Control ** //

  bool                is_down(const key_id id) const;
  bool                is_down_on_frame(const key_id id) const;
  bool                is_up_on_frame(const key_id id) const;
  std::string         get_key_name(const key_id id) const;
  void                set_text_stream(const bool set);


  // ** Events ** //

	std::function<void (const float, const float)>      on_mouse_move_position_event;
  std::function<void (const float, const float)>      on_mouse_delta_event;
	std::function<void (const key_id, const key_state)>   on_key_change_event;
	std::function<void (const std::string &)>           on_text_stream_event;
	
private:

  friend class caffeine::application::application;

  void                think();
  void                force_clear();
  void                pump_system_event(const void * event_data);

private:

	std::map<key_id, key_state> m_key_states;
	std::map<key_id, key_state> m_frame_key_states;
	
	float                     m_x_delta = 0.f;
	float                     m_y_delta = 0.f;


}; // class

} // namespace
} // namespace

#endif // include guard