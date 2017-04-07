#include "stdafx.h"
#include "Engine.h"
#include "YellowBox.h" //Include you header



void YellowBox::Start()
{
	eventListener[BoardcastEvent::UpdateWindowPosition] = true;
	this->transform->SetWorldPosition(Vector2(-1000, -1000));

    this->gameObject->collider->SetEnable(false);
}

void YellowBox::LateUpdate()
{
    Vector2I worldPos = GameScene::CameraPosition() + (hoppos - GameScene::WindowPosition());
    this->gameObject->transform->SetWorldPosition(worldPos.GetV2());
}

void YellowBox::OnRecivedBoardcast(BoardcastMessageData bmd)
{
    hoppos = bmd.position;
}
