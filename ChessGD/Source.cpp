#include <iostream>
#include <stdio.h>
#include "Board.h"
#include <thread>
#include "Game.h"
#include <Windows.h>
#pragma comment(lib, "winmm.lib")

int main()
{ 
	do
	{
		Game game;

		game.render_game();
	} while (false);

	return 0;
}