
#include <Application/Console/TextConsoleModel.hpp>
#include <Caffeine/Math/Math.hpp>


namespace
{
  enum CharProperties { U_COORD, V_COORD, CHAR_PROP_SIZE, };
}


TextConsoleModel::TextConsoleModel(const uint32_t cols, const uint32_t rows)
: m_cols(cols)
, m_rows(rows)
{
  prepareData();
}


const std::vector<float>& TextConsoleModel::getPropertyData() const
{
  std::lock_guard<std::mutex> lockModel(m_modelMutex);

  return m_characterProperties;
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
    m_characterProperties.clear();

    const uint32_t sizeOfData = (m_cols * m_rows) * CHAR_PROP_SIZE;

    const float randVal = CaffMath::RandFloatRange(0.1f, 1.0f);

    for(uint32_t i = 0; i < sizeOfData; ++i)
    {
      const float data = randVal + 0.1f;
      m_characterProperties.push_back(data);
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
