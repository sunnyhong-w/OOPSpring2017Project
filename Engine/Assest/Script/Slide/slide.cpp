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
	targetPosition = this->transform->GetPostion();
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
		targetPosition = (nowPos / slice).round() * slice;
	}
	
	if (locked && Input::GetKeyPressing(VK_LBUTTON))
	{
		Vector2 dv = Input::GetMouseWorldPos() - clickPos;
		targetPosition = oWorldPos + dv;
		Vector2 wpos = this->transform->GetWorldPosition();
		Vector2 newpos = Vector2::SmoothDampEX(wpos, targetPosition, vel, 0.003f, 2, 0.5f);

		rigidbody->velocity = newpos - wpos;
	}
	else
	{
		Vector2 vpos = this->transform->GetPostion();
		Vector2 newpos = Vector2::SmoothDampEX(vpos, targetPosition, vel, 15.0f, 10, 9.0f);

		rigidbody->velocity = newpos - vpos;
	}
	
	//rigidbody->velocity = vel * Time::deltaTime;
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
	Vector2 pos = transform->GetPostion();
	Vector2 worldpos = transform->GetWorldPosition();
	pDC->TextOutA(worldpos.x, worldpos.y, (pos.toString()).c_str());
}

void Slide::SendData()
{

}


