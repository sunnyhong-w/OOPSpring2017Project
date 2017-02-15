/*  gameobject.cpp: 本檔案儲存遊戲引擎所使用的GameObject的interface的實作
Copyright (C) 2017 Guo Xiang, Hong <sunnyhong.w@gmail.com>

基本上是把Unity的Gmae Object移植到現在的game framework上面讓未來能夠加速開發的邏輯。
請不要把自己的Script也一起塞到這一堆裡面，這邊只放所有人共有的基礎類別。

HISTORY :
2017-02-09 v0.1.0
*/

#include"StdAfx.h"
#include <string>
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
    for (ComponentData::iterator it = componentData.begin(); it != componentData.end(); it++)
        delete it->second;
    
}

void GameObject::Start()
{
    for (ComponentData::iterator it = componentData.begin(); it != componentData.end(); it++)
    {
        if (it->second->isBehavior())
        {
            it->second->enable = true;
            static_cast<GameBehaviour*>(it->second)->Start();
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
    if (this->GetComponent<SpriteRenderer>() != nullptr)
        this->GetComponent<SpriteRenderer>()->Draw();
}

void GameObject::OnRecivedBoardcast(int ev, string from, string text, Vector2I point, Vector2I size)
{
    for (ComponentData::iterator it = componentData.begin(); it != componentData.end(); it++)
    {
        if (it->second->isBehavior())
        {
            GameBehaviour* gb = static_cast<GameBehaviour*>(it->second);

            if (gb->enable)
                gb->OnRecivedBoardcast(ev, from, text, point, size);
        }
    }
}

void GameObject::SetName(string name)
{
    GameObject::objectsName.erase(this->name);
    this->name = name;
    GameObject::UpdateName(this);
}

string GameObject::GetName()
{
    return this->name;
}

void GameObject::SetTag(Tag tag)
{
    typedef multimap<Tag, GameObject*>::iterator iter;
    std::pair<iter, iter> data = GameObject::objectsTag.equal_range(this->tag);
    for (iter it = data.first; it != data.second; it++)
    {
        if (it->second == this)
        {
            GameObject::objectsTag.erase(it);
            this->tag = tag;
            GameObject::UpdateTag(this);
            break;
        }
    }
}

void GameObject::SetLayer(Layer layer)
{
    typedef multimap<Layer, GameObject*>::iterator iter;
    std::pair<iter, iter> data = GameObject::objectsLayer.equal_range(this->layer);
    for (iter it = data.first; it != data.second; it++)
    {
        if (it->second == this)
        {
            GameObject::objectsLayer.erase(it);
            this->layer = layer;
            GameObject::UpdateLayer(this);
            break;
        }
    }
}

/////////////////////////////////////////////////////////////////////////
//  Game Object Management
/////////////////////////////////////////////////////////////////////////

vector<GameObject*> GameObject::gameObjects;
map<string, GameObject*> GameObject::prefrabsData;
map<string, GameObject*> GameObject::objectsName;
multimap<Tag, GameObject*> GameObject::objectsTag;
multimap<Layer, GameObject*> GameObject::objectsLayer;

void Destory(GameObject& gobj)
{
    gobj.destoryFlag = true;
}

void Instantiate(GameObject* objectPrefrabs, Vector2 posision)
{
    GameObject* gobj = new GameObject(*objectPrefrabs);

    if (!posision.isNull())
        gobj->GetComponent<Transform>()->position = posision;

    GameObject::Insert(gobj);
    GameObject::UpdateName(gobj);
    GameObject::UpdateTag(gobj);
    GameObject::UpdateLayer(gobj);
}

void GameObject::Insert(GameObject* gobj)
{
    //Magic : Do an instertion sort with binary search
    int high = GameObject::gameObjects.size() - 1, low = 0, mid = 0;

    //Do an binary search so we know where should the object be
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

    GameObject::gameObjects.insert(GameObject::gameObjects.begin() + mid, gobj);
}

void GameObject::UpdateName(GameObject *gobj)
{
    if (GameObject::objectsName.find(gobj->name) != GameObject::objectsName.end())
        GameObject::objectsName[gobj->name] = gobj;
    else
    {
        int i;
        for (i = 0; GameObject::objectsName.find(gobj->name + " Clone(" + to_string(i) + ")") != GameObject::objectsName.end(); i++);

        GameObject::objectsName[gobj->name + " Clone(" + to_string(i) + ")"] = gobj;
        gobj->name = gobj->name + " Clone(" + to_string(i) + ")";
    }
}

void GameObject::UpdateTag(GameObject * gobj)
{
    GameObject::objectsTag.insert(multimap<Tag, GameObject*>::value_type(gobj->tag, gobj));
}

void GameObject::UpdateLayer(GameObject * gobj)
{
    GameObject::objectsLayer.insert(multimap<Layer, GameObject*>::value_type(gobj->layer, gobj));
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

void GameObject::UpdateRenderOrder(GameObject* gobj)
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
            GameObject::Insert(gobj);
            return;
        }
    }

    //foward
    for (int i = mid; i >= 0 && GameObject::gameObjects[i]->transform->GetZCode() == zcode; i--)
    {
        if (GameObject::gameObjects[i] == gobj)
        {
            GameObject::gameObjects.erase(GameObject::gameObjects.begin() + i);
            GameObject::Insert(gobj);
            return;
        }
    }
}

GameObject* GameObject::findGameObjectByName(string name)
{
    return GameObject::objectsName[name];
}

vector<GameObject*> GameObject::findGameObjectsByTag(Tag tag)
{
    vector<GameObject*> retdat;
    typedef multimap<Tag, GameObject*>::iterator iter;
    std::pair<iter, iter> data = GameObject::objectsTag.equal_range(tag);
    for (iter it = data.first; it != data.second; it++)
        retdat.push_back(it->second);
    return retdat;
}

vector<GameObject*> GameObject::findGameObjectsByLayer(Layer layer)
{
    vector<GameObject*> retdat;
    typedef multimap<Layer, GameObject*>::iterator iter;
    std::pair<iter, iter> data = GameObject::objectsLayer.equal_range(layer);
    for (iter it = data.first; it != data.second; it++)
        retdat.push_back(it->second);
    return retdat;
}

GameObject * GameObject::GetPrefrabs(std::string file)
{
    if (GameObject::prefrabsData.find(file) == GameObject::prefrabsData.end())
        return nullptr;
    else
        return GameObject::prefrabsData[file];
}

GameObject* GameObject::InsertPrefrabs(string file, GameObject *gobj)
{
    GAME_ASSERT(GameObject::prefrabsData.find(file) != GameObject::prefrabsData.end(), 
        ("Prefrab name repeated. #[Engine]GameObject::InsertPrefrabs (PrefrabName : " + file + ")").c_str());

    GameObject::prefrabsData[file] = gobj;
    return gobj;
}

}

