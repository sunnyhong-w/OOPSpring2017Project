#include "stdafx.h"
#include "Engine.h"
#include "YellowBox.h" //Include you header

void YellowBox::Start()
{
}

void YellowBox::Update()
{
	this->gameObject->transform->SetPosition(GetWorldPos(Vector2(500,500)));
}

Vector2 YellowBox::GetWorldPos(Vector2 screenPos)
{
	return GameScene::CameraPosition().GetV2() + (screenPos - GameScene::WindowPosition().GetV2());
}