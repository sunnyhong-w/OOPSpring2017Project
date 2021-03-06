#include "stdafx.h"
#include "Engine.h"
#include "slide.h" //Include you header
void Slide::ParseJSON(json j)
{
    if (j.find("tileSetting") != j.end())
    {
        for (auto sbsjson : j["tileSetting"])
            this->boxdata.push_back(sbsjson);
    }
}

void Slide::Start()
{
	locked = false;
    this->gameObject->rigidbody->TimeSliceCollision = true;
	vel = Vector2::zero;
    targetPos = this->transform->GetPostion();
}

void Slide::Update()
{
	bool collided = this->gameObject->collider->PointCollision(Input::GetMouseWorldPos().GetV2I());
    this->gameObject->rigidbody->velocity = Vector2::zero;


	if (Input::GetKeyDown(VK_LBUTTON) && collided)
	{
		locked = true;
		clickPos = this->transform->GetParent()->GetWorldPosition() + this->transform->GetPostion().sliceRound(Vector2(64, 64)) + Vector2(32, 32);
		oWorldPos = this->transform->GetWorldPosition();

        AudioPlayer::GetSource("Select")->PlayOneShot();
	}

	if (Input::GetKeyUp(VK_LBUTTON))
	{
        if(locked)
            AudioPlayer::GetSource("Release")->PlayOneShot();

		locked = false;
        this->gameObject->rigidbody->velocity = Vector2::zero;
		//vel = Vector2::zero;
		clickPos = Vector2::zero;

		Vector2 nowPos = this->transform->GetPostion();
		Vector2 slice(64,64);
        targetPos = (nowPos / slice).round() * slice;

        this->transform->GetParent()->gameObject->GetComponent<BoxParent>()->CheckData();
	}
	
	if (locked && Input::GetKeyPressing(VK_LBUTTON))
	{
		Vector2 dv = Input::GetMouseWorldPos() - clickPos;
		Vector2 wpos = this->transform->GetWorldPosition();
        Vector2 tpos = oWorldPos + dv;

        if (wpos != tpos)
        {
            Vector2 newpos = Vector2::Damp(wpos, tpos, vel, 0.003f, 2, 1.0f);
            this->gameObject->rigidbody->velocity = (newpos - wpos).floorSpecial();
        }
	}
    else
    {
        if (this->transform->GetPostion() != targetPos)
        {
            Vector2 wpos = this->transform->GetPostion();
            this->gameObject->rigidbody->velocity = Vector2::Damp(wpos, targetPos, vel, 0.003f, 16, 1.0f) - wpos;
        }
    }
}

void Slide::OnRecivedBroadcast(BroadcastMessageData bmd)
{
}

void Slide::OnDrawGizmos(CDC * pDC)
{
	Rigidbody *rb = this->gameObject->rigidbody;
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

void from_json(const json & j, PassState & ps)
{
    ps.top = ps.bottom = ps.left = ps.right = false;

    if (j.find("top") != j.end())
        ps.top = j["top"];

    if (j.find("bottom") != j.end())
        ps.bottom = j["bottom"];

    if (j.find("left") != j.end())
        ps.left = j["left"];

    if (j.find("right") != j.end())
        ps.right = j["right"];
}

void to_json(json & j, const PassState & ps)
{
    j["top"] = ps.top;
    j["bottom"] = ps.bottom;
    j["left"] = ps.left;
    j["right"] = ps.right;
}

void from_json(const json & j, SlideBoxSetting & sbs)
{
    sbs.position = j["position"];

    if(j.find("state") != j.end())
        sbs.state = j["state"];
}

void to_json(json & j, const SlideBoxSetting & sbs)
{
    j["position"] = sbs.position;
    j["state"] = sbs.state;
}

SlideBoxSetting::SlideBoxSetting()
{
    position == Vector2I::null;
    state.bottom = state.left = state.right = state.top = false;
}
