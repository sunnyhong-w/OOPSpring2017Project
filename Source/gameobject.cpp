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
    GameObject::GameObject(bool doNotDestoryOnChangeScene, bool isPureScript)
    {
        this->doNOTDestoryOnChangeScene = doNOTDestoryOnChangeScene;
        if (!isPureScript)
        {
            this->transform = this->AddComponentOnce<Transform>();
            this->isPureScript = isPureScript;
        }
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

/////////////////////////////////////////////////////////////////////////
//  Game Object Management
/////////////////////////////////////////////////////////////////////////

    vector<GameObject*> GameObject::gameObjects;

    void Destory(GameObject &gobj)
    {
        gobj.destoryFlag = true;
    }

    void Instantiate(GameObject *objectPrefrabs, Vector2 posision)
    {
        GameObject* gobj = new GameObject(*objectPrefrabs);
        if (!posision.isNull())
            gobj->GetComponent<Transform>()->position = posision;
        
        GameObject::Insert(&gobj);
    }

    void GameObject::Insert(GameObject **objptr)
    {
        //Magic : Do an instertion sort with binary search
        int high = GameObject::gameObjects.size() - 1, low = 0, mid = 0;

        //Do an binary search so we know where should the object be
        while (low <= high)
        {
            mid = (high + low) / 2;
            if (GameObject::gameObjects[mid]->transform->GetZCode() > (*objptr)->transform->GetZCode())
                high = mid - 1;
            else if (GameObject::gameObjects[mid]->transform->GetZCode() < (*objptr)->transform->GetZCode())
                low = mid + 1;
            else
                break;
        }

        GameObject::gameObjects.insert(GameObject::gameObjects.begin() + mid, (*objptr));
    }

    void GameObject::ResetObjectPool()
    {
        vector<GameObject*>::iterator it;
        for (it = GameObject::gameObjects.begin(); it != GameObject::gameObjects.end(); )
        {
            if (!(*it)->doNOTDestoryOnChangeScene)
            {
                delete (*it);
                it = GameObject::gameObjects.erase(it);
            }
            else
                it++;
        }
    }

    void GameObject::UpdateRenderOrder(GameObject *gobj)
    {
        //Do an binary search so we know where could the object be, 
        //which can save a lot of time when size is large.
        //when we found the index, go front and fo backward unitl zcode is different
        int high = GameObject::gameObjects.size() - 1, low = 0, mid = 0;
        while (low <= high)
        {
            mid = (high + low) / 2;
            if (GameObject::gameObjects[mid]->transform->GetZCode() > gobj->transform->GetZCode())
                high = mid - 1;
            else if (GameObject::gameObjects[mid]->transform->GetZCode() < gobj->transform->GetZCode())
                low = mid + 1;
            else
                break;
        }

        int zcode = GameObject::gameObjects[mid]->transform->GetZCode();
        //backward
        for (int i = mid; i < (int)GameObject::gameObjects.size() && GameObject::gameObjects[i]->transform->GetZCode() == zcode; i++)
        {
            if (GameObject::gameObjects[i] == gobj)
            {
                GameObject::gameObjects.erase(GameObject::gameObjects.begin() + i);
                GameObject::Insert(&gobj);
                return;
            }
        }

        //foward
        for (int i = mid; i >= 0 && GameObject::gameObjects[i]->transform->GetZCode() == zcode; i--)
        {
            if (GameObject::gameObjects[i] == gobj)
            {
                GameObject::gameObjects.erase(GameObject::gameObjects.begin() + i);
                GameObject::Insert(&gobj);
                return;
            }
        }
    }
}

