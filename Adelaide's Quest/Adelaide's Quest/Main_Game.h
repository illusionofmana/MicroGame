#pragma once
#include "Main.h"
//global macros

//global interfaces

//extensions

//global variables

//namespaces

//global class lookup
class GameCore
{
public:
	GameCore();
	GameCore(const GameCore&);
	~GameCore();

	bool Initialize();
	void Run(HWND hwnd);
	void Shutdown();

private:

};
//non protected universally accessable objects
