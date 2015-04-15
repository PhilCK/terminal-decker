#ifndef SCENE_MODEL_INCLUDED_880C8E95_333C_4292_A1C4_6E5DA2C878B8
#define SCENE_MODEL_INCLUDED_880C8E95_333C_4292_A1C4_6E5DA2C878B8


#include <Caffeine/Common/Platform.hpp>
#include <Caffeine/Math/Matrix.hpp>
#include <Caffeine/Math/Quaternion.hpp>


class SceneModel
{
public:

  explicit              SceneModel();

  inline float          getMaxFOV() const     { return m_maxFOV;      }
  inline float          getMinFOV() const     { return m_minFOV;      }
  inline float          getCurrentFOV() const { return m_currentFOV;  }
  
  CaffMath::Matrix44    getProjectionMatrix() const;
  CaffMath::Matrix44    getViewMatrix() const;
  CaffMath::Matrix44    getModelMatrix() const;

private:

  friend class SceneController;

  void                  setFOV(const float current);
  void                  setRotation(const CaffMath::Quaternion &quat);

private:

  float m_currentFOV;
  float m_maxFOV = 3.142f / 3.f;
  float m_minFOV = 3.142f / 6.f;

  uint32_t m_width;
  uint32_t m_height;

  CaffMath::Matrix44 m_modelMatrix = CaffMath::Matrix44InitIdentity();
  CaffMath::Matrix44 m_viewMatrix = CaffMath::Matrix44InitIdentity();

}; // class


#endif // include guard