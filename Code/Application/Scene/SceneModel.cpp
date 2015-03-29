
#include <Application/Scene/SceneModel.hpp>

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
  return CaffMath::Matrix44InitIdentity();
}


