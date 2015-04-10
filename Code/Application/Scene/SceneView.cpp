
#include <Application/Scene/SceneView.hpp>
#include <Application/Scene/SceneModel.hpp>
#include <Caffeine/Common/Utilities/Directories.hpp>
#include <Caffeine/Application/Model.hpp>
#include <Caffeine/Application/Renderer/RendererDev.hpp>


namespace
{
  inline std::vector<CaffApp::Dev::AttributeFormatDesc> GetSceneVertexFmt()
  {
    const std::vector<CaffApp::Dev::AttributeFormatDesc> vertDesc = {
      CaffApp::Dev::AttributeFormatDesc{"inPosition", CaffApp::Dev::AttrType::FLOAT3},
      CaffApp::Dev::AttributeFormatDesc{"inTexC", CaffApp::Dev::AttrType::FLOAT2},
      CaffApp::Dev::AttributeFormatDesc{"inNormal", CaffApp::Dev::AttrType::FLOAT3},
    };

    return vertDesc;
  }

  inline std::vector<float> GetGLBufferFromModel(const std::string &filename)
  {
    CaffApp::Model model;

    const bool loaded = model.loadModel(CaffUtil::GetPathDir() + filename);
    assert(loaded);

    return model.getMesh(0).getGLVertexBuffer();
  }
}


SceneView::SceneView(const SceneModel &model)
: m_model(model)
, m_sceneVertexFormat(GetSceneVertexFmt())
, m_sceneShader(CaffApp::Dev::ShaderUtil::GetShaderCodeFromFile(CaffUtil::GetPathDir() + "Shaders/Fullbright.shd"))
, m_lightingShader(CaffApp::Dev::ShaderUtil::GetShaderCodeFromFile(CaffUtil::GetPathDir() + "Shaders/ForwardLighting.shd"))
, m_laptopBuffer(GetGLBufferFromModel("Models/laptop.obj"))
, m_screenBuffer(GetGLBufferFromModel("Models/laptop_screen.obj"))
, m_roomBuffer(GetGLBufferFromModel("Models/room.obj"))
, m_laptopDiffuse(CaffUtil::GetPathDir() + "Textures/laptop_body.png")
, m_roomDiffuse(CaffUtil::GetPathDir() + "Textures/room.png")
{
  assert(m_sceneShader.isValid());
  assert(m_lightingShader.isValid()); 
  assert(m_laptopDiffuse.isValid());

  // Upload some things
  {
    const CaffMath::Matrix44 projMat  = m_model.getProjectionMatrix();
    const CaffMath::Matrix44 modelMat = m_model.getModelMatrix();

    m_sceneShader.setShaderRaw("projMat", sizeof(projMat), &projMat._11);
    m_sceneShader.setShaderRaw("worldMat", sizeof(modelMat), &modelMat._11);

    m_lightingShader.setShaderRaw("projMat", sizeof(projMat), &projMat._11);
    m_lightingShader.setShaderRaw("worldMat", sizeof(modelMat), &modelMat._11);
  }

}


void SceneView::draw(const CaffApp::Dev::Device &device, const CaffApp::Dev::FrameBuffer &laptopScreen)
{
  const CaffMath::Matrix44 modelMat = m_model.getModelMatrix();
  const CaffMath::Matrix44 viewMat  = m_model.getViewMatrix();

  // Upload some things
  {
    
    //const CaffMath::Matrix44 modelMat = m_model.getModelMatrix();
    const CaffMath::Matrix44 projMat  = m_model.getProjectionMatrix();

    m_sceneShader.setShaderRaw("projMat", sizeof(projMat), &projMat._11);
    m_sceneShader.setShaderRaw("viewMat", sizeof(viewMat), &viewMat._11);
    m_sceneShader.setShaderRaw("worldMat", sizeof(modelMat), &modelMat._11);

    m_lightingShader.setShaderRaw("projMat", sizeof(projMat), &projMat._11);
    m_lightingShader.setShaderRaw("viewMat", sizeof(viewMat), &viewMat._11);
    m_lightingShader.setShaderRaw("worldMat", sizeof(modelMat), &modelMat._11);
  }

  // Draw scene
  {
    CaffApp::Dev::Renderer::Reset();

    m_lightingShader.setTexture("diffuseTex", m_roomDiffuse);





    const std::array<float, 3> pos = {{ 3.f, 1.f, 0 }};

    m_lightingShader.setShader3f("eyePos", pos);
    CaffApp::Dev::Renderer::Draw(device, m_lightingShader, m_sceneVertexFormat, m_roomBuffer);

    m_sceneShader.setTexture("diffuseTex",  m_laptopDiffuse);
    CaffApp::Dev::Renderer::Draw(device, m_sceneShader, m_sceneVertexFormat, m_laptopBuffer);

    m_sceneShader.setTexture("diffuseTex", laptopScreen);
    CaffApp::Dev::Renderer::Draw(device, m_sceneShader, m_sceneVertexFormat, m_screenBuffer);   
  }
}