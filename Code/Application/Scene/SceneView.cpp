
#include <Application/Scene/SceneView.hpp>
#include <Application/Scene/SceneModel.hpp>
#include <Caffeine/Common/Utilities/Directories.hpp>
#include <Caffeine/Application/Model.hpp>
#include <Caffeine/Application/Renderer/RendererDev.hpp>
#include <fstream>
#include <streambuf> 

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

  inline std::string GetShaderCode(const std::string &filename)
  {
    const std::string shaderCode(std::istreambuf_iterator<char>(std::ifstream(CaffUtil::GetPathDir() + filename).rdbuf()), std::istreambuf_iterator<char>());
    return shaderCode;
  }

}


SceneView::SceneView(const SceneModel &model)
: m_model(model)
, m_sceneVertexFormat(GetSceneVertexFmt())
, m_sceneShader(GetShaderCode("Shaders/Fullbright.shd"))
, m_laptopBuffer(GetGLBufferFromModel("Models/laptop.obj"))
, m_screenBuffer(GetGLBufferFromModel("Models/laptop_screen.obj"))
, m_laptopDiffuse(CaffUtil::GetPathDir() + "Textures/laptop_body.png")
{
  assert(m_sceneShader.isValid());
  assert(m_laptopDiffuse.isValid());

  // Upload some things
  {
    const CaffMath::Matrix44 projMat  = m_model.getProjectionMatrix();
    const CaffMath::Matrix44 modelMat = m_model.getModelMatrix();

    m_sceneShader.setShaderRaw("projMat", sizeof(projMat), &projMat._11);
    m_sceneShader.setShaderRaw("worldMat", sizeof(modelMat), &modelMat._11);
  }

}


void SceneView::draw(const CaffApp::Dev::Device &device, const CaffApp::Dev::FrameBuffer &laptopScreen)
{
  // Upload some things
  {
    const CaffMath::Matrix44 viewMat  = m_model.getViewMatrix();
    const CaffMath::Matrix44 modelMat = m_model.getModelMatrix();
    
        // Move
        static float spin = CaffMath::QuartTau();
        //spin += deltaTime;
        //
        const float x = CaffMath::Sin(spin) * 2.f;
        const float z = CaffMath::Cos(spin) * 2.f;
        const float y = CaffMath::Sin(spin) * 2.f;

        CaffMath::Vector3 eye   = CaffMath::Vector3Init(x, 1.7f, z);
        CaffMath::Vector3 look  = CaffMath::Vector3Init(0.f, 1.f, 0.f);
        CaffMath::Vector3 up    = CaffMath::Vector3Init(0.f, 1.f, 0.f);
        CaffMath::Matrix44 view = CaffMath::Matrix44LookAt(eye, up, look);

    m_sceneShader.setShaderRaw("viewMat", sizeof(view), &view._11);
    m_sceneShader.setShaderRaw("worldMat", sizeof(modelMat), &modelMat._11);
  }

  // Draw scene
  {
    m_sceneShader.setTexture("diffuseTex",  m_laptopDiffuse);
    CaffApp::Dev::Renderer::Draw(device, m_sceneShader, m_sceneVertexFormat, m_laptopBuffer);

    m_sceneShader.setTexture("diffuseTex", laptopScreen);
    CaffApp::Dev::Renderer::Draw(device, m_sceneShader, m_sceneVertexFormat, m_screenBuffer);
  }
}