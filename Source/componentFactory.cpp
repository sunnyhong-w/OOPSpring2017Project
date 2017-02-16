#include"StdAfx.h"
#include"componentFactory.h"

namespace game_engine
{
ComponentFactory::map_type ComponentFactory::classConstructor;

Component* ComponentFactory::construct(string className, GameObject* gobj)
{
    if (ComponentFactory::classConstructor.find(className) == ComponentFactory::classConstructor.end())
        return nullptr;
    else
        return ComponentFactory::classConstructor[className](gobj);
}
}