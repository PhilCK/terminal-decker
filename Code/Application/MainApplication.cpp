
#include <Caffeine/Application/Application.hpp>
#include <Caffeine/Application/Model.hpp>
#include <Caffeine/Application/Renderer.hpp>
#include <Caffeine/Math/Matrix.hpp>
#include <Caffeine/Math/Vector.hpp>
#include <Caffeine/Common/Utilities/Directories.hpp>
#include <Caffeine/Common/Utilities/Logging.hpp>
#include <glm/glm.hpp>
#include <glm/GTC/matrix_transform.hpp>

#include <fstream>
#include <streambuf> 
#include <Caffeine/Application/Renderer/Shader.hpp>
#include <Caffeine/Application/Renderer/VertexFormat.hpp>
#include <Caffeine/Application/Renderer/VertexBuffer.hpp>
#include <Caffeine/Application/Renderer/Texture.hpp>
#include <Caffeine/Application/Renderer/FrameBuffer.hpp>
#include <Caffeine/Application/Renderer/Device.hpp>
#include <Caffeine/Application/Renderer/RendererDev.hpp>

#include <Application/TextConsole.hpp>

#include <Awesomium/WebCore.h>

namespace
{
  CaffApp::Model          model;

  CaffApp::Dev::Shader        caffAppShader;
  CaffApp::Dev::Shader        caffAppPostShader;
  CaffApp::Dev::VertexFormat  caffAppVertexFormat;
  CaffApp::Dev::VertexFormat  caffAppPostVertexFormat;
  CaffApp::Dev::Texture       caffAppTexture;
  CaffApp::Dev::VertexBuffer  caffAppVertexBuffer;
  CaffApp::Dev::VertexBuffer  caffAppPostVertexBuffer;
  CaffApp::Dev::FrameBuffer   caffAppFrameBuffer;
  
  std::unique_ptr<TextConsole> textConsole;

  std::vector<float> fullscreenVerts = {{
    -1.f, -1.f, 0.f, 0.f,
    +3.f, -1.f, 2.f, 0.f,
    -1.f, +3.f, 0.f, 2.f,
  }};

}


namespace Terminal {

class Application
{
public:

  explicit Application()
  : m_caffApp("Terminal", 864 * 2, 486 * 2, false)
  , m_projectionMatrix(CaffMath::Matrix44Projection(3.142f / 3.f, 864.f, 486.f, 0.1f, 1000.f))
  , m_viewMatrix(CaffMath::Matrix44InitIdentity())
  , m_worldMatrix(CaffMath::Matrix44InitIdentity())
  {
    // Logging
    CaffUtil::SetLogLevel(CaffUtil::LogLevel::ERROR_LOG | CaffUtil::LogLevel::INFO_LOG | CaffUtil::LogLevel::WARNING_LOG);
    CaffUtil::SetOutputLogTargets(CaffUtil::LogOutput::CONSOLE);


    model.loadModel(CaffUtil::GetPathDir() + "Models/unit_cube.obj");

    // Nooo Renderer stuff
    {
      const std::string filename = CaffUtil::GetPathDir() + "Shaders/Fullbright.shd";
      const std::string shaderFullbright(std::istreambuf_iterator<char>(std::ifstream(filename).rdbuf()), std::istreambuf_iterator<char>());
      caffAppShader.loadShader(shaderFullbright);
    }

    {
      const std::string filename = CaffUtil::GetPathDir() + "Shaders/Post.shd";
      const std::string shaderPost(std::istreambuf_iterator<char>(std::ifstream(filename).rdbuf()), std::istreambuf_iterator<char>());
      caffAppPostShader.loadShader(shaderPost);
    }
    
    std::vector<CaffApp::Dev::AttributeFormatDesc> vertDesc;
    vertDesc.emplace_back(CaffApp::Dev::AttributeFormatDesc{"inPosition", CaffApp::Dev::AttrType::FLOAT3});
    vertDesc.emplace_back(CaffApp::Dev::AttributeFormatDesc{"inTexC", CaffApp::Dev::AttrType::FLOAT2});
    vertDesc.emplace_back(CaffApp::Dev::AttributeFormatDesc{"inNormal", CaffApp::Dev::AttrType::FLOAT3});

    caffAppVertexFormat.loadFormat(vertDesc);

    vertDesc.clear();

    vertDesc.emplace_back(CaffApp::Dev::AttributeFormatDesc{"position", CaffApp::Dev::AttrType::FLOAT2});
    vertDesc.emplace_back(CaffApp::Dev::AttributeFormatDesc{"texcoord", CaffApp::Dev::AttrType::FLOAT2});

    caffAppPostVertexFormat.loadFormat(vertDesc);

    caffAppTexture.loadTexture(CaffUtil::GetPathDir() + "Textures/dev_grid_blue.png");

    caffAppVertexBuffer.loadVertexBuffer(model.getMesh(0).getGLVertexBuffer());
    caffAppPostVertexBuffer.loadVertexBuffer(fullscreenVerts);

    caffAppFrameBuffer.loadBuffer(864.f, 486.f);


    caffAppShader.setShaderRaw("projMat",   sizeof(float) * 16, &m_projectionMatrix._11);
    caffAppShader.setShaderRaw("worldMat",  sizeof(float) * 16, &m_worldMatrix._11);

    m_caffApp.getRenderer().setViewPort(864 * 2, 486 * 2);

    textConsole.reset(new TextConsole());

  }


  void start()
  {
    while(!m_caffApp.shouldQuit())
    {
      m_caffApp.startFrame();

      const float32_t deltaTime = m_caffApp.getDeltaTime();
      

      // Render
      {
        auto &renderer = m_caffApp.getRenderer();

        static float spin = 0.f;
        spin += deltaTime;
        
        const float x = CaffMath::Sin(spin) * 2.f;
        const float z = CaffMath::Cos(spin) * 2.f;
        const float y = CaffMath::Sin(spin) * 2.f;

        CaffMath::Vector3 eye   = CaffMath::Vector3Init(x, 2.f, z);
        CaffMath::Vector3 look  = CaffMath::Vector3Init(0.f, 0.f, 0.f);
        CaffMath::Vector3 up    = CaffMath::Vector3Init(0.f, 1.f, 0.f);
        CaffMath::Matrix44 view = CaffMath::Matrix44LookAt(eye, up, look);

        // Draw geometry
        {
          caffAppFrameBuffer.clear(true, true);

          CaffApp::Dev::Renderer::Reset();
          caffAppShader.setShaderRaw("viewMat",   sizeof(float) * 16, &view._11);
          caffAppShader.setTexture("diffuseTex",  caffAppTexture);

          //CaffApp::Dev::Renderer::Draw(caffAppFrameBuffer, caffAppShader, caffAppVertexFormat, caffAppVertexBuffer);
        }

        {
          textConsole->renderTextConsole(caffAppFrameBuffer);
        }

        // Draw post
        {
          CaffApp::Dev::Renderer::Reset();
          glDisable(GL_DEPTH_TEST);

          caffAppPostShader.setTexture("texFramebuffer", caffAppFrameBuffer);

          CaffApp::Dev::Renderer::Draw(m_caffApp.getRenderer(), caffAppPostShader, caffAppPostVertexFormat, caffAppPostVertexBuffer);
        }

        GL_ERROR("End of frame");
      }
      
      m_caffApp.endFrame();
    }
  }

private:

  CaffApp::Application    m_caffApp;
  CaffMath::Matrix44      m_projectionMatrix;
  CaffMath::Matrix44      m_viewMatrix;
  CaffMath::Matrix44      m_worldMatrix;

};

} // namespace



int main(int argc, char **argv)
{
  Terminal::Application app;
  app.start();

  return 0;
}