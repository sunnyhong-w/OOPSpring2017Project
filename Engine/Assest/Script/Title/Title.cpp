#include "stdafx.h"
#include "Engine.h"
#include "Title.h" //Include you header

void Title::Start()
{
	time = 0;


}

void Title::Update()
{
	float omega = 2 * 3.14159f;
	float d = 1;
	this->gameObject->spriteRenderer->SetOffset(Vector2I(0, (int)Easing::easeFT(Easing::easeInOutSine, Easing::backForth(time, d, Time::deltaTime), -10, 10, d)));
	if (Input::GetKeyDown(VK_LBUTTON)) 
	{
		GameScene::NowScene()->LoadScene("Main_" + GameScene::GetNowScene().substr(6));
	}
}
