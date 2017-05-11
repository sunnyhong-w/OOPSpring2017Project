#include "stdafx.h"
#include "Engine.h"
#include "spawnPlayer.h" //Include you header


void SpawnPlayer::ParseJSON(json j)
{
    count = 0;
    target = 0;

    if (j.find("wait") != j.end())
        this->target = j["wait"];
}

void SpawnPlayer::Start()
{
	pos = this->gameObject->spriteRenderer->GetRealRenderPostion().GetV2();
    this->gameObject->RemoveComponent<SpriteRenderer>();

    targetRoom = "Blue";

    if (GameStatus::status.find("last") != GameStatus::status.end())
    {
        targetRoom = GameStatus::status["last"]["room"].get<string>();
        pos = GameStatus::status["last"]["position"];
    }
}

void SpawnPlayer::Update()
{
    if (count == target)
    {
        GameObject* gobj = GameScene::CreateGameObject("spawnPlayer", { { "include" , "player" } });
        gobj->transform->SetWorldPosition(pos);
        gobj->GetComponent<Player>()->SetRoomName(targetRoom);

        Background *bg = GameObject::findGameObjectByName("Background")->GetComponent<Background>();
        bg->SetBackground(targetRoom);

        json pass;
        pass["name"] = targetRoom;
        GameScene::Broadcast(BroadcastEvent::ChangeRoom, pass);

        Destroy(this->gameObject);
    }

    count++;
}

void SpawnPlayer::OnDrawGizmos(CDC * pDC)
{
    pDC->TextOutA(0,100,to_string(count).c_str());
}
