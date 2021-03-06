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
		static bool GetAnyKeyDown();
        static bool GetKeyDown(UINT nChar);
        static bool GetKeyUp(UINT nChar);
		static bool GetKeyPressing(UINT nChar);
        static Vector2I GetMousePos();
		static Vector2I GetMouseDeltaPos();
        static Vector2 GetMouseWorldPos();

    private:
        static map<UINT, DWORD> keyEvent;
        static map<UINT, DWORD> nowState;
        static map<UINT, DWORD> lastState;
        static Vector2I nowPos;
		static Vector2I lastPos;
        static Vector2I mousePos;
        static const DWORD trigger_ms;

};
}