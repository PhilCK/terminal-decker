
#include <Application/Console/TextConsoleView.hpp>
#include <Application/Console/TextConsoleModel.hpp>
#include <Caffeine/Application/Renderer/FrameBuffer.hpp>
#include <Caffeine/Application/Renderer/RendererDev.hpp>
#include <Caffeine/Common/Utilities/Directories.hpp>
#include <fstream>
#include <array>


FontDesc ConvertFontToConsole(FontData::FontDataInfo fontData)
{
  ConsoleFont font;
  std::vector<ConsoleChar> chars;

  FontDesc desc {font, chars};

  return desc;
}


TextConsoleView::TextConsoleView(const TextConsoleModel &model, FontData::FontDataInfo fontData)
: m_model(model)
{
  const uint32_t cols = m_model.getColumns();
  const uint32_t rows = m_model.getRows();

  // Simple Shader
  {
    const std::string filename = CaffUtil::GetPathDir() + "Shaders/SimpleShader.shd";
    const std::string shader(std::istreambuf_iterator<char>(std::ifstream(filename).rdbuf()), std::istreambuf_iterator<char>());

    m_simpleShader.loadShader(shader);
    assert(m_simpleShader.isValid());
  }

  // Console VF
  {
    std::vector<CaffApp::Dev::AttributeFormatDesc> vertFmtDesc = {{
      CaffApp::Dev::AttributeFormatDesc{"inPosition", CaffApp::Dev::AttrType::FLOAT2},
      CaffApp::Dev::AttributeFormatDesc{"inID", CaffApp::Dev::AttrType::FLOAT2},
    }};

    m_consoleGridVF.loadFormat(vertFmtDesc);
    assert(m_consoleGridVF.hasFormatedLoaded());
  }

  // Generate VBO a collection of points each point is a character.
  {
    std::vector<float> pointsVBO;
    const float sizeOfVert = 2.f; // x,y
    pointsVBO.reserve(cols * rows * sizeOfVert);

    for(uint32_t r = 0; r < rows; ++r)
    {
      for(uint32_t c = 0; c < cols; ++c)
      {
        // inPosition
        const float xUnits = 2.f / static_cast<float>(cols);
        pointsVBO.emplace_back((xUnits * c) - 1.f + (xUnits * 0.5f)); // unit - startpoint + half unit.

        const float yUnits = 2.f / static_cast<float>(rows);
        pointsVBO.emplace_back((yUnits * r) - 1.f + (yUnits * 0.5f)); // unit - startpoint + half unit.
        
        // inID
        const float uvUnitsX = 1.f / static_cast<float>(cols);
        const float uvUnitsY = 1.f / static_cast<float>(rows);

        pointsVBO.push_back(uvUnitsX * static_cast<float>(c));
        pointsVBO.push_back(uvUnitsY * static_cast<float>(r));
      }
    }

    // Generate Texture Lookups
    {
      m_textureLookup.loadTexture(m_model.getPropertyData());
      assert(m_textureLookup.isValid());
    }

    m_consoleGridVBO.loadVertexBuffer(pointsVBO, false);
    assert(m_consoleGridVBO.isValid());
  }

  const float sizeOfWidth  = (static_cast<float>(cols) * static_cast<float>(78 / 2.f)) / 4;
  const float sizeOfHeight = (static_cast<float>(rows) * static_cast<float>(78 / 2.f)) / 4;

  m_frameBuffer.loadBuffer(sizeOfWidth, sizeOfHeight);
}

void TextConsoleView::renderTextConsole()
{
  m_textureLookup.updateSubset(m_model.getPropertyData(), 0, 0);
  
  // Simple shader.
  CaffApp::Dev::Renderer::Reset();
  glPointSize(2.f);

  std::array<float, 2> uniSize = {{0.02f, 0.03f}};

  m_simpleShader.setShader2f("uniSize", uniSize);
  m_simpleShader.setTexture("dataLookup", m_textureLookup);
    
  m_frameBuffer.bind();
  m_simpleShader.bind();
  m_consoleGridVBO.bind(m_consoleGridVF, m_simpleShader);
      
  glDrawArrays(GL_POINTS, 0, 3200);
}
