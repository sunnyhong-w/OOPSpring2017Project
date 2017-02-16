#include"StdAfx.h"
#include"gamelib.h"
#include"input.h"
#include"scene.h"
namespace game_engine
{
map<UINT, DWORD> Input::keyEvent;
map<UINT, DWORD> Input::nowState;
map<UINT, DWORD> Input::lastState;
const DWORD Input::trigger_ms = 1000;
void Input::Update()
{
    Input::lastState = Input::nowState;
    Input::nowState = Input::keyEvent;
}
bool Input::GetKeyTrigger(UINT nChar)
{
    if (Input::nowState.find(nChar) != Input::nowState.end())
        return (game_framework::CSpecialEffect::GetEllipseTime() - Input::nowState[nChar]) >= Input::trigger_ms;
    else
        return false;
}
bool Input::GetKeyClick(UINT nChar)
{
    if (Input::GetKeyUp(nChar))
        return (game_framework::CSpecialEffect::GetEllipseTime() - Input::lastState[nChar]) < Input::trigger_ms;
    else
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
}
