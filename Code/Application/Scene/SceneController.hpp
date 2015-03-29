#ifndef SCENE_CONTROLLER_INCLUDED_6CB7E681_B740_4F0B_AED8_9A63A911AAD8
#define SCENE_CONTROLLER_INCLUDED_6CB7E681_B740_4F0B_AED8_9A63A911AAD8


#include <Caffeine/Common/Platform.hpp>
#include <Caffeine/Common/Utilities/NonCopyable.hpp>
#include <Application/Scene/SceneFwd.hpp>


class SceneController : private CaffUtil::NonCopyable
{
public:

  explicit          SceneController(SceneModel &model);

  void              update(const float dt);

private:

  SceneModel        &m_model;

}; // class


#endif // include guard