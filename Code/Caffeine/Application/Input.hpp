

#ifndef CAFFEINE_APPLICATION_INPUT_INCLUDED
#define CAFFEINE_APPLICATION_INPUT_INCLUDED


#include <Caffeine/Application/ApplicationFwd.hpp>
#include <Caffeine/Common/Platform.hpp>
#include <Caffeine/Common/Utilities/NonCopyable.hpp>
#include <functional>
#include <memory>
#include <map>


namespace Caffeine {
namespace Application {


enum class KeyState {
	DOWN = 0,
	UP,
};


enum class KeyID
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


class Input : private CaffUtil::NonCopyable
{
public:

  explicit            Input();
                      ~Input();
	
	
	// ** Mouse Control ** //

	uint32_t            getMouseX() const;
	uint32_t            getMouseY() const;
	
	inline float				getMouseDeltaX() const { return m_xDelta; }
	inline float				getMouseDeltaY() const { return m_yDelta; }
	
	void                setMouseHold(const bool isHeld);


  // ** Keyboard Control ** //

  bool                isDown(const KeyID keyID) const;
  bool                isDownOnFrame(const KeyID keyID) const;
  bool                isUpOnFrame(const KeyID keyID) const;

  std::string         getKeyName(const KeyID keyID) const;

  void                setTextStream(const bool setTextStream);


  // ** Events ** //

	std::function<void (const float, const float)>      onMouseMovePositionEvent;
  std::function<void (const float, const float)>      onMouseDeltaEvent;
	std::function<void (const KeyID, const KeyState)>   onKeyChangeEvent;
	std::function<void (const std::string &)>           onTextStreamEvent;
	

private:

  friend class CaffApp::Application;

  // ** State changing hooks ** //

  void                update();
  void                clear();
  void                pumpSystemEvent(const void * sysEventData);


private:

	std::map<KeyID, KeyState> m_keystates;
	std::map<KeyID, KeyState> m_frameKeystates;
	
	float                     m_xDelta;
	float                     m_yDelta;


}; // class

} // namespace
} // namespace

#endif // include guard
