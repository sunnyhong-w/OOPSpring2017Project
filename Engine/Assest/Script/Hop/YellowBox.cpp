#include "stdafx.h"
#include "Engine.h"
#include "YellowBox.h" //Include you header



void YellowBox::Start()
{
	eventListener[BoardcastEvent::UpdateWindowPosition] = true;
	this->transform->SetWorldPosition(Vector2(-1000, -1000));
}

void YellowBox::OnRecivedBoardcast(BoardcastMessageData bmd)
{
	Vector2I worldPos = GameScene::CameraPosition() + (bmd.position - GameScene::WindowPosition());
	this->gameObject->transform->SetWorldPosition(worldPos.GetV2());
}
