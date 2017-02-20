#include"StdAfx.h"
#include<ddraw.h>
#include"gameobject.h"
#include"scene.h"
#include"input.h"
#include<ctime>
#include<fstream>
#include<thread>

namespace game_engine
{

void GameScene::OnBeginState()
{
    GameObject::ResetObjectPool();

    thread th(&GameScene::LoadSceneData, this);

    //Darken the whole window

    //Render Loading Animation(force it OnDraw)
    while (!th.joinable())
        OnDraw();

    th.join();
    game_framework::CSpecialEffect::SetCurrentTime();

    for (GameObject* gobj : GameObject::gameObjects)
        if(!gobj->isStarted)
            gobj->Start();
}

void GameScene::OnMove()
{
    if (loadname != "")
        OnBeginState();
    else
    {
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

        //COLLISION DECTECTION WORK OUT HERE

        while (TDPQueue.size() != 0)
        {
            for (GameObject* gobj : GameObject::gameObjects)
                if (gobj->enable)
                    gobj->OnRecivedBoardcast(TDPQueue[0]);

            string str = "Recived JSON : \n" + TDPQueue[0].dump(4);

            AfxMessageBox(str.c_str());

            TDPQueue.erase(TDPQueue.begin());
        }

        if (Input::GetKeyClick(VK_LBUTTON))
        {
            json j;
            game_framework::CGame::Instance()->BoardcastMessage(j);
        }

        for (GameObject* gobj : GameObject::gameObjects)
            if (gobj->enable)
                gobj->Update();

        //ANIMATION UPDATE WORKOUT HERE

        for (GameObject* gobj : GameObject::gameObjects)
            if (gobj->enable)
                gobj->LateUpdate();
    }
}

void GameScene::OnShow()
{
    for (GameObject* gobj : GameObject::gameObjects)
        if (gobj->enable)
            gobj->Draw();

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
        for (json::iterator it = j["GameObject"].begin(); it != j["GameObject"].end(); ++it)
        {
            json objJSON = (*it);

            json prefrab = GameObject::GetPrefrabs(objJSON.begin().key());
            if (prefrab != nullptr)
            {
                GameObject *gobj = Instantiate(prefrab);
                gobj->ParseJSON(objJSON.begin().value());
            }
            else
            {
                json jsonobj = objJSON.begin().value();
                bool doNOTDestoryOnChangeScene = jsonobj.find("doNOTDestoryOnChangeScene") != jsonobj.end() ? jsonobj["doNOTDestoryOnChangeScene"] : false;
                bool isPureScript = jsonobj.find("isPureScript") != jsonobj.end() ? jsonobj["isPureScript"] : false;
                GameObject *gobj = Instantiate(new GameObject(doNOTDestoryOnChangeScene, isPureScript));
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


