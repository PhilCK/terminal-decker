#ifndef SCENE_MODEL_INCLUDED_880C8E95_333C_4292_A1C4_6E5DA2C878B8
#define SCENE_MODEL_INCLUDED_880C8E95_333C_4292_A1C4_6E5DA2C878B8


#include <Caffeine/Common/Platform.hpp>
#include <Caffeine/Math/Matrix.hpp>


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

  void                  setFOV(const float current);

private:

  float m_currentFOV;
  float m_maxFOV;
  float m_minFOV;

  uint32_t m_width;
  uint32_t m_height;

}; // class


#endif // include guard