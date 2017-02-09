/*  gameobject.cpp: 本檔案儲存遊戲引擎所使用的GameObject的interface的實作
Copyright (C) 2017 Guo Xiang, Hong <sunnyhong.w@gmail.com>

基本上是把Unity的Gmae Object移植到現在的game framework上面讓未來能夠加速開發的邏輯。
請不要把自己的Script也一起塞到這一堆裡面，這邊只放所有人共有的基礎類別。

HISTORY :
2017-02-09 v0.1.0
*/

#include"gameobject.h"

namespace game_engine
{
    template<class T>
    T* GameObject::AddComponent()
    {
        T* TPointer = new T(this);
        componentData.insert(ComponentData::value_type(typeid(T), TPointer));
        return TPointer;
    }
    
    template<class T>
    T* GameObject::AddComponentOnce()
    {
        ComponentData::iterator iter = componentData.find(typeid(T));
        if (iter == componentData.end())
        {
            T* TPointer = new T(this);
            componentData.insert(ComponentData::value_type(typeid(T), TPointer));
            return TPointer;
        }
        else
            return GetComponent<T>();
    }

    template<class T>
    T* GameObject::GetComponent()
    {
        ComponentData::iterator iter = componentData.find(typeid(T));
        if (iter != componentData.end())
            return iter->second;
        else
            return nullptr;
    }

    template<class T>
    const auto GameObject::GetComponents()
    {
        return componentData.equal_range(typeid(T));
    }

    GameObject::~GameObject()
    {
    }

}

