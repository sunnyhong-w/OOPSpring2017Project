#include "stdafx.h"
#include "Engine.h"
#include "spring.h"

void Spring::Start()
{
	pressed = false;
	canBounce = true;
}

void Spring::OnAnimationEnd(AnimationData ad)
{
	if (ad.name == "Push")
		canBounce = true;
}

void Spring::OnCollisionStay(Collider * c)
{
	pressed = true;

	Player* pl = c->gameObject->GetComponent<Player>();
	if (pl->gameObject->rigidbody->colliderInfo.bottom && canBounce)
	{
		pl->bounce = true;
		canBounce = false;
		this->gameObject->animationController->PlayOneShot(1);
	}
}

void Spring::OnCollisionExit(Collider * c)
{
	pressed = false;
}
