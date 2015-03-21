#ifndef TEXT_CONSOLE_MODEL_INCLUDED_335C9250_E154_440B_ADDA_85DAFEDAD3D2
#define TEXT_CONSOLE_MODEL_INCLUDED_335C9250_E154_440B_ADDA_85DAFEDAD3D2

#include <Application/Console/TextDataParse.hpp>
#include <Caffeine/Common/Platform.hpp>
#include <string>
#include <vector>
#include <mutex>
#include <deque>

class TextConsoleModel
{
public:

  explicit                    TextConsoleModel(const uint32_t cols, const uint32_t rows, FontData::FontDataInfo fontData);
                              ~TextConsoleModel() {}

  inline uint32_t             getColumns() const    { return m_cols; }
  inline uint32_t             getRows() const       { return m_rows; }
  uint32_t                    getMaxCharWidth() const;
  uint32_t                    getMaxCharHeight() const;
  uint32_t                    getLineHeight() const;
 
  const std::vector<float>&   getPropertyData() const;
  inline uint32_t             getNumberOfCharactersInData() const { return m_numberOfCharsInData; }
  uint32_t                    getSizeOfProperty() const;

  inline bool                 requriesUpdate() const { return m_bufferDirty; }
  void                        prepareData();

  inline std::string          getInput() const  { return m_input;  }

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
  std::vector<std::string>    m_bufferHistory;
  std::vector<float>          m_characterProperties;
  mutable std::mutex          m_modelMutex;
  std::string                 m_input                 = "";
  std::size_t                 m_currentBufferPosition = 0;
  uint32_t                    m_cols                  = 0;
  uint32_t                    m_rows                  = 0;
  uint32_t                    m_numberOfCharsInData   = 0;
  bool                        m_bufferDirty           = true;


}; // class

#endif // include guard