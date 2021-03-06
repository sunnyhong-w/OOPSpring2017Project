
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

#define FindJSON(str) j.find(str) != j.end()

GameObject::GameObject(bool doNotDestoryOnChangeScene)
{
    this->doNOTDestoryOnChangeScene = doNOTDestoryOnChangeScene;
    this->spriteRenderer = nullptr;
    this->collider = nullptr;
    this->rigidbody = nullptr;
    this->animation = nullptr;
    this->animationController = nullptr;
    this->transform = this->AddComponentOnce<Transform>();
	this->SetTag(Tag::Untagged);
	this->SetLayer(Layer::Default);
    this->enableIndex = 0;
    this->privateEnable = true;
    this->enable = true;
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
        this->SetEnable(j["enable"]);

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
    for (auto it = gamebehaviorSetBegin; it != gamebehaviorSetEnd; ++it)
        if((*it)->enable)
            (*it)->Start();

    isStarted = true;
}

void GameObject::Update()
{
    for (auto it = gamebehaviorSetBegin; it != gamebehaviorSetEnd; ++it)
        if ((*it)->enable)
            (*it)->Update();
}

void GameObject::LateUpdate()
{
    for (auto it = gamebehaviorSetBegin; it != gamebehaviorSetEnd; ++it)
        if ((*it)->enable)
            (*it)->LateUpdate();
}

void GameObject::Draw(Vector2I cameraPos)
{
    if (!this->renderByBehavior)
    {
        SpriteRenderer* SR = this->spriteRenderer;

        if (SR != nullptr && SR->enable)
        {
            if (this->isGUI)
            {
                if (SR->CameraTest(Vector2I::zero))
                    SR->Draw();
            }
            else
            {
                if (SR->CameraTest(cameraPos))
                    SR->Draw(cameraPos);
            }
        }
    }
    else
    {
        for (auto it = gamebehaviorSetBegin; it != gamebehaviorSetEnd; ++it)
        {
            if ((*it)->enable)
            {
                if (this->isGUI)
                    (*it)->Draw();
                else
                    (*it)->Draw(cameraPos);
            }
        }
    }
}

void GameObject::OnRecivedBroadcast(BroadcastMessageData bmd)
{
    for (auto it = gamebehaviorSetBegin; it != gamebehaviorSetEnd; ++it)
    {
        if ((*it)->enable)
        {
            if ((*it)->eventListener[BroadcastEvent::All] || (*it)->eventListener[bmd.event])
                (*it)->OnRecivedBroadcast(bmd);
        }
    }
}

void GameObject::OnDrawGizmos(CDC * pDC)
{
    for (auto it = gamebehaviorSetBegin; it != gamebehaviorSetEnd; ++it)
        if ((*it)->enable)
            (*it)->OnDrawGizmos(pDC);
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

Tag GameObject::GetTag()
{
    return tag;
}

void GameObject::SetTag(Tag tag)
{
    if (!this->doNOTUpdateObjectPool)
    {
        auto &tagSet = GameObject::objectsTag[this->tag];
        auto it = tagSet.find(this);

        if (it != tagSet.end())
            tagSet.erase(it);

        this->tag = tag;
        GameObject::InsertTag(this);
    }
    else
        this->tag = tag;
}

Layer GameObject::GetLayer()
{
    return layer;
}

void GameObject::SetLayer(Layer layer)
{
    if (!this->doNOTUpdateObjectPool)
    {
        auto &layerSet = GameObject::objectsLayer[this->layer];
        auto it = layerSet.find(this);

        if (it != layerSet.end())
            layerSet.erase(it);

        this->layer = layer;
        GameObject::InsertLayer(this);
    }
    else
        this->layer = layer;
}

void GameObject::SetEnable(bool enable)
{
    if (privateEnable ^ enable)
    {
        privateEnable = enable;
        UpdateEnable();
    }
}

bool GameObject::GetEnable()
{
    return enable;
}

void GameObject::UpdateComponentPair()
{
    this->gamebehaviorSetBegin = gamebehaviorSet.begin();
    this->gamebehaviorSetEnd = gamebehaviorSet.end();
}

void GameObject::UpdateEnable()
{
    auto *parent = this->transform->GetParent();
    int enableIndex = (parent != nullptr ? parent->gameObject->enableIndex : 0);

    this->enableIndex = enableIndex + (this->privateEnable ? 0 : 1);
    enable = this->enableIndex == 0;

    if (enable)
    {
        for (auto it = gamebehaviorSetBegin; it != gamebehaviorSetEnd; ++it)
            if ((*it)->enable)
                (*it)->Awake();
    }
    else
    {
        for (auto it = gamebehaviorSetBegin; it != gamebehaviorSetEnd; ++it)
            if ((*it)->enable)
                (*it)->Sleep();
    }

    for (auto ctrans : this->transform->GetChild())
        ctrans->gameObject->UpdateEnable();
}

/////////////////////////////////////////////////////////////////////////
//  Game Object Management
/////////////////////////////////////////////////////////////////////////

vector<GameObject*> GameObject::gameObjects;
vector<GameObject*> GameObject::gameObjectsWaitingPools;
set<GameObject*> GameObject::gameObjectRenderOrderUpdatePool;
map<string, json> GameObject::prefrabsData;
map<string, GameObject*> GameObject::objectsName;
map<string, int> GameObject::objectsNameCount;
map<Tag, set<GameObject*>> GameObject::objectsTag;
map<Layer, set<GameObject*>> GameObject::objectsLayer;

void Destroy(GameObject& gobj)
{
    Destroy(&gobj);
}

void Destroy(GameObject* gobj)
{
    gobj->destoryFlag = true;
    gobj->transform->SetParent(nullptr);
	gobj->SetEnable(false);


    auto childList = gobj->transform->GetChild();
    for (auto childTransform : childList)
        Destroy(childTransform->gameObject);
}

GameObject* Instantiate(GameObject* gobj, Transform* parent, Vector2 position)
{
    if (!position.isNull())
        gobj->transform->SetPosition(position);

    if (parent != nullptr)
        gobj->transform->SetParent(parent);

	GameObject::gameObjectsWaitingPools.push_back(gobj);
    GameObject::InsertTag(gobj);
    GameObject::InsertLayer(gobj);
    return gobj;
}

GameObject* InstantiateJSON(json jsonobj, Transform* parent, Vector2 position)
{
    bool doNOTDestoryOnChangeScene = jsonobj.find("doNOTDestoryOnChangeScene") != jsonobj.end() ? jsonobj["doNOTDestoryOnChangeScene"] : false;
    GameObject* gobj = new GameObject(doNOTDestoryOnChangeScene);
    gobj->ParseJSON(jsonobj);

	gobj->SetLayer(gobj->layer);
	gobj->SetTag(gobj->tag);

    if (!position.isNull())
        gobj->transform->SetPosition(position);

    if (parent != nullptr)
        gobj->transform->SetParent(parent);

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

void GameObject::InsertTag(GameObject* gobj)
{
    GameObject::objectsTag[gobj->tag].insert(gobj);
}

void GameObject::InsertLayer(GameObject* gobj)
{
    GameObject::objectsLayer[gobj->layer].insert(gobj);
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
            ++it;
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

set<GameObject*>& GameObject::findGameObjectsByTag(Tag tag)
{
    return GameObject::objectsTag[tag];
}

set<GameObject*>& GameObject::findGameObjectsByLayer(Layer layer)
{
    return GameObject::objectsLayer[layer];
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

