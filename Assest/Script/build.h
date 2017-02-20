//this is for building
//after there is any *.h file in script,
//you can delete this file
#include"gamebehavior.h"

using namespace game_engine;

class GameScriptSample : public GameBehaviour
{
public:
	GameScriptSample(GameObject *gobj) : GameBehaviour(gobj){}
	void ParseJSON(json j) override;
};