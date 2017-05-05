#include "stdafx.h"
#include "Engine.h"
#include "Background.h" //Include you header

void Background::Start()
{
	this->camera = GameObject::findGameObjectByName("Camera");
	SetBackground("Blue");
}

void Background::Update()
{
	this->cameraPos = GameScene::CameraPosition().GetV2();
	TileMap tm = map->GetComponent<MapReader>()->tileMap;
	Vector2 bgMoveRange = tm.GetMapSize().GetV2() - this->gameObject->spriteRenderer->GetSize().GetV2();
	Vector2 cameraMapRelatePos = cameraPos - map->transform->GetWorldPosition();
	Vector2 cameraMoveRange = tm.GetMapSize().GetV2() - camera->spriteRenderer->GetSize();
	this->transform->SetPosition(map->transform->GetPostion() + (bgMoveRange * ( cameraMapRelatePos / cameraMoveRange ) ));
	TRACE(("\n cmpos " + cameraMapRelatePos.toString() + "\n").c_str());
	TRACE(("\n cmr" + cameraMoveRange.toString() + "\n").c_str());
	TRACE(this->transform->GetPostion().toString().c_str());
}

void Background::SetBackground(string roomName)
{
	this->map = GameObject::findGameObjectByName(roomName);
	this->gameObject->spriteRenderer->LoadBitmapData("Background\\" + roomName);
}
