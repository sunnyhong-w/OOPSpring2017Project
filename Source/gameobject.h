/*  gameobject.h: 本檔案儲存遊戲引擎所使用的GameObject的interface，以及GmaeObject中Template的實作
Copyright (C) 2017 Guo Xiang, Hong <sunnyhong.w@gmail.com>

基本上是把Unity的Gmae Object移植到現在的game framework上面讓未來能夠加速開發的邏輯。
請不要把自己的Script也一起塞到這一堆裡面，這邊只放所有人共有的基礎類別。

HISTORY :
2017-02-09 v0.1.0
*/

#pragma once

#include<map>
#include<typeindex>

namespace game_engine {

    class Component;
    class GameScript;
    class GameObject;

    void Destory(GameObject &gobj);

    class GameObject {
        friend void Destory(GameObject &gobj);
    public:
        ~GameObject();
        virtual void Start() {};
        virtual void Update() {};
        virtual void LateUpdate() {};
        virtual void Draw() {};

        //處理Component的Template

        //加入指定型別的物件，如果成功加入，會回傳對應指標
        //T component的泛型
        template<class T> T* AddComponent();
        //加入限定一個的指定型別的物件，如果成功加入，會回傳對應指標，如果已經存在，則會回傳該指標
        //T component的泛型
        template<class T> T* AddComponentOnce();
        //回傳一個指定型別的指標
        //T component的泛型
        template<class T> T* GetComponent();
        //取得所有指定型別的Component，會回傳一個iterator pair，其first為start，second為end
        //要判斷已沒有就判斷first==second，有的話就是true
        //T component的泛型
        template<class T> const auto GetComponents();

    protected:
        bool isEnable = true;

    private:
        typedef std::multimap<std::type_index, Component*> ComponentData;
        ComponentData componentData;
        bool destoryFlag = false;
    };

    //由於Template分離的話編譯器會找不到進入點，所以必須將Template的實作在gameobject.h中
    //Template Declare

    template<class T> T* GameObject::AddComponent()
    {
        T* TPointer = new T(this);
        componentData.insert(ComponentData::value_type(typeid(T), TPointer));
        return TPointer;
    }

    template<class T> T* GameObject::AddComponentOnce()
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

    template<class T> T* GameObject::GetComponent()
    {
        ComponentData::iterator iter = componentData.find(typeid(T));
        if (iter != componentData.end())
            return static_cast<T*>(iter->second);
        else
            return nullptr;
    }

    template<class T> const auto GameObject::GetComponents()
    {
        return componentData.equal_range(typeid(T));
    }

    //一些Public的最高權限Object
    void Destory(GameObject &gobj);

}