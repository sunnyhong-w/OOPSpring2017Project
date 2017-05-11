#include "stdafx.h"
#include "Engine.h"
#include "MapEdge.h" //Include you header

void MapEdge::Update()
{
	if (target != nullptr)
	{
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

	target = nullptr;
}

void MapEdge::OnCollisionStay(Collider * c)
{
    if (this->gameObject->GetLayer() == Layer::MapEdge) 
    {
        if (!targetPosition.isNull())
        {
			target = c->gameObject;
        }
    }
}
