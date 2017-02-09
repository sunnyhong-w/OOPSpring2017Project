/*  component.h: 本檔案儲存遊戲引擎所使用的component的interface和basic component的實作
Copyright (C) 2017 Guo Xiang, Hong <sunnyhong.w@gmail.com>

基本上是把Unity那種組件式引擎移植到現在的game framework上面讓未來能夠加速開發的邏輯。
請不要把自己的Script也一起塞到這一堆裡面，這邊只放所有人共有的基礎類別。

HISTORY :
2017-02-09 v0.1.0
*/

#include"component.h"
#include"gameobject.h"

namespace game_engine {
    Component::Component(GameObject* gobj)
    {
        this->gameObject = gobj;
        this->transform = gobj->GetComponent<Transform>();
    }
    Component::~Component()
    {}
}