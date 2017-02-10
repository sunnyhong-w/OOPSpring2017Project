/*  gameobject.cpp: 本檔案儲存遊戲引擎所使用的GameObject的interface的實作
Copyright (C) 2017 Guo Xiang, Hong <sunnyhong.w@gmail.com>

基本上是把Unity的Gmae Object移植到現在的game framework上面讓未來能夠加速開發的邏輯。
請不要把自己的Script也一起塞到這一堆裡面，這邊只放所有人共有的基礎類別。

HISTORY :
2017-02-09 v0.1.0
*/

#include"StdAfx.h"
#include <ddraw.h>
#include"component.h"
#include"gamebehavior.h"
#include"gameobject.h"

namespace game_engine
{
    GameObject::GameObject()
    {
        this->AddComponentOnce<Transform>();
    }

    GameObject::~GameObject()
    {
    }

    void GameObject::Start()
    {
        for (ComponentData::iterator it = componentData.begin(); it != componentData.end(); it++)
        {
            if (it->second->isBehavior())
            {
                static_cast<GameBehaviour*>(it->second)->Start();
                it->second->enable = true;
            }
        }
    }

    void GameObject::Update()
    {
        for (ComponentData::iterator it = componentData.begin(); it != componentData.end(); it++)
        {
            if (it->second->isBehavior())
            {
                GameBehaviour* gb = static_cast<GameBehaviour*>(it->second);
                if (gb->enable)
                    gb->Update();
            }
        }
    }

    void GameObject::LateUpdate()
    {
        for (ComponentData::iterator it = componentData.begin(); it != componentData.end(); it++)
        {
            if (it->second->isBehavior())
            {
                GameBehaviour* gb = static_cast<GameBehaviour*>(it->second);
                if (gb->enable)
                    gb->LateUpdate();
            }
        }
    }

    void GameObject::Draw()
    {
        if(this->GetComponent<SpriteRenderer>() != nullptr)
            this->GetComponent<SpriteRenderer>()->Draw();
    }

    void Destory(GameObject &gobj)
    {
        gobj.destoryFlag = true;
    }
}

