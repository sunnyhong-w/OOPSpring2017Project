#include "stdafx.h"
#include "Engine.h"
#include "Smoke.h" //Include you header

void Smoke::OnAnimationEnd(AnimationData ad)
{
	Destroy(this->gameObject);
}

void Smoke::Start()
{
	frame = 0.3f;
}

void Smoke::Update()
{
	if (frame < 0)
	{
		frame = 0.3f;
		this->transform->SetWorldPosition(this->transform->GetWorldPosition() - Vector2(0, rand() % 10 ));
	}
	frame -= Time::deltaTime;

}
