#include"StdAfx.h"
#include<ddraw.h>
#include"gameobject.h"
#include"scene.h"
#include"input.h"

namespace game_engine
{
GameScene::GameScene(game_framework::CGame* CG): game_framework::CGameState(CG)
{
}
void GameScene::OnInit()
{
    CreateObjectPrefrabs();
}

void GameScene::OnBeginState()
{
    GameObject::ResetObjectPool();
    InstantiateGameObject();

    for (GameObject* gobj : GameObject::gameObjects)
        gobj->Start();
}

void GameScene::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    if (Input::keyEvent.find(nChar) == Input::keyEvent.end())
    {
        Input::keyEvent[nChar] = game_framework::CSpecialEffect::GetEllipseTime();
    }
}

void GameScene::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    Input::keyEvent.erase(nChar);
}

void GameScene::OnLButtonDown(UINT nFlags, CPoint point)
{
}

void GameScene::OnLButtonUp(UINT nFlags, CPoint point)
{
}

void GameScene::OnMouseMove(UINT nFlags, CPoint point)
{
}

void GameScene::OnRButtonDown(UINT nFlags, CPoint point)
{
}

void GameScene::OnRButtonUp(UINT nFlags, CPoint point)
{
}

void GameScene::OnMove()
{
    //INPUT WORKOUT HERE
    Input::Update();

    if (Input::GetKeyTrigger('A'))
    {
        AfxMessageBox("!");
    }

    //Destory Dectechtion
    for (vector<GameObject*>::iterator it = GameObject::gameObjects.begin(); it != GameObject::gameObjects.end(); )
    {
        if ((*it)->destoryFlag)
        {
            delete (*it);
            GameObject::gameObjects.erase(it);
        }
        else
            it++;
    }

    //COLLISION DECTECTION WORK OUT HERE

    if (TDPQueue.size() != 0)
    {
        for (GameObject* gobj : GameObject::gameObjects)
            if (gobj->enable)
                gobj->OnRecivedBoardcast(TDPQueue[0].ev, TDPQueue[0].sender, TDPQueue[0].message,
                                         Vector2I(TDPQueue[0].pos.x, TDPQueue[0].pos.y),
                                         Vector2I(TDPQueue[0].pos.w, TDPQueue[0].pos.h));

        TDPQueue.erase(TDPQueue.begin());
    }

    for (GameObject* gobj : GameObject::gameObjects)
        if (gobj->enable)
            gobj->Update();

    //ANIMATION UPDATE WORKOUT HERE

    for (GameObject* gobj : GameObject::gameObjects)
        if (gobj->enable)
            gobj->LateUpdate();
}

void GameScene::OnShow()
{
    for (GameObject* gobj : GameObject::gameObjects)
        gobj->Draw();
}


void GameScene::OnCopyData(game_framework::TransferData* TDP)
{
    TDPQueue.push_back(*TDP);
}

}


