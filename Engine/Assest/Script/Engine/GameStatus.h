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
		static json  status;
		static void LoadFile();
		static void SaveFile();
    protected:
    private:
};
