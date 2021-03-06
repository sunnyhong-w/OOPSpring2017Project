#include"StdAfx.h"
#include<ddraw.h>
#include"scene.h"
#include"input.h"
#include<string>
using namespace std;
namespace game_engine
{

	map<UINT, DWORD> Input::keyEvent;
	map<UINT, DWORD> Input::nowState;
	map<UINT, DWORD> Input::lastState;
	Vector2I Input::mousePos;
	Vector2I Input::nowPos;
	Vector2I Input::lastPos;
	const DWORD Input::trigger_ms = 200;

	void Input::Update()
	{
		Input::lastState = Input::nowState;
		Input::lastPos = Input::nowPos;
		Input::nowState = Input::keyEvent;
		Input::nowPos = Input::mousePos;
	}

	bool Input::GetKeyTrigger(UINT nChar)
	{
		if (Input::nowState.find(nChar) != Input::nowState.end())
			return (clock() - Input::nowState[nChar]) >= Input::trigger_ms;
		else
			return false;
	}

	bool Input::GetKeyClick(UINT nChar)
	{
		if (Input::GetKeyUp(nChar))
			return (clock() - Input::lastState[nChar]) < Input::trigger_ms;
		else
			return false;
	}
	bool Input::GetAnyKeyDown()
	{
		for (auto key : Input::nowState)
		{
			return (key.second && lastState.find(key.first) == Input::lastState.end());
		}
		return false;
	}
	bool Input::GetKeyDown(UINT nChar)
	{
		return (Input::lastState.find(nChar) == Input::lastState.end() && Input::nowState.find(nChar) != Input::nowState.end());
	}

	bool Input::GetKeyUp(UINT nChar)
	{
		return (Input::lastState.find(nChar) != Input::lastState.end() && Input::nowState.find(nChar) == Input::nowState.end());
	}

	bool Input::GetKeyPressing(UINT nChar)
	{
		if (Input::nowState.find(nChar) != Input::nowState.end())
			return true;
		else
			return false;
	}

	Vector2I Input::GetMousePos()
	{
		return Input::nowPos;
	}

	Vector2I Input::GetMouseDeltaPos()
	{
		return Input::mousePos - Input::lastPos;
	}

	Vector2 Input::GetMouseWorldPos()
	{
		return (Input::nowPos + GameScene::CameraPosition()).GetV2();
	}

}
