#include "stdafx.h"
#include "Engine.h"
#include "GameStatus.h" //Include you header

json  GameStatus::status;

GameStatus::GameStatus()
{
	MakeSaveFile();
}

void GameStatus::MakeSaveFile()
{
	fstream save;
	save.open("0.save", ios::out);
	save << status;
	save.close();
}
