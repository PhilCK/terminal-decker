
#include <Application/Console/TextConsoleModel.hpp>
#include <Caffeine/Math/Math.hpp>


namespace
{
  std::size_t bufferHistorySize = 80;

  enum CharProperties { U_COORD = 0, V_COORD, WIDTH, HEIGHT, X_OFFSET, Y_OFFSET, X_ADVANCE, Y_ADVANCE, COLOR_R, COLOR_G, COLOR_B, PADDING, PADDING_2, PADDING_3, PADDING_4, PADDING_5, CHAR_PROP_SIZE, };
}


TextConsoleModel::TextConsoleModel(const uint16_t cols, const uint16_t rows, FontData::FontDataInfo fontData)
: m_charSize((cols << 16) | (rows))
, m_fontData(fontData)
{
  // Reserve enough space for our texture data
  {
    assert((cols * rows) < 4096);

    m_characterProperties.resize((4096 * 4) * 4, 128);
  }

  // Buffer history setup
  {
    bufferHistorySize = rows * 2;  
    m_bufferHistory.resize(bufferHistorySize);
  }
}


uint32_t TextConsoleModel::getMaxCharWidth() const  { return m_fontData.maxWidth;   }
uint32_t TextConsoleModel::getMaxCharHeight() const { return m_fontData.maxHeight;  }
uint32_t TextConsoleModel::getLineHeight() const    { return m_fontData.lineHeight; }


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
  if(!requriesUpdate())
  {
    return;
  }

  std::vector<std::string> outputScreen(m_bufferHistory.size());

  // Copy data
  {
    std::lock_guard<std::mutex> lockModel(m_modelMutex);

    m_bufferDirty = false;

    const auto pivot = m_currentBufferPosition % m_bufferHistory.size();
    std::rotate_copy(m_bufferHistory.begin(), m_bufferHistory.begin() + pivot, m_bufferHistory.end(), outputScreen.begin());
  }

  // Build data
  {
    std::lock_guard<std::mutex> lockModel(m_modelMutex);

    int i = 0;
    m_numberOfCharsInData = 0;

    float xPosition = 0;
    float yPosition = 0;

    auto GenerateTextureInfo = [&](const std::string &str)
    {
      for(const auto &c : str)
      {
        const char lineEnd = '\n';

        if(lineEnd == c)
        {
          continue;
        }

        const char backspace = '\b';

        if(backspace == c)
        {
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

        m_characterProperties.at(i++) = xPosition;
        m_characterProperties.at(i++) = yPosition;

        m_characterProperties.at(i++) = 0.1f;
        m_characterProperties.at(i++) = 1.0f;
        m_characterProperties.at(i++) = 0.0f;

        m_characterProperties.at(i++) = 0.f; // padding
        m_characterProperties.at(i++) = 0.f; // padding
        m_characterProperties.at(i++) = 0.f; // padding
        m_characterProperties.at(i++) = 0.f; // padding
        m_characterProperties.at(i++) = 0.f; // padding

        xPosition += (static_cast<float>(character.xAdvance)) + 1;// / static_cast<float>(m_fontData.scaleWidth));
      }
    };

    // BufferData
    {
      std::size_t i = CaffMath::Abs(outputScreen.size() - getRows() + 1);
      for(; i < outputScreen.size(); ++i)
      {
        GenerateTextureInfo(outputScreen.at(i));

        yPosition += m_fontData.lineHeight;
        xPosition = 0;
      }
    }

    // Input line data
    {
      const std::size_t rowsWithoutInput = getRows() - 1;
      yPosition = static_cast<float>(rowsWithoutInput) * m_fontData.lineHeight;
      xPosition = 0;

      GenerateTextureInfo(m_inputPrompt + m_input);
    }
  }
}


void TextConsoleModel::addStringToBuffer(const std::string &str)
{
  std::lock_guard<std::mutex> lockModel(m_modelMutex);

  m_bufferDirty = true;
  m_bufferHistory.at(m_currentBufferPosition % bufferHistorySize) = str;
  m_currentBufferPosition++;
}


void TextConsoleModel::clearBuffer()
{
  std::lock_guard<std::mutex> lockModel(m_modelMutex);

  m_bufferDirty = true;
  m_bufferHistory.clear();
  m_currentBufferPosition = 0;
}


void TextConsoleModel::addStringToInput(const std::string &str)
{
  std::lock_guard<std::mutex> lockModel(m_modelMutex);

  m_bufferDirty = true;
  m_input.append(str);
}


void TextConsoleModel::clearInput()
{
  std::lock_guard<std::mutex> lockModel(m_modelMutex);

  m_bufferDirty = true;
  m_input.clear();
  m_input.clear();
}


void TextConsoleModel::backspaceInput()
{
  std::lock_guard<std::mutex> lockModel(m_modelMutex);

  m_bufferDirty = true;
  m_input = m_input.substr(0, m_input.size() - 1);
}


void TextConsoleModel::setInputPrompt(const std::string &str)
{
  std::lock_guard<std::mutex> lockModel(m_modelMutex);

  m_bufferDirty = true;
  m_inputPrompt = str;
}