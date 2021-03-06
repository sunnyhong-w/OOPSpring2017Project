#include"StdAfx.h"
#include<ddraw.h>
#include"audio.h"
#include"gameobject.h"
#include"scene.h"
#include"component.h"
#include"input.h"
#include"Script\Engine\GameSetting.h"
#include<ctime>
#include<fstream>
#include<thread>

namespace game_engine
{

json GameScene::prefrabmap;

void GameScene::OnBeginState()
{
    GameObject::ResetObjectPool();
    //Before Loading Easing(Ex : Darken the whoile windows)
    thread th(&GameScene::LoadSceneData, this);

    //Render Loading Animation(force it OnDraw)
    while (!th.joinable())
        OnDraw();

    th.join();
    //After Loading Easing(Ex : Brighten the whole window)
    game_framework::CSpecialEffect::SetCurrentTime();

	GameStatus::LoadFile();
}

void GameScene::OnMove()
{
    while (loadname != "")
        OnBeginState();

	//FMOD Audio Update 
    AudioPlayer::instance->m_pSystem->update();

	//Time Update
    Time::Update();

    //INPUT WORKOUT HERE
    Input::Update();

    if (Input::GetKeyDown(VK_F1))
    {
        debug = !debug;
    }

    bool updateGameObjectIter = false;

    //Destroy Dectechtion
    for (vector<GameObject*>::iterator it = GameObject::gameObjects.begin(); it != GameObject::gameObjects.end(); )
    {
        if ((*it)->destoryFlag)
        {
            auto &gobjTagSet = GameObject::objectsTag[(*it)->tag];
            auto itTagSet = gobjTagSet.find((*it));
            if (itTagSet != gobjTagSet.end())
                gobjTagSet.erase(itTagSet);

            auto &gobjLayerSet = GameObject::objectsLayer[(*it)->layer];
            auto itLayerSet = gobjLayerSet.find((*it));
            if (itLayerSet != gobjLayerSet.end())
                gobjLayerSet.erase(itLayerSet);

            GameObject::objectsName.erase((*it)->name);

            delete (*it);
            it = GameObject::gameObjects.erase(it);

            updateGameObjectIter = true;
        }
        else
            ++it;
    }


    for (unsigned i = 0; i < GameObject::gameObjectsWaitingPools.size(); i++)
    {
        auto ptr = GameObject::gameObjectsWaitingPools[i];
        GameObject::Insert(ptr);
        ptr->Start();
        updateGameObjectIter = true;
    }
    GameObject::gameObjectsWaitingPools.clear();

    if (updateGameObjectIter)
    {
        gameobjectVectorBegin = GameObject::gameObjects.begin();
        gameobjectVectorEnd = GameObject::gameObjects.end();
    }

    //COLLISION DECTECTION WORK OUT HERE ----> BUT NO. I'm NOT GONNA DO THIS.

    for (auto it = gameobjectVectorBegin; it != gameobjectVectorEnd; ++it)
    {
        if ((*it)->enable)
        {
            Rigidbody* rigidbody = (*it)->rigidbody;
            if (rigidbody != nullptr && rigidbody->GetEnable())
                rigidbody->Update();
        }
    }

    for (auto it = gameobjectVectorBegin; it != gameobjectVectorEnd; ++it)
    {
        if ((*it)->enable)
        {
            Collider* collider = (*it)->collider;
            if (collider != nullptr && collider->GetEnable())
                collider->Update();
        }
    }

    //Windows File Transmission
    while (TDPQueue.size() != 0)
    {
        for (auto it = gameobjectVectorBegin; it != gameobjectVectorEnd; ++it)
            if ((*it)->enable)
                (*it)->OnRecivedBroadcast(TDPQueue[0]);

        TDPQueue.erase(TDPQueue.begin());
    }

    //GameBehavior Update Cycle
    for (auto it = gameobjectVectorBegin; it != gameobjectVectorEnd; ++it)
        if ((*it)->enable)
            (*it)->Update();

    //Animator Update Workout Here
    for (auto it = gameobjectVectorBegin; it != gameobjectVectorEnd; ++it)
    {
        if ((*it)->enable)
        {
            AnimationController* anic = (*it)->animationController;
            if (anic != nullptr && anic->GetEnable())
                anic->Update();
        }
    }

    //Animation Update
    for (auto it = gameobjectVectorBegin; it != gameobjectVectorEnd; ++it)
    {
        if ((*it)->enable)
        {
            Animation* ani = (*it)->animation;
            if (ani != nullptr && ani->GetEnable())
                ani->Update();
        }
    }

    //GameBehavior LateUpdate Cycle
    for (auto it = gameobjectVectorBegin; it != gameobjectVectorEnd; ++it)
        if ((*it)->enable)
            (*it)->LateUpdate();


    // Update Render Order
    for (auto gobj : GameObject::gameObjectRenderOrderUpdatePool)
        GameObject::UpdateRenderOrder(gobj);
    
    if (GameObject::gameObjectRenderOrderUpdatePool.size() != 0)
    {
        gameobjectVectorBegin = GameObject::gameObjects.begin();
        gameobjectVectorEnd = GameObject::gameObjects.end();
    }

    GameObject::gameObjectRenderOrderUpdatePool.clear();
}

void GameScene::OnShow()
{
    if (loadname == "")
    {
        for (auto it = gameobjectVectorBegin; it != gameobjectVectorEnd; ++it)
            if ((*it)->enable)
                (*it)->Draw(cameraPosition);

        //Draw Superve GUI thing after gameobject drawn

        //Draw Debug Info By CDC

        if (debug)
        {
            CDC *pDC = game_framework::CDDraw::GetBackCDC();
            pDC->SetBkMode(TRANSPARENT);
            for (auto it = gameobjectVectorBegin; it != gameobjectVectorEnd; ++it)
            {
                Collider* collider = (*it)->collider;
				SpriteRenderer* spriteRenderer = (*it)->spriteRenderer;
                if (collider != nullptr && collider->GetEnable() && (spriteRenderer != nullptr ? spriteRenderer->CameraTest(cameraPosition) : true))
                    collider->OnDrawGismos(pDC, cameraPosition);

                (*it)->OnDrawGizmos(pDC);

                //SpriteRenderer *SR = gobj->GetComponent<SpriteRenderer>();
                //if (SR != nullptr)
                //{
                //    CPen pen;
                //    pen.CreatePen(0, 1, RGB(255,0,0));
                //    pDC->SelectObject(pen);
                //
                //    Vector2I pos = (SR->GetRealRenderPostion() - cameraPosition);
                //    int r = 2;
                //    pDC->Ellipse(pos.x - r, pos.y - r, pos.x + r, pos.y + r);
                //}
            }

            Vector2 vw = Input::GetMouseWorldPos();
            Vector2I vc = Input::GetMousePos();

            //pDC->TextOutA(vc.x, vc.y - 20, CString(vw.toString().c_str()));
            //pDC->TextOutA(vc.x, vc.y + 20, CString(vc.GetV2().toString().c_str()));
            pDC->TextOutA(0, 0, CString(("Camera Pos : " + cameraPosition.GetV2().toString()).c_str()));

            game_framework::CDDraw::ReleaseBackCDC();
        }
    }
}

#define FindJSON(str) j.find(str) != j.end()

void GameScene::ParseJSON(json j)
{
    if (FindJSON("LoadScene"))
    {
        json SceneLoadData = j["LoadScene"];
        this->loadname = SceneLoadData[GameSetting::WindowName].get<string>();
        return;
    }

    if (FindJSON("IncludePrefrab"))
        IncludePrefrabs(filename + ".scene", j["IncludePrefrab"]);
    
    if (FindJSON("GameObject"))
        InstantiateGameObject(filename + ".scene", j["GameObject"]);

	AudioPlayer::instance->ReleaseBuffer();

	if (FindJSON("Sound"))
		AudioPlayer::instance->ParseSoundJSON(j["Sound"]);

	if (FindJSON("Music"))
		AudioPlayer::instance->ParseMusicJSON(j["Music"]);

	if (FindJSON("BackgroundColor"))
		bgcolor = RGB(j["BackgroundColor"]["r"], j["BackgroundColor"]["g"], j["BackgroundColor"]["b"]);
	else
		bgcolor = RGB(23, 23, 23);
}

void GameScene::IncludePrefrabs(string filename, json prefrabObject)
{
    prefrabmap[filename] = prefrabObject;

    for (json::iterator it = prefrabObject.begin(); it != prefrabObject.end(); ++it)
        ReadPrefrab(filename, it.value());
}

void GameScene::ReadPrefrab(string filename, string includename)
{
    if (GameObject::GetPrefrabs(includename) != nullptr)
        return;

    string PATH = R"(.\Assest\Prefrab\)";
    ifstream file;
    file.open(PATH + includename + ".prefrab");

    if (file.good())
    {
        stringstream buffer;
        buffer << file.rdbuf();
        json jsonobj = json::parse(buffer);
        file.close();
        GameObject::InsertPrefrabs(includename, jsonobj);

        if (jsonobj.find("IncludePrefrab") != jsonobj.end())
            IncludePrefrabs(includename, jsonobj["IncludePrefrab"]);
    }
    else
    {
        file.close();
        string str = "ERROR : Prefrab NOT FOUND when parse JSON :\n";
        str += "File : " + filename + "\n";
        str += "Prefrab : " + includename + "NOT FOUND";
        GAME_ASSERT(false, str.c_str());
    }
}

vector<GameObject*> GameScene::InstantiateGameObject(string filename, json objArray)
{
    vector<GameObject*> objectList;
    for (json jsonobj : objArray)
        objectList.push_back(CreateGameObject(filename, jsonobj));
    return objectList;
}

GameObject* GameScene::CreateGameObject(string filename, json jsonobj)
{
    GameObject* gobj;

    json prefrab = nullptr;

    if (jsonobj.find("include") != jsonobj.end())
    {
        json filemap = prefrabmap[filename];
        string prefrabcode = jsonobj["include"].get<string>();
        prefrab = GameObject::GetPrefrabs(filemap[prefrabcode]);
    }

    if (prefrab != nullptr)
    {
        string prefrabcode = jsonobj["include"].get<string>();
        gobj = InstantiateJSON(prefrab);

        if (prefrab.find("Child") != prefrab.end())
        {
            auto childList = InstantiateGameObject(prefrabcode, prefrab["Child"]);

            for (auto childobj : childList)
                childobj->transform->SetParent(gobj->transform);
        }
    }
    else
    {
        bool doNOTDestoryOnChangeScene = jsonobj.find("doNOTDestoryOnChangeScene") != jsonobj.end() ? jsonobj["doNOTDestoryOnChangeScene"] : false;
        gobj = Instantiate(new GameObject(doNOTDestoryOnChangeScene));
    }

    gobj->ParseJSON(jsonobj);

    if (jsonobj.find("Child") != jsonobj.end())
    {
        auto childList = InstantiateGameObject(filename, jsonobj["Child"]);
        for (auto childobj : childList)
            childobj->transform->SetParent(gobj->transform);
    }

    return gobj;
}


void GameScene::LoadScene(string filename)
{
    this->loadname = filename;
}

void GameScene::LoadSceneData()
{
    this->filename = loadname;
    this->loadname = "";

    string PATH = R"(.\Assest\Scene\)";
    ifstream file;
    file.open(PATH + filename + ".scene");

    if (file.good())
    {
        stringstream buffer;
        buffer << file.rdbuf();
        file.close();
        json jsonobj = json::parse(buffer);
        this->ParseJSON(jsonobj);
    }
    else
    {
        file.close();
        string str = "ERROR : Scene NOT FOUND when LoadScene!\n";
        str += "Scene : " + filename;
        GAME_ASSERT(false, str.c_str());
    }
}


Vector2I & GameScene::CameraPosition()
{
    return GameScene::NowScene()->cameraPosition;
}

GameScene * GameScene::NowScene()
{
    return ((GameScene*)game_framework::CGame::Instance()->GetState());
}

string GameScene::GetNowScene()
{
	return GameScene::NowScene()->filename;
}

Vector2I & GameScene::WindowPosition()
{
	return game_framework::CGame::Instance()->windowPosition;
}

void GameScene::Broadcast(BroadcastEvent event, json data, string windowName)
{
	BroadcastMessageData bmd;
	bmd.event = event;
	bmd.data = data;
	game_framework::CGame::Instance()->BroadcastMessage(bmd, windowName);
}

void GameScene::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    if (Input::keyEvent.find(nChar) == Input::keyEvent.end())
    {
        Input::keyEvent[nChar] = clock();
    }
}

void GameScene::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    Input::keyEvent.erase(nChar);
}

void GameScene::OnLButtonDown(UINT nFlags, CPoint point)
{
    if (Input::keyEvent.find(VK_LBUTTON) == Input::keyEvent.end())
    {
        Input::keyEvent[VK_LBUTTON] = clock();
    }
}

void GameScene::OnLButtonUp(UINT nFlags, CPoint point)
{
    Input::keyEvent.erase(VK_LBUTTON);
}

void GameScene::OnMouseMove(UINT nFlags, CPoint point)
{
    Input::mousePos = Vector2I(point.x, point.y);
}

void GameScene::OnRButtonDown(UINT nFlags, CPoint point)
{
    if (Input::keyEvent.find(VK_RBUTTON) == Input::keyEvent.end())
    {
        Input::keyEvent[VK_RBUTTON] = clock();
    }
}

void GameScene::OnRButtonUp(UINT nFlags, CPoint point)
{
    Input::keyEvent.erase(VK_RBUTTON);
}

void GameScene::OnCopyData(json j)
{
    TDPQueue.push_back(j);
}

}


