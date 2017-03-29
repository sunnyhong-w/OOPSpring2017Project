#include "stdafx.h"
#include "Engine.h"
#include "slide.h" //Include you header
void Slide::ParseJSON(json j)
{
}

void Slide::Start()
{
	locked = false;
	collider = this->gameObject->GetComponent<Collider>();
	rigidbody = this->gameObject->GetComponent<Rigidbody>();
	rigidbody->TimeSliceCollision = true;
	vel = Vector2::zero;
}

void Slide::Update()
{
	bool collided = collider->PointCollision(Input::GetMouseWorldPos().GetV2I());

	if (Input::GetKeyDown(VK_LBUTTON) && collided)
	{
		locked = true;
		clickPos = Input::GetMouseWorldPos();
		oWorldPos = this->transform->GetWorldPosition();
	}

	if (Input::GetKeyUp(VK_LBUTTON))
	{
		locked = false;
		rigidbody->velocity = Vector2::zero;
		vel = Vector2::zero;
		clickPos = Vector2::zero;

		Vector2 nowPos = this->transform->GetPostion();
		Vector2 slice(64,64);
		nowPos = (nowPos / slice).round() * slice;
		
		this->transform->SetPosition(nowPos);
	}
	
	if (locked && Input::GetKeyPressing(VK_LBUTTON))
	{
		Vector2 dv = Input::GetMouseWorldPos() - clickPos;
		Vector2 tp = oWorldPos + dv;;
		Vector2::SmoothDampEX(this->transform->GetWorldPosition(), tp, vel, 0.00001f, 2, 0.5);
	}
	
	rigidbody->velocity = vel * Time::deltaTime;
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
	pDC->TextOutA(0, 20, ("mouse  pos : " + Input::GetMouseWorldPos().toString()).c_str());
}

void Slide::SendData()
{

}


