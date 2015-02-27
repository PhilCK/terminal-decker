
#include <Application/Console/TextConsoleModel.hpp>
#include <Caffeine/Math/Math.hpp>


namespace
{
  enum CharProperties { U_COORD, V_COORD, CHAR_PROP_SIZE, };
}


TextConsoleModel::TextConsoleModel(const uint32_t cols, const uint32_t rows, FontData::FontDataInfo fontData)
: m_cols(cols)
, m_rows(rows)
, m_fontData(fontData)
{
  m_characterProperties.resize(m_cols * m_rows * CHAR_PROP_SIZE);
}


const std::vector<float>& TextConsoleModel::getPropertyData() const
{
  std::lock_guard<std::mutex> lockModel(m_modelMutex);

  return m_characterProperties;
}


uint32_t TextConsoleModel::getSizeOfProperty() const
{
  return static_cast<uint32_t>(CHAR_PROP_SIZE);
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
    //m_characterProperties.clear();

    //const std::string testStr = "void HelloWorld() const { std::cout << \"Hello\" << std::endl; }, id HelloWorld() const { std::cout << \"Hello\" << std::endl; }, id HelloWorld() const { std::cout << \"Hello\" << std::endl; }";

    const std::string testStr = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";

    int i = 0;

    for(const auto &c : testStr)
    {
      const auto &character = m_fontData.characters[c];

      // Get UV Data.
      const float u = static_cast<float>(character.x) / static_cast<float>(m_fontData.scaleWidth);
      const float v = static_cast<float>(character.y) / static_cast<float>(m_fontData.scaleHeight);

      m_characterProperties.at(i) = (u);
      m_characterProperties.at(i) = (v);

      // Get Height and Width
      const float w = static_cast<float>(character.width) / static_cast<float>(m_fontData.scaleWidth);
      const float h = static_cast<float>(character.height) / static_cast<float>(m_fontData.scaleHeight);

      m_characterProperties.at(i) = (w);
      m_characterProperties.at(i) = (h);

      ++i;
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
