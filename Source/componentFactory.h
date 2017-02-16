#pragma once

#include<map>
#include<typeinfo>
#include"component.h"

using namespace std;

namespace game_engine
{
class ComponentFactory
{
    public:
        typedef Component* (*constructor_t)(GameObject* gobj);
        typedef map<string, constructor_t> map_type;

        template<class T> static void RegClass(string className);
        static Component* construct(string className, GameObject* gobj);

    private:
        template<class T> static Component* Construct(GameObject* gobj);
        static map_type classConstructor;
};

template<class T>
inline Component* ComponentFactory::Construct(GameObject* gobj)
{
    return new T(gobj);
}

template<class T>
inline void ComponentFactory::RegClass(string className)
{
    ComponentFactory::classConstructor[className] = &ComponentFactory::classConstructor<T>;
}

#ifndef REGISTER_CLASS
#define REGISTER_CLASS(n) (ComponentFactory::RegClass<n>(#n));
#endif // REGISTER_CLASS



}

