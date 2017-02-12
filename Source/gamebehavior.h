/*  gameobject.h: 本檔案儲存遊戲引擎所使用的GameObjectScript的基礎
Copyright (C) 2017 Guo Xiang, Hong <sunnyhong.w@gmail.com>

基本上是把Unity的Gmae Object移植到現在的game framework上面讓未來能夠加速開發的邏輯。
請不要把自己的Script也一起塞到這一堆裡面，這邊只放所有人共有的基礎類別。
把這邊這個當成Unity的MonoBehavior來用就對啦~

HISTORY :
2017-02-09 v0.1.0
*/

#pragma once
#include"component.h"

namespace game_engine
{
class GameBehaviour : public Component
{

    public:
        GameBehaviour(GameObject* gobj) : Component(gobj, true) {};

        virtual void Start() {};
        virtual void Update() {};
        virtual void LateUpdate() {};

        virtual void OnCollisionEnter(Collision* c) {};
        virtual void OnCollisionExit(Collision* c) {};
        virtual void OnCollisionStay(Collision* c) {};

        virtual void OnRecivedBoardcast(int ev, string from, string text, Vector2I point, Vector2I size) {};
};
}