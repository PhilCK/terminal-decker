
#include <Application/Scene/SceneModel.hpp>


SceneModel::SceneModel()
{
  m_width = 1280;
  m_height = 720;
  m_currentFOV = 3.142f / 6.f;
}


CaffMath::Matrix44 SceneModel::getProjectionMatrix() const
{
  return CaffMath::Matrix44Projection(m_currentFOV, static_cast<float>(m_width), static_cast<float>(m_height), 0.1f, 1000.f);
}


CaffMath::Matrix44 SceneModel::getViewMatrix() const
{
  return m_viewMatrix;
}


CaffMath::Matrix44 SceneModel::getModelMatrix() const
{
  return m_modelMatrix;
}


void SceneModel::setFOV(const float currentFOV)
{
  m_currentFOV = CaffMath::Clamp(currentFOV, m_minFOV, m_maxFOV);
}


void SceneModel::setRotation(const CaffMath::Quaternion &quat)
{
  m_curr_rot = quat;

  const CaffMath::Vector3 pos = CaffMath::Vector3Init(3.f, 2.f, 0.f);

  const CaffMath::Vector3 fwd     = CaffMath::Vector3Init(0.f, 0.f, 1.f);
  const CaffMath::Vector3 viewDir = CaffMath::QuaternionRotate(m_curr_rot, fwd);
  const CaffMath::Vector3 viewAt  = CaffMath::Vector3ComponentAdd(viewDir, pos);

  m_viewMatrix = CaffMath::Matrix44LookAt(pos, CaffMath::Vector3Init(0.f, 1.f, 0.f), viewAt);
}
