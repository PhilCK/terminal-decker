#ifndef SCENE_VIEW_INCLUDED_6F65D0AA_9B92_4AC0_8672_B6FF29A11E8E
#define SCENE_VIEW_INCLUDED_6F65D0AA_9B92_4AC0_8672_B6FF29A11E8E


#include <Application/Scene/SceneFwd.hpp>
#include <Caffeine/Common/Platform.hpp>
#include <Caffeine/Common/Utilities/NonCopyable.hpp>
#include <Caffeine/Application/Renderer/Shader.hpp>
#include <Caffeine/Application/Renderer/VertexFormat.hpp>
#include <Caffeine/Application/Renderer/VertexBuffer.hpp>
#include <Caffeine/Application/Renderer/FrameBuffer.hpp>
#include <Caffeine/Application/Renderer/Texture.hpp>
#include <Caffeine/Application/Renderer/Device.hpp>


class SceneView : private CaffUtil::NonCopyable
{
public:

  explicit                    SceneView(const SceneModel &model);

  void                        draw(const CaffApp::Dev::Device &device, const CaffApp::Dev::FrameBuffer &laptopScreen);


private:

  const SceneModel                  &m_model;
  const CaffApp::Dev::VertexFormat  m_sceneVertexFormat;
  const CaffApp::Dev::VertexBuffer  m_laptopBuffer;
  const CaffApp::Dev::VertexBuffer  m_screenBuffer;
  const CaffApp::Dev::VertexBuffer  m_roomBuffer;
  const CaffApp::Dev::Texture       m_laptopDiffuse;
  const CaffApp::Dev::Texture       m_roomDiffuse;
  CaffApp::Dev::Shader              m_sceneShader;
  CaffApp::Dev::Shader              m_lightingShader;

}; // class


#endif // include guard