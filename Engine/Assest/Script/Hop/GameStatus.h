#pragma once
#include <fstream>
using namespace game_engine;
enum class StatusName
{
	Gem
};
class GameStatus
{
	public:
		GameStatus();
		static json  status;
		static void MakeSaveFile();
    protected:
    private:
};
