#include "stdafx.h"
#include "Engine.h"
#include "MapEdge.h" //Include you header

void MapEdge::OnCollisionStay(Collider * c)
{
    if (this->gameObject->GetLayer() == Layer::MapEdge) 
    {
        if (!targetPosition.isNull())
        {
            GameObject* camobj = GameObject::findGameObjectByName("Camera");
            camobj->transform->SetWorldPosition(targetPosition);
            GameScene::CameraPosition() = camobj->spriteRenderer->GetRealRenderPostion();

            for (auto mr : MapReader::readerList)
            {
                mr->LateUpdate();
            }

            c->gameObject->transform->SetWorldPosition(targetPosition);
			c->gameObject->GetComponent<Player>()->roomName = targetRoom;
        }
    }
}
