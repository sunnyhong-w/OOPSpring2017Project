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

    for (GameObject* gobj : GameObject::gameObjects)
        if (!gobj->isStarted)
            gobj->Start();
}

void GameScene::OnMove()
{
    if (loadname != "")
        OnBeginState();
    else
    {
		Time::Update();

        //INPUT WORKOUT HERE
        Input::Update();

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

        //COLLISION DECTECTION WORK OUT HERE ----> BUT NO. I'm NOT GONNA DO THIS.

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
			if (anic != nullptr)
				anic->Update();
		}

        //Animation Update
        for (GameObject* gobj : GameObject::gameObjects)
        {
            Animation* ani = gobj->GetComponent<Animation>();
            if (ani != nullptr)
                ani->Update();
        }

        //GameBehavior LateUpdate Cycle
        for (GameObject* gobj : GameObject::gameObjects)
            if (gobj->enable)
                gobj->LateUpdate();
    }
}


void GameScene::OnShow()
{
    for (GameObject* gobj : GameObject::gameObjects)
        if (gobj->enable)
            gobj->Draw(cameraPosition);

    //Draw Superve GUI thing after gameobject drawn
}

#define FindJSON(str) j.find(str) != j.end()

void GameScene::ParseJSON(json j)
{
    if (FindJSON("IncludePrefrab"))
    {
        string PATH = R"(.\Assest\Prefrab\)";

        for (json::iterator it = j["IncludePrefrab"].begin(); it != j["IncludePrefrab"].end(); ++it)
        {
            ifstream file;
            file.open(PATH + (*it).get<string>() + ".prefrab");

            if (file.good())
            {
                stringstream buffer;
                buffer << file.rdbuf();
                json jsonobj = json::parse(buffer);
                bool doNOTDestoryOnChangeScene = jsonobj.find("doNOTDestoryOnChangeScene") != jsonobj.end() ? jsonobj["doNOTDestoryOnChangeScene"] : false;
                bool isPureScript = jsonobj.find("isPureScript") != jsonobj.end() ? jsonobj["isPureScript"] : false;
                GameObject::InsertPrefrabs((*it).get<string>(), jsonobj);
            }
            else
            {
                string str = "ERROR : Prefrab NOT FOUND when parse Scene JSON :\n";
                str += "Scene : " + filename + "\n";
                str += "Prefrab : " + (*it).get<string>() + "NOT FOUND";
                GAME_ASSERT(false, str.c_str());
            }

            file.close();
        }
    }

    if (FindJSON("GameObject"))
    {
        for (json jsonobj : j["GameObject"])
        {
            json prefrab = nullptr;

            if(jsonobj.find("include") != jsonobj.end())
                prefrab = GameObject::GetPrefrabs(jsonobj["include"]);

            if (prefrab != nullptr)
            {
                GameObject* gobj = Instantiate(prefrab);
                gobj->ParseJSON(jsonobj);
            }
            else
            {
                bool doNOTDestoryOnChangeScene = jsonobj.find("doNOTDestoryOnChangeScene") != jsonobj.end() ? jsonobj["doNOTDestoryOnChangeScene"] : false;
                bool isPureScript = jsonobj.find("isPureScript") != jsonobj.end() ? jsonobj["isPureScript"] : false;
                GameObject* gobj = Instantiate(new GameObject(doNOTDestoryOnChangeScene, isPureScript));
                gobj->ParseJSON(jsonobj);
            }
        }
    }
}

void GameScene::LoadScene(string filename)
{
    this->loadname = filename;
}

void GameScene::LoadSceneData()
{
    this->filename = loadname;
    string PATH = R"(.\Assest\Scene\)";
    ifstream file;
    file.open(PATH + filename + ".scene");

    if (file.good())
    {
        stringstream buffer;
        buffer << file.rdbuf();
        json jsonobj = json::parse(buffer);
        this->ParseJSON(jsonobj);
    }
    else
    {
        string str = "ERROR : Scene NOT FOUND when LoadScene!\n";
        str += "Scene : " + filename;
        GAME_ASSERT(false, str.c_str());
    }

    file.close();
    this->loadname = "";
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


