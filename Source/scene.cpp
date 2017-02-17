#include"StdAfx.h"
#include<ddraw.h>
#include"gameobject.h"
#include"scene.h"
#include<fstream>
#include<thread>

namespace game_engine
{

void GameScene::OnInit()
{}

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

void GameScene::OnCopyData(game_framework::TransferData *TDP)
{
    TDPQueue.push_back(*TDP);
}

}


