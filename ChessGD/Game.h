#pragma once
#include "Board.h"
#include <vector>
#include "conio.h"
#include <SFML/Graphics.hpp>
#include <fstream>
#include <SFML/Audio.hpp>

using namespace std;

class Game
{
private:
	Board board;
	bool isMenu;
	bool whiteTurn;
	bool soundOn;
	bool canRedo;
	std::vector<std::string> moves;
	std::vector<std::string> redoMoves;
	std::vector<std::pair<sf::SoundBuffer, sf::Sound>> soundTrack;
	void playMusic(int i);
	void stopMusic(int i);
	ofstream outfile;
public:
	Game();

	void recordMove(int old_row, int old_col, int new_row, int new_col, MoveType moveType);
	void undo();
	void redo();
	void render_game();
	int gameMode();
	char promoteWindow();
	int resultWindow(int res);
	int menuWindow(sf::RenderWindow& game);
	void drawPossibleMoves(sf::RenderWindow& game, const sf::Sprite* square, std::vector<std::pair<int, int>> movesList, char piePre);
	int playWithPC(sf::RenderWindow& game, bool checkLoad);
	int playWithHuman(sf::RenderWindow& game, bool checkLoad);
	void replayGame(sf::RenderWindow& game, int mode);
	void setSound(bool setSound);
	void saveGame();
	void loadGame(sf::RenderWindow& game);
};