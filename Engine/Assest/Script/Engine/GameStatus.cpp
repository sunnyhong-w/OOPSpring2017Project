#include "stdafx.h"
#include "Engine.h"
#include "GameStatus.h" //Include you header

json  GameStatus::status;



void GameStatus::LoadFile()
{
	fstream load;
	load.open("0.save", ios::in);
	if (load)
	{
		status << load;
	}
		load.close();
}

void GameStatus::SaveFile()
{
	fstream save;
	save.open("0.save", ios::out);
	save << status;
	save.close();
}
