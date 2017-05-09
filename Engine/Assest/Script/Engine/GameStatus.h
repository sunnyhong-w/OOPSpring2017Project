#pragma once
#include"json_include.h"

enum class StatusName
{
	Gem
};

class GameStatus
{
	public:
		static json status;
		static void LoadFile();
		static void SaveFile();
};

