
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
  return CaffMath::Matrix44InitIdentity();
}


CaffMath::Matrix44 SceneModel::getModelMatrix() const
{
  return m_modelMatrix;
}


void SceneModel::setFOV(const float currentFOV)
{
  m_currentFOV = CaffMath::Clamp(currentFOV, m_minFOV, m_maxFOV);

  std::cout << m_currentFOV << std::endl;
}


void SceneModel::setRotation(const CaffMath::Quaternion &quat)
{
  CaffMath::Matrix33 rot = CaffMath::QuaternionToRotationMatrix(quat);
  m_modelMatrix = CaffMath::Matrix44InitWithRotationAndPosition(rot, CaffMath::Vector3Init(0,0,0));
}
