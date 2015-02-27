#ifndef TEXT_CONSOLE_MODEL_INCLUDED_335C9250_E154_440B_ADDA_85DAFEDAD3D2
#define TEXT_CONSOLE_MODEL_INCLUDED_335C9250_E154_440B_ADDA_85DAFEDAD3D2

#include <Application/Console/TextDataParse.hpp>
#include <Caffeine/Common/Platform.hpp>
#include <string>
#include <vector>
#include <mutex>

class TextConsoleModel
{
public:

  explicit                    TextConsoleModel(const uint32_t cols, const uint32_t rows, FontData::FontDataInfo fontData);
                              ~TextConsoleModel() {}

  inline uint32_t             getColumns() const    { return m_cols; }
  inline uint32_t             getRows() const       { return m_rows; }
 
  const std::vector<float>&   getPropertyData() const;
  uint32_t                    getSizeOfProperty() const;

  inline bool                 requriesUpdate() const { return !m_pendingBuffer.empty() || !m_pendingInput.empty(); }
  void                        prepareData();

private:

  friend class TextConsoleController;

  void                        addStringToBuffer(const std::string &str);
  void                        addStringToInput(const std::string &str);

private:

  uint32_t                    m_cols      = 0;
  uint32_t                    m_rows      = 0;

  mutable std::mutex          m_controllerMutex;
  mutable std::mutex          m_modelMutex;

  std::vector<std::string>    m_buffer;
  std::vector<std::string>    m_input;
  std::vector<float>          m_characterProperties;

  std::vector<std::string>    m_pendingBuffer;
  std::vector<std::string>    m_pendingInput;

  FontData::FontDataInfo      m_fontData;

}; // class

#endif // include guard