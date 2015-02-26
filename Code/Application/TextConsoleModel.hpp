#ifndef TEXT_CONSOLE_MODEL_INCLUDED_335C9250_E154_440B_ADDA_85DAFEDAD3D2
#define TEXT_CONSOLE_MODEL_INCLUDED_335C9250_E154_440B_ADDA_85DAFEDAD3D2

#include <Caffeine/Common/Platform.hpp>
#include <string>
#include <vector>

class TextConsoleModel
{
public:

  explicit            TextConsoleModel(const uint32_t cols, const uint32_t rows);
                      ~TextConsoleModel();

  inline uint32_t     getColumns() const    { return m_cols; }
  inline uint32_t     getRows() const       { return m_rows; }
 
  float*              getPropertyData() const;
  inline bool         isDirty() const { return !m_pendingBuffer.empty() || !m_pendingInput.empty(); }

  void                prepareData();

private:

  friend class TextConsoleController;

  void                addStringToBuffer();
  void                addStringToInput();

private:

  uint32_t                    m_cols;
  uint32_t                    m_rows;

  std::vector<std::string>   m_pendingBuffer;
  std::vector<std::string>   m_pendingInput;

}; // class

#endif // include guard