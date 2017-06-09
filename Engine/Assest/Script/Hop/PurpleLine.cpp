#include "stdafx.h"
#include "Engine.h"
#include "PurpleLine.h" //Include you header

bool PurpleLine::lineLinked;
void PurpleLine::Start()
{
	this->RegisterEvent(BroadcastEvent::UpdateWindowPosition);
	slideWorldPos = Vector2I(0, 0);
}

void PurpleLine::Update()
{
	slideWorldPos = GameScene::CameraPosition() + (slidePos - GameScene::WindowPosition());
	lineLinked = this->gameObject->collider->PointCollision(slideWorldPos);
}

void PurpleLine::OnRecivedBroadcast(BroadcastMessageData bmd)
{
	slidePos = bmd.position;
}

void PurpleLine::OnDrawGizmos(CDC * pDC)
{
	pDC->TextOutA(0, 80, ("Slide World Pos : " + (slideWorldPos).GetV2().toString()).c_str());
}
