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
		target->GetComponent<Player>()->roomName = targetRoom;
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
