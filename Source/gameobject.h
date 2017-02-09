#pragma once

/*  gameobject.h: 本檔案儲存遊戲引擎所使用的GameObject的interface
Copyright (C) 2017 Guo Xiang, Hong <sunnyhong.w@gmail.com>

基本上是把Unity的Gmae Object移植到現在的game framework上面讓未來能夠加速開發的邏輯。
請不要把自己的Script也一起塞到這一堆裡面，這邊只放所有人共有的基礎類別。

HISTORY :
2017-02-09 v0.1.0
*/

#include<map>
#include<typeindex>

namespace game_engine {

class Component;
class GameScript;
class GameObject;

void Destory(GameObject &gobj)
{
    gobj.destoryFlag = true;
}

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

}