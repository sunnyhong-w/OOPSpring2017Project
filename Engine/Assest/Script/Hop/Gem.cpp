#include "stdafx.h"
#include "Engine.h"
#include "Gem.h" //Include you header

void Gem::Start()
{	
    name = this->gameObject->GetName();
	currentVelocity = Vector2::zero;
	if (name == "RedGem"|| name == "PurpleGem")
	{	
		this->gameObject->spriteRenderer->SetEnable(false);
		this->gameObject->collider->SetEnable(false);
		if(name == "RedGem")
			this->RegisterEvent(BroadcastEvent::RedRoomFinish);
	}

	if (GameStatus::status.find("0") !=GameStatus::status.end())
	{
		if (GameStatus::status["0"].find(this->gameObject->GetName()) != GameStatus::status["0"].end())
		{
			if ((int)GameStatus::status["0"][this->gameObject->GetName()] == 2)
			{
				Destroy(this->gameObject);
			}
		}
	}
	radius = Vector2(30.0f, 30.0f);
}

void Gem::Update()
{
    if (GameStatus::status.find(to_string((int)StatusName::Gem)) != GameStatus::status.end())
    {
        json gemstate = GameStatus::status[to_string((int)StatusName::Gem)];

        int gemcount = 0;

        for (int state : gemstate)
        {
            if (state == 2)
                gemcount++;
        }

        if (gemstate.find(name) != gemstate.end())
        {
            int state = gemstate[name];

            if (state == 2)
            {
                this->gameObject->spriteRenderer->SetAnchorRaito(Vector2(0.5, 0.5));
                Vector2 minusedScale = (this->transform->scale - Vector2::one*0.01);
                GameObject * player = GameObject::findGameObjectByName("Player");
                Vector2 targetPos = player->transform->GetWorldPosition() + (player->spriteRenderer->GetSize().GetV2() / 2);

                this->transform->scale = Vector2(clamp(minusedScale.x, 0, FLT_MAX), clamp(minusedScale.y, 0, FLT_MAX));
                this->transform->SetWorldPosition(targetPos + radius *minusedScale * Vector2(sinf((minusedScale.x + 0.25) * 2 * 3.1415926 * 4.8), cosf((minusedScale.y + 0.25) * 2 * 3.1415926 * 4.8)));
                if (this->transform->scale == Vector2::zero)
                {
                    if (gemcount == 6)
                        PassPlayer();

                    Destroy(this->gameObject);
                }
            }
        }
    }
	if (PurpleLine::lineLinked && this->name == "PurpleGem"&& !this->gameObject->spriteRenderer->GetEnable())
	{
		this->gameObject->spriteRenderer->SetEnable(true);
		this->gameObject->collider->SetEnable(true);
		auto cam = GameObject::findGameObjectByName("Camera");
		if (cam != nullptr)
		{
			cam->GetComponent<Camera>()->Shake(1, 4, 10);
		}
	}
}

void Gem::OnCollisionEnter(Collider * c)
{
	GameStatus::status[to_string((int)StatusName::Gem)][name] = 2;
	GameStatus::SaveFile();
	this->gameObject->collider->SetEnable(false);

    AudioPlayer::GetSource("GetGem")->PlayOneShot();

    if (name == "RedGem")
        game_framework::CGame::Instance()->SendEvent(WM_CLOSE, 0, 0, "Hop");
	
}

void Gem::OnRecivedBroadcast(BroadcastMessageData bmd)
{
	if (bmd.event == BroadcastEvent::RedRoomFinish)
	{
		this->gameObject->spriteRenderer->SetEnable(true);
		this->gameObject->collider->SetEnable(true);
	}
}

void Gem::PassPlayer()
{
    auto targetPosition = GameObject::findGameObjectByName("EndGamePos")->transform->GetWorldPosition();
    auto target = GameObject::findGameObjectByName("Player");
    string targetRoom = "Blue";
    
    GameObject* camobj = GameObject::findGameObjectByName("Camera");

    camobj->transform->SetWorldPosition(targetPosition);
    GameScene::CameraPosition() = camobj->spriteRenderer->GetRealRenderPostion();

    for (auto mr : MapReader::readerList)
    {
        mr->LateUpdate();
    }
    
    target->transform->SetWorldPosition(targetPosition);
    target->GetComponent<Player>()->SetRoomName(targetRoom);

    Background *bg = GameObject::findGameObjectByName("Background")->GetComponent<Background>();
    bg->SetBackground(targetRoom);

    json pass;
    pass["name"] = targetRoom;
    GameScene::Broadcast(BroadcastEvent::ChangeRoom, pass);

    GameStatus::status["last"]["room"] = targetRoom;
    GameStatus::status["last"]["position"] = targetPosition;
    GameStatus::SaveFile();
}
