
#include <Application/Scene/SceneController.hpp>
#include <Application/Scene/SceneModel.hpp>
#include <Caffeine/Application/input.hpp>
#include <Caffeine/Math/Quaternion.hpp>


SceneController::SceneController(SceneModel &model)
: m_model(model)
, m_controllerState(SceneState::LOCKON_TARGET_0)
{
}


void SceneController::update(const float dt, const caff_app::input &input)
{
  // Do we need to change state
  {
    if(input.get_mouse_x_delta() != 0 || input.get_mouse_y_delta() != 0)
    {
      m_controllerState = SceneState::FREEVIEW;
    }

    if(input.is_down_on_frame(caff_app::key_id::KB_TAB))
    {
      m_controllerState = SceneState::LOCKON_TARGET_0;
      m_currentPitch = 0.3f;
      m_targetPitch = 0.3f;
      m_targetYaw = CaffMath::HalfTau();
    }
  }

  m_targetYaw   += (static_cast<float>(input.get_mouse_x_delta()) * dt);
  m_targetPitch += (static_cast<float>(input.get_mouse_y_delta()) * dt);

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
    const float newPos    = m_model.getCurrentFOV() + updatePos;

    m_model.setFOV(newPos);
  }

  // Head movement logic
  {
    CaffMath::Quaternion yawQuat    = CaffMath::QuaternionInitFromAngleAxis(m_targetYaw - CaffMath::QuartTau(), CaffMath::Vector3Init(0,-1,0));
    CaffMath::Quaternion pitchQuat  = CaffMath::QuaternionInitFromAngleAxis(m_targetPitch, CaffMath::Vector3Init(1,0,0));
    CaffMath::Quaternion rollQuat   = CaffMath::QuaternionInitFromAngleAxis(0, CaffMath::Vector3Init(0,0,1));

    CaffMath::Quaternion rollPitchQuat  = CaffMath::QuaternionMultiply(rollQuat, pitchQuat);
    CaffMath::Quaternion quat           = CaffMath::QuaternionMultiply(yawQuat, pitchQuat);
    
    //CaffMath::Quaternion rollPitchQuat  = CaffMath::QuaternionMultiply(yawQuat, pitchQuat);
    //CaffMath::Quaternion quat           = CaffMath::QuaternionMultiply(rollQuat, rollPitchQuat);

    m_model.setRotation(quat);
  }
}