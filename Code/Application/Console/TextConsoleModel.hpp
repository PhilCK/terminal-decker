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
  inline uint32_t             getNumberOfCharactersInData() const { return m_numberOfCharsInData; }
  uint32_t                    getSizeOfProperty() const;

  inline bool                 requriesUpdate() const { return !m_pendingBuffer.empty() || !m_pendingInput.empty(); }
  void                        prepareData();

  inline std::string          getInput() const  { return m_input;  }
  inline std::string          getBuffer() const { return m_buffer; }

  void                        getCursorX();
  void                        getCursorY();
  void                        setCursor(const uint32_t x, const uint32_t y);
  void                        setCharacter(const uint32_t x, const uint32_t y);
  void                        blitScreen(const std::vector<int> markup);
  void                        addLineToScreen(const std::vector<int> markup);

private:

  friend class TextConsoleController;

  void                        addStringToBuffer(const std::string &str);
  void                        clearBuffer();
  void                        addStringToInput(const std::string &str);
  void                        clearInput();
  void                        backspaceInput();

private:

  FontData::FontDataInfo      m_fontData;

  std::vector<float>          m_characterProperties;
  
  std::string                 m_buffer;
  std::string                 m_pendingBuffer;
  std::string                 m_input;
  std::string                 m_pendingInput;

  mutable std::mutex          m_controllerMutex;  // Controller Setters
  mutable std::mutex          m_modelMutex;       // Model Getters
  
  uint32_t                    m_cols                = 0;
  uint32_t                    m_rows                = 0;
  uint32_t                    m_numberOfCharsInData = 0;

}; // class

#endif // include guard