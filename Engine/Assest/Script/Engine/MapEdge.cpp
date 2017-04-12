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
            c->gameObject->transform->SetWorldPosition(targetPosition);
        }
    }
}
