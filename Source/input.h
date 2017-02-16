#pragma once
#include <map>

using namespace std;

namespace game_engine
{
class GameScene;
class Input
{
        friend class GameScene;
    public:
        static void Update();
        static bool GetKeyTrigger(UINT nChar);
        static bool GetKeyClick(UINT nChar);
        static bool GetKeyDown(UINT nChar);
        static bool GetKeyUp(UINT nChar);
    private:
        static map<UINT, DWORD> keyEvent;
        static map<UINT, DWORD> nowState;
        static map<UINT, DWORD> lastState;
        static const DWORD trigger_ms;

};
}