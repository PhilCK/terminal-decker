#ifndef DICT_HACK_INCLUDED_7033C6E6_AEAD_44B5_B228_4C74F45860CC
#define DICT_HACK_INCLUDED_7033C6E6_AEAD_44B5_B228_4C74F45860CC


#include <string>
#include <vector>
#include <stdint.h>


class dictionary_hack
{
public:

  explicit                dictionary_hack();
  
  void                    hack();
  void                    help();
  void                    update();
  
private:

  enum class hack_state { pending, hacking, finished, };
  
  hack_state m_curr_hack_state = hack_state::pending;

}; // class


void on_connection(dictionary_hack &self);
void on_think(dictionary_hack &self, const float dt);
void on_input_str(dictionary_hack &self, const std::string &input);


#endif // include guard