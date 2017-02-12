#include"StdAfx.h"
#include<ddraw.h>
#include"gameobject.h"
#include"scene.h"

namespace game_engine
{

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

void GameScene::OnMove()
{
    //INPUT WORKOUT HERE

    //Destory Dectechtion
    for(vector<GameObject*>::iterator it = GameObject::gameObjects.begin(); it != GameObject::gameObjects.end(); )
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
        if(gobj->enable)
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


void GameScene::OnCopyData(game_framework::TransferData *TDP)
{
    TDPQueue.push_back(*TDP);
}

}


