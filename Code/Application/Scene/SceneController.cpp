
#include <Application/Scene/SceneController.hpp>
#include <Application/Scene/SceneModel.hpp>
#include <Caffeine/Application/Input.hpp>
#include <Caffeine/Math/Quaternion.hpp>


namespace
{
  const float freeviewFOVTarget = 3.143f / 1.5f;
  const float lockonFOVTarget = 3.143f / 6.f;
}


SceneController::SceneController(SceneModel &model)
: m_model(model)
, m_controllerState(SceneState::LOCKON_TARGET_0)
{
}


void SceneController::update(const float dt, const CaffApp::Input &input)
{
  // Do we need to change state
  {
    if(input.getMouseDeltaX() != 0 || input.getMouseDeltaY() != 0)
    {
      m_controllerState = SceneState::FREEVIEW;
    }

    if(input.isDownOnFrame(CaffApp::KeyID::KB_TAB))
    {
      m_controllerState = SceneState::LOCKON_TARGET_0;
      m_targetPitch = 0;
      m_targetYaw = 0;
    }
  }

  //

  m_targetYaw += (input.getMouseDeltaX() / 10.f);
  m_targetPitch += (input.getMouseDeltaY() / 10.f);

  switch(m_controllerState)
  {
    case(SceneState::FREEVIEW):
      m_targetFOV = m_model.getMaxFOV();
    break;

    case(SceneState::LOCKON_TARGET_0):
      m_targetFOV = m_model.getMinFOV();
    break;

    case(SceneState::SLEEP):
      return; // Don't need to do anymore calculations.
    break;

    default:
      assert(false); // shouldn't be here.
  }


  // Logic for FOV change
  {
    const float distance  = m_targetFOV - m_model.getCurrentFOV();
    const float updatePos = distance * (dt * 2);
    const float clamped   = CaffMath::Clamp(updatePos, m_model.getMinFOV(), m_model.getMaxFOV());
    
    const float newPos    = m_model.getCurrentFOV() + updatePos;

    m_model.setFOV(newPos);
  }

  // Head movement logic
  {
    CaffMath::Quaternion yawQuat    = CaffMath::QuaternionInitFromAngleAxis(m_targetYaw, CaffMath::Vector3Init(0,1,0));
    CaffMath::Quaternion pitchQuat  = CaffMath::QuaternionInitFromAngleAxis(m_targetPitch, CaffMath::Vector3Init(0,0,1));
    CaffMath::Quaternion rollQuat   = CaffMath::QuaternionInitFromAngleAxis(0, CaffMath::Vector3Init(1,0,0));

    CaffMath::Quaternion rollPitchQuat  = CaffMath::QuaternionMultiply(rollQuat, pitchQuat);
    CaffMath::Quaternion quat           = CaffMath::QuaternionMultiply(yawQuat, rollPitchQuat);

    m_model.setRotation(quat);
  }

   
}