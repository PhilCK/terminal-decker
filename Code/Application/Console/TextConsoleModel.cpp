
#include <Application/Console/TextConsoleModel.hpp>
#include <Caffeine/Math/Math.hpp>


namespace
{
  enum CharProperties { U_COORD = 0, V_COORD, WIDTH, HEIGHT, X_OFFSET, Y_OFFSET, X_ADVANCE, Y_ADVANCE, CHAR_PROP_SIZE, };
}


TextConsoleModel::TextConsoleModel(const uint32_t cols, const uint32_t rows, FontData::FontDataInfo fontData)
: m_cols(cols)
, m_rows(rows)
, m_fontData(fontData)
{
  m_characterProperties.resize(64 * 64 * 4, 128);
}


const std::vector<float>& TextConsoleModel::getPropertyData() const
{
  std::lock_guard<std::mutex> lockModel(m_modelMutex);

  return m_characterProperties;
}


uint32_t TextConsoleModel::getSizeOfProperty() const
{
  return static_cast<uint32_t>(CHAR_PROP_SIZE / 4);
}

void TextConsoleModel::prepareData()
{
  std::vector<std::string> pendingBuffer;
  std::vector<std::string> pendingInput;


  // Copy buffers
  {
    std::lock_guard<std::mutex> lockController(m_controllerMutex);

    pendingBuffer.swap(m_pendingBuffer);
    m_pendingBuffer.clear();

    pendingInput.swap(m_pendingInput);
    m_pendingInput.clear();
  }

  // Build data
  {
    std::lock_guard<std::mutex> lockModel(m_modelMutex);

    //const std::string testStr = "void HelloWorld() const { std::cout << \"Hello\" << std::endl; }, id HelloWorld() const { std::cout << \"Hello\" << std::endl; }, id HelloWorld() const { std::cout << \"Hello\" << std::endl; }";







    //const std::string testStr = "\"mop\"";
    const std::string testStr = "void foo() \n{\n   std::cout << \"moop\"; \n}";

    //m_characterProperties.resize(testStr.size() * CHAR_PROP_SIZE);

    int i = 0;
    m_numberOfCharsInData = 0;

    float xPosition = 0;
    float yPosition = 0;

    for(const auto &c : testStr)
    //for(int t = 0; t < m_characterProperties.capacity() / 4; ++t)
    {
      const char lineEnd = '\n';

      if(lineEnd == c)
      {
        yPosition += m_fontData.lineHeight;
        xPosition = 0;
        continue;
      }


      m_numberOfCharsInData++;
      const auto &character = m_fontData.characters[c];
      //const auto &character = m_fontData.characters[testStr.front()];

      // Get UV Data.
      const float u = static_cast<float>(character.x) / static_cast<float>(m_fontData.scaleWidth);
      const float v = static_cast<float>(character.y) / static_cast<float>(m_fontData.scaleHeight);

      // Get Height and Width
      const float w = static_cast<float>(character.width);// / static_cast<float>(m_fontData.scaleWidth);
      const float h = static_cast<float>(character.height);// / static_cast<float>(m_fontData.scaleHeight);

      // Offset
      
      const float xOffset = static_cast<float>(character.xOffset);
      const float yOffset = static_cast<float>(character.yOffset);

      // Push data
      m_characterProperties.at(i++) = (u); // r b 
      m_characterProperties.at(i++) = (v); // g g 
      m_characterProperties.at(i++) = (w); // b r 
      m_characterProperties.at(i++) = (h); // a a 
      m_characterProperties.at(i++) = (xOffset);
      m_characterProperties.at(i++) = (yOffset);

      const float padding = 0.f;
      m_characterProperties.at(i++) = xPosition;
      m_characterProperties.at(i++) = yPosition;

      xPosition += (static_cast<float>(character.xAdvance)) + 1;// / static_cast<float>(m_fontData.scaleWidth));
    }
  }
}



void TextConsoleModel::addStringToBuffer(const std::string &str)
{
  std::lock_guard<std::mutex> lockController(m_controllerMutex);

  m_pendingBuffer.emplace_back(str);
}


void TextConsoleModel::addStringToInput(const std::string &str)
{
  std::lock_guard<std::mutex> lockController(m_controllerMutex);

  m_pendingInput.emplace_back(str);
}
