#include "Main_Game.h"

bool InitProgInst(HWND hwnd)
{
	bool result = true;
	GameCore* Game;
	Game = new GameCore;
	if (!Game) result = false;
	//init the drivers
	GrafXCore(hwnd, INITGRAFX);
	return result;
}
//entry point of our game, This cantains the message interface and lets the OS and framework know when
//we need a certain event call to happen.
int RunPrgInst(HWND hwnd)
{
	int osMsg = 0;

	GrafXCore(hwnd, STARTFRAME);
	GrafXCore(hwnd, DRAW3D);
	GrafXCore(hwnd, ENDFRAME);
	return osMsg;
}
//shutdown the program
void ShutDownPrgInst(HWND hwnd)
{
	GrafXCore(hwnd, SHUTDOWNGRAFX);

}

//this initializes our game.
GameCore::GameCore()
{

}
//our constructor (not using)
GameCore::GameCore(const GameCore &)
{

}
//our destructor (this will be used as a protection incase we need to kill the container
//	but do not want to lose some objects in memory)
GameCore::~GameCore()
{

}

//This is our start vector all memory inits will begin here (think of it like setting up the
//game board before playing)
bool GameCore::Initialize()
{
	return false;
}
//this is the core of our project, this is where our games root operation resides. 
void GameCore::Run(HWND hwnd)
{

}
//this is our end function, this is where all game resident memory should be cleared and any additional contexts
void GameCore::Shutdown()
{

}
