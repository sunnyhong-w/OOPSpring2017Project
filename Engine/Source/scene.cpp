#include"StdAfx.h"
#include<ddraw.h>
#include"gameobject.h"
#include"scene.h"
#include"component.h"
#include"input.h"
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
}

void GameScene::OnMove()
{
    while (loadname != "")
        OnBeginState();

    Time::Update();

    //INPUT WORKOUT HERE
    Input::Update();

    if (Input::GetKeyDown(VK_F1))
    {
        debug = !debug;
    }

    //Destroy Dectechtion
    for (vector<GameObject*>::iterator it = GameObject::gameObjects.begin(); it != GameObject::gameObjects.end(); )
    {
        if ((*it)->destoryFlag)
        {
            typedef multimap<Tag, GameObject*>::iterator iter_tag;
            std::pair<iter_tag, iter_tag> data_t = GameObject::objectsTag.equal_range((*it)->tag);

            for (iter_tag tit = data_t.first; tit != data_t.second; tit++)
            {
                if (tit->second == (*it))
                {
                    GameObject::objectsTag.erase(tit);
                    break;
                }
            }

            typedef multimap<Layer, GameObject*>::iterator iter_layer;
            std::pair<iter_layer, iter_layer> data_l = GameObject::objectsLayer.equal_range((*it)->layer);

            for (iter_layer tit = data_l.first; tit != data_l.second; tit++)
            {
                if (tit->second == (*it))
                {
                    GameObject::objectsLayer.erase(tit);
                    break;
                }
            }

            GameObject::objectsName.erase((*it)->name);

            delete (*it);
            it = GameObject::gameObjects.erase(it);
        }
        else
            it++;
    }


    for (unsigned i = 0; i < GameObject::gameObjectsWaitingPools.size(); i++)
    {
        auto ptr = GameObject::gameObjectsWaitingPools[i];
        GameObject::Insert(ptr);
        ptr->Start();
    }
    GameObject::gameObjectsWaitingPools.clear();

    //COLLISION DECTECTION WORK OUT HERE ----> BUT NO. I'm NOT GONNA DO THIS.

    for (GameObject* gobj : GameObject::gameObjects)
    {
        Rigidbody* rigidbody = gobj->GetComponent<Rigidbody>();
        if (rigidbody != nullptr && rigidbody->enable)
            rigidbody->Update();
    }

    //Windows File Transmission
    while (TDPQueue.size() != 0)
    {
        for (GameObject* gobj : GameObject::gameObjects)
            if (gobj->enable)
                gobj->OnRecivedBoardcast(TDPQueue[0]);

        TDPQueue.erase(TDPQueue.begin());
    }

    //GameBehavior Update Cycle
    for (GameObject* gobj : GameObject::gameObjects)
        if (gobj->enable)
            gobj->Update();

    //Animator Update Workout Here
    for (GameObject* gobj : GameObject::gameObjects)
    {
        AnimationController* anic = gobj->GetComponent<AnimationController>();
        if (anic != nullptr && anic->enable)
            anic->Update();
    }

    //Animation Update
    for (GameObject* gobj : GameObject::gameObjects)
    {
        Animation* ani = gobj->GetComponent<Animation>();
        if (ani != nullptr && ani->enable)
            ani->Update();
    }

    //GameBehavior LateUpdate Cycle
    for (GameObject* gobj : GameObject::gameObjects)
        if (gobj->enable)
            gobj->LateUpdate();

}

void GameScene::OnShow()
{
    if (loadname == "")
    {
        for (GameObject* gobj : GameObject::gameObjects)
            if (gobj->enable)
                gobj->Draw(cameraPosition);

        //Draw Superve GUI thing after gameobject drawn

        //Draw Debug Info By CDC

        if (debug)
        {
            CDC *pDC = game_framework::CDDraw::GetBackCDC();
            pDC->SetBkMode(TRANSPARENT);
            for (GameObject* gobj : GameObject::gameObjects)
            {
                Collider* collider = gobj->GetComponent<Collider>();
                if (collider != nullptr)
                    collider->OnDrawGismos(pDC, cameraPosition);

                gobj->OnDrawGizmos(pDC);

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
        string SceneName = j["LoadScene"].get<string>();
        if (SceneName != "")
            this->loadname = SceneName;

        return;
    }

    if (FindJSON("IncludePrefrab"))
        IncludePrefrabs(filename + ".scene", j["IncludePrefrab"]);
    
    if (FindJSON("GameObject"))
        InstantiateGameObject(filename + ".scene", j["GameObject"]);    
}

void GameScene::IncludePrefrabs(string filename, json prefrabObject)
{
    prefrabmap[filename] = prefrabObject;

    for (json::iterator it = prefrabObject.begin(); it != prefrabObject.end(); it++)
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

Vector2I & GameScene::WindowPosition()
{
	return game_framework::CGame::Instance()->windowPosition;
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


