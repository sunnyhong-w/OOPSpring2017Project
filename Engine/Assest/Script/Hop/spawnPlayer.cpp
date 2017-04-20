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
}

void SpawnPlayer::Update()
{
    if (count == target)
    {
        GameObject* gobj = GameScene::CreateGameObject("spawnPlayer", { { "include" , "player" } });
        gobj->transform->SetWorldPosition(pos);
        Destroy(this->gameObject);
    }

    count++;
}

void SpawnPlayer::OnDrawGizmos(CDC * pDC)
{
    pDC->TextOutA(0,100,to_string(count).c_str());
}
