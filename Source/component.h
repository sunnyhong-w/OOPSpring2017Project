/*  component.h: 本檔案儲存遊戲引擎所使用的component的interface和basic component
Copyright (C) 2017 Guo Xiang, Hong <sunnyhong.w@gmail.com>

基本上是把Unity那種組件式引擎移植到現在的game framework上面讓未來能夠加速開發的邏輯。
請不要把自己的Script也一起塞到這一堆裡面，這邊只放所有人共有的基礎類別。

HISTORY :
2017-02-09 v0.1.0
*/

#pragma once
#include"enginelib.h"

namespace game_engine {

class GameObject;
class Transform;

class Component{
public:
    Component(GameObject* gobj, bool skip = false) : skipTriverse(skip) {};
    ~Component() {};
    virtual void Update() = 0;
    virtual void OnDestory() = 0;
    ///<summary>獲得skipTriverse的資料，確認這個Component能不能被Skip</summary>
    bool Skip();
protected:
    GameObject* gameObject;
    Transform* transform;
private:
    ///<summary>在Scene處理Object Component Triverse的時候，跳過這個Component</summary>
    const bool skipTriverse = false;
};



class Transform : public Component {
public:
    Transform(GameObject* gobj, Vector2 pos = Vector2::zero, int z = 0);
    ~Transform() {};
    Vector2 position;
    int zindex;
};

}