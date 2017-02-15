/*  gameobject.h: 本檔案儲存遊戲引擎所使用的GameObject的interface，以及GmaeObject中Template的實作
Copyright (C) 2017 Guo Xiang, Hong <sunnyhong.w@gmail.com>

基本上是把Unity的Gmae Object移植到現在的game framework上面讓未來能夠加速開發的邏輯。
請不要把自己的Script也一起塞到這一堆裡面，這邊只放所有人共有的基礎類別。

HISTORY :
2017-02-09 v0.1.0
*/

#pragma once

#include<map>
#include<vector>
#include<typeindex>
#include"scene.h"
#include"_setting.h"

namespace game_engine
{

class Component;
class Transform;
class GameObject;

//一些Public的最高權限Function
void Destory(GameObject& gobj);
void Instantiate(GameObject* objectPrefrabs, Vector2 posision = Vector2::null);

class GameObject
{
        friend void Destory(GameObject& gobj);
        friend void Instantiate(GameObject* objectPrefrabs, Vector2 posision);
        friend class GameScene;
        friend class Transform;
        friend class game_framework::CGame;
    public:
        GameObject(bool doNotDestoryOnChangeScene = false, bool isPureScript = false);
        ~GameObject();
        void Start();
        void Update();
        void LateUpdate();
        void Draw();
        void OnRecivedBoardcast(int ev, string from, string text, Vector2I point, Vector2I size);
        void SetName(string name);
        string GetName();
        void SetTag(Tag tag);
        void SetLayer(Layer layer);

        bool enable = false;
        Transform* transform;

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
        template<class T> const std::vector<T*> GetComponents();
        //刪除指定型別的Component物件
        //T component的泛型
        template<class T> void RemoveComponent(T* comp);
        //刪除所有指定型別的Component
        //T component的泛型
        template<class T> void RemoveComponents();

        //GameObject Object Manage
        static GameObject* InsertPrefrabs(std::string file, GameObject* gobj);
        static GameObject* findGameObjectByName(string name);
        static vector<GameObject*> findGameObjectsByTag(Tag tag);
        static vector<GameObject*> findGameObjectsByLayer(Layer layer);

        static vector<GameObject*> gameObjects;

    private:
        //GameObjectManagement
        static void Insert(GameObject* gobj);
        static void UpdateName(GameObject* gobj);
        static void UpdateTag(GameObject* gobj);
        static void UpdateLayer(GameObject* gobj);
        static void ResetObjectPool();
        static void UpdateRenderOrder(GameObject* gobj);
        static std::map<std::string, GameObject*> prefrabsData;
        static std::map<std::string, GameObject*> objectsName;
        static std::multimap<Tag, GameObject*> objectsTag;
        static std::multimap<Layer, GameObject*> objectsLayer;

        //GameObject
        typedef std::multimap<std::type_index, Component*> ComponentData;
        ComponentData componentData;
        std::string name;
        Tag tag;
        Layer layer;
        bool destoryFlag = false;
        bool isPureScript = false;
        bool doNOTDestoryOnChangeScene = false;
        
};

//由於Template分離的話編譯器會找不到進入點，所以必須將Template的實作在gameobject.h中
//Template Declare

template<class T> inline T* GameObject::AddComponent()
{
    T* TPointer = new T(this);
    componentData.insert(ComponentData::value_type(typeid(T), TPointer));
    return TPointer;
}

template<class T> inline T* GameObject::AddComponentOnce()
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

template<class T> inline T* GameObject::GetComponent()
{
    ComponentData::iterator iter = componentData.find(typeid(T));

    if (iter != componentData.end())
        return static_cast<T*>(iter->second);
    else
        return nullptr;
}

template<class T> inline const std::vector<T*> GameObject::GetComponents()
{
    std::pair<ComponentData::iterator, ComponentData::iterator> data = componentData.equal_range(typeid(T));
    std::vector<T*> retval;

    for (ComponentData::iterator it = data.first; it != data.second; it++)
        retval.push_back(static_cast<T*>(it->second));

    return retval;
}

template<class T> inline void GameObject::RemoveComponent(T* comp)
{
    std::pair<ComponentData::iterator, ComponentData::iterator> data = componentData.equal_range(typeid(T));

    for (ComponentData::iterator it = data.first; it != data.second; it++)
    {
        if (it->second == comp)
        {
            delete comp;
            componentData.erase(it);
            break;
        }
    }
}

template<class T> inline void GameObject::RemoveComponents()
{
    auto list = GetComponents<T>();

    for (T* tempcomp : list)
        delete tempcomp;

    componentData.erase(typeid(T));
}

}