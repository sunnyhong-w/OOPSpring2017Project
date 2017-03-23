#include "stdafx.h"
#include "Engine.h"
#include "slide.h" //Include you header
void Slide::ParseJSON(json j)
{
}

void Slide::Start()
{
	locked = false;
}

void Slide::Update()
{
	Rigidbody *rb = gameObject->GetComponent<Rigidbody>();
	rb->TimeSliceCollision = true;
	bool collided = this->gameObject->GetComponent<Collider>()->PointCollision(Input::GetMouseWorldPos().GetV2I());

	if (Input::GetKeyDown(VK_LBUTTON) && collided)
	{
		locked = true;
	}

	if (Input::GetKeyUp(VK_LBUTTON)||!collided)
	{
		locked = false;
		rb->velocity = Vector2::zero;
	}
	if (locked &&Input::GetKeyPressing(VK_LBUTTON) && collided)
	{
		rb->velocity = Input::GetMouseDeltaPos().GetV2();
	}
	//rb->velocity = Vector2::zero;
	//rb->velocity = vel * tiledPixel * Time::deltaTime ;

	if (Input::GetKeyPressing(VK_NUMPAD8))
		GameScene::CameraPosition() = GameScene::CameraPosition() + Vector2I::up * 10;
	if (Input::GetKeyPressing(VK_NUMPAD6))
		GameScene::CameraPosition() = GameScene::CameraPosition() + Vector2I::right * 10;
	if (Input::GetKeyPressing(VK_NUMPAD4))
		GameScene::CameraPosition() = GameScene::CameraPosition() + Vector2I::left * 10;
	if (Input::GetKeyPressing(VK_NUMPAD2))
		GameScene::CameraPosition() = GameScene::CameraPosition() + Vector2I::down * 10;
	if (Input::GetKeyDown(VK_NUMPAD5))
	{
		Vector2 size = Vector2((float)SIZE_X / 2, (float)SIZE_Y / 2);

		GameScene::CameraPosition() = (this->transform->GetPostion() - size).GetV2I();
	}

}

void Slide::OnRecivedBoardcast(json j)
{
}

void Slide::OnDrawGizmos(CDC * pDC)
{
	Rigidbody *rb = this->gameObject->GetComponent<Rigidbody>();
	//pDC->TextOutA(0, 20, ("colliderINFO : " + rb->colliderInfo.toString()).c_str());
	//pDC->TextOutA(0, 40, ("Can Jump : " + to_string(canJump)).c_str());
	//pDC->TextOutA(0, 60, ("velocity : " + rb->velocity.toString()).c_str());
	pDC->TextOutA(0, 20, ("mouse  pos : " + Input::GetMousePos().GetV2().toString()).c_str());
}


