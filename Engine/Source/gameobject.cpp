/*  gameobject.cpp: 本檔案儲存遊戲引擎所使用的GameObject的interface的實作
Copyright (C) 2017 Guo Xiang, Hong <sunnyhong.w@gmail.com>

基本上是把Unity的Gmae Object移植到現在的game framework上面讓未來能夠加速開發的邏輯。
請不要把自己的Script也一起塞到這一堆裡面，這邊只放所有人共有的基礎類別。

HISTORY :
2017-02-09 v0.1.0
*/
#include "StdAfx.h"
#include <string>
#include <ddraw.h>
#include "component.h"
#include "gamebehavior.h"
#include "gameobject.h"
#include "_GameScripts.h"

namespace game_engine
{

///// Add Component String Parser

//Use define to make sure class won't have spelling mistake
#define RegisterComponent(v) else if (ComponentName == #v) return this->AddComponentOnce<v>();

Component* GameObject::AddComponentOnce(string ComponentName)
{
    if (ComponentName == "Component") return nullptr;

    //Register Basic Component
    RegisterComponent(Transform)
    RegisterComponent(SpriteRenderer)
    RegisterComponent(Collider)
	RegisterComponent(Rigidbody)
    RegisterComponent(Animation)
	RegisterComponent(AnimationController)
    //Register Script
    RegisterComponent(Tutorial)
    RegisterComponent(TextStamp)
    RegisterComponent(TextRenderer)
	RegisterComponent(MapReader)
	RegisterComponent(Slide)
    RegisterComponent(BoxParent)  
    RegisterComponent(Player)
    RegisterComponent(SpawnPlayer)
	RegisterComponent(Spring)
	RegisterComponent(Button)
    else return nullptr;
}

/////

#define FindJSON(str) j.find(str) != j.end()

GameObject::GameObject(bool doNotDestoryOnChangeScene) : enable(true)
{
    this->doNOTDestoryOnChangeScene = doNOTDestoryOnChangeScene;
    this->transform = this->AddComponentOnce<Transform>();
	this->SetTag(Tag::Untagged);
	this->SetLayer(Layer::Default);
}

GameObject::~GameObject()
{
    for (auto it : componentData)
        delete it.second;
}

void GameObject::ParseJSON(json j, bool noUpdateObjectPool)
{
    this->doNOTUpdateObjectPool = noUpdateObjectPool;

    if (FindJSON("Component"))
    {
        for (json::iterator it = j["Component"].begin(); it != j["Component"].end(); ++it)
        {
            Component* comp = this->AddComponentOnce(it.key());
			GAME_ASSERT(comp != nullptr, ("Component : " + it.key() + " NOT Found\n\nCheck \"GameObject -> Component Register\" If You Forget To Regsiter.\nOr Maybe You Have A Simple Spelling Mistake.").c_str());
            comp->ParseJSON(it.value());
        }
    }

    if (FindJSON("enable"))
        this->enable = j["enable"];

    if (FindJSON("name"))
        this->SetName(j["name"].get<string>());

    if (FindJSON("tag"))
        this->SetTag(j["tag"]);

    if (FindJSON("layer"))
        this->SetLayer(j["layer"]);

    if (FindJSON("isGUI"))
        this->isGUI = j["isGUI"];

    if (FindJSON("renderByBehavior"))
        this->renderByBehavior = j["renderByBehavior"];
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

    isStarted = true;
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

void GameObject::Draw(Vector2I cameraPos)
{
    if (!this->renderByBehavior)
    {
        SpriteRenderer* SR = this->GetComponent<SpriteRenderer>();

        if (SR != nullptr && SR->enable)
        {
            if (this->isGUI)
                SR->Draw();
            else
                SR->Draw(cameraPos);
        }
    }
    else
    {
        for (auto comp : componentData)
        {
            if (comp.second->isBehavior())
            {
                GameBehaviour* gb = static_cast<GameBehaviour*>(comp.second);

                if (gb->enable)
                {
                    if (this->isGUI)
                        gb->Draw();
                    else
                        gb->Draw(cameraPos);
                }
            }
        }
    }
}

void GameObject::OnRecivedBoardcast(BoardcastMessageData bmd)
{
    for (ComponentData::iterator it = componentData.begin(); it != componentData.end(); it++)
    {
        if (it->second->isBehavior())
        {
            GameBehaviour* gb = static_cast<GameBehaviour*>(it->second);

            if (gb->enable && (gb->eventListener[BoardcastEvent::All] || gb->eventListener[bmd.event]))
                gb->OnRecivedBoardcast(bmd);
        }
    }
}

void GameObject::OnDrawGizmos(CDC * pDC)
{
	for (ComponentData::iterator it = componentData.begin(); it != componentData.end(); it++)
	{
		if (it->second->isBehavior())
		{
			GameBehaviour* gb = static_cast<GameBehaviour*>(it->second);

			if (gb->enable)
				gb->OnDrawGizmos(pDC);
		}
	}
}

void GameObject::SetName(string name)
{
    if (!this->doNOTUpdateObjectPool)
    {
        GameObject::objectsName.erase(this->name);
        this->name = name;
        GameObject::UpdateName(this);
    }
    else
        this->name = name;
}

string GameObject::GetName()
{
    return this->name;
}

void GameObject::SetTag(Tag tag)
{
    if (!this->doNOTUpdateObjectPool)
    {
        typedef multimap<Tag, GameObject*>::iterator iter;
        std::pair<iter, iter> data = GameObject::objectsTag.equal_range(this->tag);

		bool find = false;
        for (iter it = data.first; it != data.second; it++)
        {
            if (it->second == this)
            {
				find = true;
                GameObject::objectsTag.erase(it);
                this->tag = tag;
                GameObject::UpdateTag(this);
                break;
            }
        }
	
		if (!find)
		{
			this->tag = tag;
			GameObject::UpdateTag(this);
		}
    }
    else
        this->tag = tag;
}

void GameObject::SetLayer(Layer layer)
{
    if (!this->doNOTUpdateObjectPool)
    {
        typedef multimap<Layer, GameObject*>::iterator iter;
        std::pair<iter, iter> data = GameObject::objectsLayer.equal_range(this->layer);

		bool find = false;
        for (iter it = data.first; it != data.second; it++)
        {
            if (it->second == this)
            {
                GameObject::objectsLayer.erase(it);
                this->layer = layer;
                GameObject::UpdateLayer(this);
				find = true;
                break;
            }
        }
		if (!find)
		{
			this->layer = layer;
			GameObject::UpdateLayer(this);
		}
    }
    else
        this->layer = layer;
}

/////////////////////////////////////////////////////////////////////////
//  Game Object Management
/////////////////////////////////////////////////////////////////////////

vector<GameObject*> GameObject::gameObjects;
vector<GameObject*> GameObject::gameObjectsWaitingPools;
map<string, json> GameObject::prefrabsData;
map<string, GameObject*> GameObject::objectsName;
map<string, int> GameObject::objectsNameCount;
multimap<Tag, GameObject*> GameObject::objectsTag;
multimap<Layer, GameObject*> GameObject::objectsLayer;

void Destroy(GameObject& gobj)
{
    Destroy(&gobj);
}

void Destroy(GameObject* gobj)
{
    gobj->destoryFlag = true;
    gobj->transform->SetParent(nullptr);

    auto childList = gobj->transform->GetChild();
    for (auto childTransform : childList)
        Destroy(*(childTransform->gameObject));
}

GameObject* Instantiate(GameObject* gobj, Vector2 position)
{
    if (!position.isNull())
        gobj->GetComponent<Transform>()->SetPosition(position);

	GameObject::gameObjectsWaitingPools.push_back(gobj);
    GameObject::UpdateTag(gobj);
    GameObject::UpdateLayer(gobj);
    return gobj;
}

GameObject* InstantiateJSON(json jsonobj, Vector2 position)
{
    bool doNOTDestoryOnChangeScene = jsonobj.find("doNOTDestoryOnChangeScene") != jsonobj.end() ? jsonobj["doNOTDestoryOnChangeScene"] : false;
    GameObject* gobj = new GameObject(doNOTDestoryOnChangeScene);
    gobj->ParseJSON(jsonobj);

	gobj->SetLayer(gobj->layer);
	gobj->SetTag(gobj->tag);

    if (!position.isNull())
        gobj->GetComponent<Transform>()->SetPosition(position);

	GameObject::gameObjectsWaitingPools.push_back(gobj);
    return gobj;
}

void GameObject::Insert(GameObject* gobj)
{
    //Magic : Do an instertion sort with binary search
	int size = GameObject::gameObjects.size();
	int high = size - 1, low = 0, mid = 0;
    int gobjzindex = gobj->transform->GetWorldZIndex();
    int gobjsortinglayer = gobj->transform->GetSortingLayer();
    //Do an binary search so we know where should the object be
    while (low <= high)
    {
        mid = (high + low) / 2;

        if (GameObject::gameObjects[mid]->transform->GetWorldZIndex() > gobjzindex)
            high = mid - 1;
        else if (GameObject::gameObjects[mid]->transform->GetWorldZIndex() < gobjzindex)
            low = mid + 1;
        else
        {
            if(GameObject::gameObjects[mid]->transform->GetSortingLayer() > gobjsortinglayer)
                high = mid - 1;
            else if (GameObject::gameObjects[mid]->transform->GetSortingLayer() < gobjsortinglayer)
                low = mid + 1;
            else
                break;
        }
    }
    if (mid == GameObject::gameObjects.size())
        GameObject::gameObjects.push_back(gobj);
    else
        GameObject::gameObjects.insert(GameObject::gameObjects.begin() + mid + 1, gobj);
}

void GameObject::UpdateName(GameObject* gobj)
{
    if (GameObject::objectsNameCount.find(gobj->name) == GameObject::objectsNameCount.end())
    {
        GameObject::objectsName[gobj->name] = gobj;
        GameObject::objectsNameCount[gobj->name] = 0;
    }
    else
    {
        string newname = gobj->name + " Clone(" + to_string(GameObject::objectsNameCount[gobj->name]) + ")";
        GameObject::objectsName[newname] = gobj;
		GameObject::objectsNameCount[gobj->name]++;
        gobj->name = newname;
    }
}

void GameObject::UpdateTag(GameObject* gobj)
{
    GameObject::objectsTag.insert(multimap<Tag, GameObject*>::value_type(gobj->tag, gobj));
}

void GameObject::UpdateLayer(GameObject* gobj)
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
	if (GameObject::gameObjects.size() == 0)
		return;
	
	int size = GameObject::gameObjects.size();
    int high = size - 1, low = 0, mid = 0;

    int objzindex = gobj->transform->GetWorldZIndex();

    while (low <= high)
    {
        mid = (high + low) / 2;

        if (GameObject::gameObjects[mid]->transform->GetWorldZIndex() > objzindex)
            high = mid - 1;
        else if (GameObject::gameObjects[mid]->transform->GetWorldZIndex() < objzindex)
            low = mid + 1;
        else
            break;
    }

    //backward
	for (int i = mid; i < size; i++)
	{
		if (GameObject::gameObjects[i]->transform->GetWorldZIndex() == objzindex)
		{
			if (GameObject::gameObjects[i] == gobj)
			{
				GameObject::gameObjects.erase(GameObject::gameObjects.begin() + i);
				GameObject::Insert(gobj);
				return;
			}
		}
	}

    //foward
    for (int i = mid; i >= 0; i--)
    {
		if (GameObject::gameObjects[i]->transform->GetWorldZIndex() == objzindex)
		{
			if (GameObject::gameObjects[i] == gobj)
			{
				GameObject::gameObjects.erase(GameObject::gameObjects.begin() + i);
				GameObject::Insert(gobj);
				return;
			}
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

json GameObject::GetPrefrabs(std::string file)
{
    if (GameObject::prefrabsData.find(file) == GameObject::prefrabsData.end())
        return nullptr;
    else
        return GameObject::prefrabsData[file];
}

json GameObject::InsertPrefrabs(string file, json prefrabsJSON)
{
    if (GameObject::prefrabsData.find(file) != GameObject::prefrabsData.end())
        TRACE(("WARRING : Prefrab name repeated. #[Engine]GameObject::InsertPrefrabs (PrefrabName : " + file + ")").c_str());
    GameObject::prefrabsData[file] = prefrabsJSON;
    return prefrabsJSON;
}

}

