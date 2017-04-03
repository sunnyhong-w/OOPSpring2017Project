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
        GameBehaviour(GameObject* gobj);
        ~GameBehaviour();

        virtual void ParseJSON(json j) {};

        virtual void Start() {};
        virtual void Update() {};
        virtual void LateUpdate() {};

		virtual void Draw(Vector2I v2 = Vector2I::zero) {};

        virtual void OnCollisionEnter(Collider* c) {};
        virtual void OnCollisionExit (Collider* c) {};
        virtual void OnCollisionStay (Collider* c) {};

        virtual void OnRecivedBoardcast(BoardcastMessageData bmd) {};
		virtual void OnDrawGizmos(CDC *pDC) {};

		map<BoardcastEvent, bool> eventListener;
};
}