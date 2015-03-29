#ifndef SCENE_CONTROLLER_INCLUDED_6CB7E681_B740_4F0B_AED8_9A63A911AAD8
#define SCENE_CONTROLLER_INCLUDED_6CB7E681_B740_4F0B_AED8_9A63A911AAD8


#include <Caffeine/Common/Platform.hpp>
#include <Caffeine/Common/Utilities/NonCopyable.hpp>
#include <Application/Scene/SceneFwd.hpp>
#include <Caffeine/Application/ApplicationFwd.hpp>


class SceneController : private CaffUtil::NonCopyable
{
public:

  explicit          SceneController(SceneModel &model);

  void              update(const float dt, const CaffApp::Input &input);

private:

  enum  class       SceneState { FREEVIEW, LOCKON_TARGET_0, SLEEP, };

  SceneModel        &m_model;
  SceneState        m_controllerState           = SceneState::LOCKON_TARGET_0;

  float             m_currentYaw = 0.f;
  float             m_targetYaw = 0.f;

  float             m_currentPitch = 0.f;
  float             m_targetPitch = 0.f;

  float             m_targetFOV = 0.f;

}; // class


#endif // include guard