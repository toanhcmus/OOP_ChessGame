#include "Game.h"
#include <sstream>
#include <string>

using namespace std;

void Game::playMusic(int i)
{
	switch (i)
	{
	case 0:
		soundOn ? this->soundTrack[0].second.play() : this->soundTrack[0].second.stop();
		break;
	case 1:
		soundOn ? this->soundTrack[1].second.play() : this->soundTrack[1].second.stop();
		break;
	case 2:
		soundOn ? this->soundTrack[2].second.play() : this->soundTrack[2].second.stop();
		break;
	case 3:
		soundOn ? this->soundTrack[3].second.play() : this->soundTrack[3].second.stop();
		break;
	}
}

void Game::stopMusic(int i)
{
	this->soundTrack[i].second.stop();
}

Game::Game() :board(Board()), isMenu(false), whiteTurn(true), moves(std::vector<std::string>()), soundOn(true), canRedo(false)
{
	for (int i = 0; i < 4; ++i)
		this->soundTrack.push_back({});

	this->soundTrack[0].first.loadFromFile("soundtrack/background.wav");
	this->soundTrack[0].second.setBuffer(this->soundTrack[0].first);
	this->soundTrack[0].second.setLoop(true);
	this->soundTrack[0].second.setVolume(100);

	this->soundTrack[1].first.loadFromFile("soundtrack/menu.wav");
	this->soundTrack[1].second.setBuffer(this->soundTrack[1].first);
	this->soundTrack[1].second.setLoop(true);
	this->soundTrack[1].second.setVolume(100);


	this->soundTrack[2].first.loadFromFile("soundtrack/move.wav");
	this->soundTrack[2].second.setBuffer(this->soundTrack[2].first);
	this->soundTrack[2].second.setVolume(50);

	this->soundTrack[3].first.loadFromFile("soundtrack/capture.wav");
	this->soundTrack[3].second.setBuffer(this->soundTrack[3].first);
	this->soundTrack[3].second.setVolume(50);
}

void Game::recordMove(int old_row, int old_col, int new_row, int new_col, MoveType moveType)
{
	this->board.getPiece(old_row, old_col)->hasMoved();
	std::string move = to_string(old_row) + " " + to_string(old_col) + " " + to_string(new_row) + " " + to_string(new_col) + " " + to_string(moveType);

	!this->board.getPiece(old_row, old_col)->hasMoved() ? move += " 1 " : move += " 0 ";

	this->moves.push_back(move);
}

void Game::undo()
{
	if (!this->moves.size())
	{
		return;
	}

	int new_row = this->moves.back()[0] - '0';
	int new_col = this->moves.back()[2] - '0';
	int old_row = this->moves.back()[4] - '0';
	int old_col = this->moves.back()[6] - '0';
	
	bool hasMove = this->moves.back()[10] - '0';

	MoveType moveType = static_cast<MoveType>(this->moves.back()[8] - '0');

	this->canRedo = true;
	this->redoMoves.push_back(this->moves.back());

	this->moves.pop_back();
	std::cout << "white turn: " << whiteTurn << " -> " << !whiteTurn << "\n";
	std::cout << this->redoMoves.back() << "\n";

	this->board.undo(old_row, old_col, new_row, new_col, moveType, this->whiteTurn);

	if (moveType != longCastling && moveType != shortCastling)
		this->board.getPiece(new_row, new_col)->setHasMoved(hasMove);

	this->whiteTurn = !this->whiteTurn;
}

void Game::redo()
{
	if (!this->redoMoves.size() || !this->canRedo)
	{
		this->canRedo = false;

		return;
	}
	int new_row = this->redoMoves.back()[4] - '0';
	int new_col = this->redoMoves.back()[6] - '0';
	int old_row = this->redoMoves.back()[0] - '0';
	int old_col = this->redoMoves.back()[2] - '0';
	std::cout << this->redoMoves.back()[12];

	if (this->board.canMove(old_row, old_col, new_row, new_col, this->whiteTurn))
	{
		std::cout << "redid\n";
		this->board.makeMove(old_row, old_col, new_row, new_col, this->whiteTurn);
		
		if (this->board.getMoveType() == promote || this->board.getMoveType() == capture_promote)
		{
			this->board.promotePawn(new_row, new_col, this->redoMoves.back()[12]);
		}

		this->moves.push_back(this->redoMoves.back());
		this->redoMoves.pop_back();

		this->whiteTurn = !this->whiteTurn;
	}
}

void Game::render_game()
{
	sf::RenderWindow game(sf::VideoMode(1024, 800), "Chess", sf::Style::Close);
	this->playMusic(1);
	while (game.isOpen())
	{
		
		this->isMenu = true;

		this->whiteTurn = true;
		if (!menuWindow(game))
		{
			return;
		}

		int mode = gameMode();

		if (mode == -1)
		{
			continue;
		}

		this->isMenu = false;

		this->stopMusic(1);
		this->playMusic(0);

		if (mode == 2)
		{
			loadGame(game);

			if (!game.isOpen())
			{
				return;
			}

			this->stopMusic(0);
			game.clear();
			this->moves.clear();
			this->redoMoves.clear();
			this->canRedo = false;
			continue;

		}

		if (mode == 1)
		{
			this->board.setBoard();
			playWithPC(game, false);

			this->stopMusic(0);
			game.clear();
			this->moves.clear();
			this->redoMoves.clear();
			this->canRedo = false;
			continue;
		}
		this->board.setBoard();
		playWithHuman(game, false);
		this->moves.clear();
		this->redoMoves.clear();
		this->canRedo = false;
		this->stopMusic(0);
		game.clear();
		this->board.setBoard();
	}
}

int Game::gameMode()
{
	sf::RenderWindow menu(sf::VideoMode(620, 800), "Mode", sf::Style::Close);
	sf::Texture button[6];
	sf::Texture background;
	sf::Sprite button_sprite[6];
	sf::Sprite background_spr;

	background.loadFromFile("sprites/start_menu.png");
	background_spr.setTexture(background);

	for (int i = 0; i < 3; ++i)
	{
		button[i].loadFromFile("sprites/start_buttons.png", sf::IntRect(0, 150 * i, 440, 150));
		button_sprite[i].setTexture(button[i]);
		button_sprite[i].setPosition(sf::Vector2f(90, 260 + 160 * i));
	}

	for (int i = 3; i < 6; ++i)
	{
		button[i].loadFromFile("sprites/start_buttons.png", sf::IntRect(440, 150 * (i - 3), 440, 150));
		button_sprite[i].setTexture(button[i]);
		button_sprite[i].setPosition(sf::Vector2f(90, 260 + 160 * (i- 3)));
	}

	while (menu.isOpen())
	{
		sf::Event event;
		sf::Vector2i pos = sf::Mouse::getPosition(menu);
	
		while (menu.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:

				return -1;
			case sf::Event::MouseButtonPressed:
				if (event.key.code != sf::Mouse::Left)
				{
					break;
				}

				for (int i = 0; i < 3; ++i)
				{
					if (!button_sprite[i].getGlobalBounds().contains(pos.x, pos.y) || !button_sprite[i + 3].getGlobalBounds().contains(pos.x, pos.y))
					{
						continue;
					}

					return i;
				}
			}
		}

		menu.clear();
		menu.draw(background_spr);

		for (int i = 0; i < 3; ++i)
		{
			if (!button_sprite[i].getGlobalBounds().contains(pos.x, pos.y) || !button_sprite[i + 3].getGlobalBounds().contains(pos.x, pos.y))
			{
				menu.draw(button_sprite[i]);
				continue;
			}

			menu.draw(button_sprite[i + 3]);
		}

		menu.display();
	}

	return -1;
}

char Game::promoteWindow()
{
	sf::RenderWindow startMenu(sf::VideoMode(620, 800), "Promote", sf::Style::Close);
	sf::Texture button[8];
	sf::Texture background;
	sf::Sprite button_sprite[8];
	sf::Sprite background_spr;

	background.loadFromFile("sprites/promote_bg.png");
	background_spr.setTexture(background);

	for (int i = 0; i < 4; ++i)
	{
		button[i].loadFromFile("sprites/promote_b.png", sf::IntRect(0, 130 * i, 400, 130));
		button_sprite[i].setTexture(button[i]);
		button_sprite[i].setPosition(sf::Vector2f(110, 200 + 140 * i));
	}

	for (int i = 4; i < 8; ++i)
	{
		button[i].loadFromFile("sprites/promote_b.png", sf::IntRect(400, 130 * (i - 4), 400, 130));
		button_sprite[i].setTexture(button[i]);
		button_sprite[i].setPosition(sf::Vector2f(110, 200 + 140 * (i - 4)));
	}

	while (startMenu.isOpen())
	{
		sf::Event event;
		sf::Vector2i pos = sf::Mouse::getPosition(startMenu);

		while (startMenu.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				break;

			case sf::Event::MouseButtonReleased:
				if (event.key.code != sf::Mouse::Left)
				{
					break;
				}

				for (int i = 0; i < 4; ++i)
				{
					if (!button_sprite[i].getGlobalBounds().contains(pos.x, pos.y) && !button_sprite[i + 4].getGlobalBounds().contains(pos.x, pos.y))
					{
						continue;
					}

					switch (i)
					{
					case 0:
						return 'R';
					case 1:
						return 'N';
					case 2:
						return 'B';
					case 3: 
						return 'Q';
					}
				}
			}
		}

		startMenu.clear();
		startMenu.draw(background_spr);

		for (int i = 0; i < 4; ++i)
		{
			if (!button_sprite[i].getGlobalBounds().contains(pos.x, pos.y) && !button_sprite[i + 4].getGlobalBounds().contains(pos.x, pos.y))
			{
				startMenu.draw(button_sprite[i]);
				continue;
			}

			startMenu.draw(button_sprite[i + 4]);
		}

		startMenu.display();
	}

	return '-1';
}

int Game::resultWindow(int res)
{
	sf::RenderWindow startMenu(sf::VideoMode(600, 330), "Result", sf::Style::Close);
	sf::Texture button[2];
	sf::Texture background;
	sf::Sprite button_sprite[2];
	sf::Sprite background_spr;

	switch (res)
	{
	case -1:
		background.loadFromFile("sprites/bwin.png");
		break;
	case 0:
		background.loadFromFile("sprites/draw.png");
		break;
	case 1:
		background.loadFromFile("sprites/wwin.png");
		break;
	}

	background_spr.setTexture(background);

	for (int i = 0; i < 2; ++i)
	{
		button[i].loadFromFile("sprites/resB.png", sf::IntRect(200 * i, 0, 200, 65));
		button_sprite[i].setTexture(button[i]);
		button_sprite[i].setPosition(sf::Vector2f(190, 255));
	}

	while (startMenu.isOpen())
	{
		sf::Event event;
		sf::Vector2i pos = sf::Mouse::getPosition(startMenu);

		while (startMenu.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				startMenu.close();

				return 0;
			case sf::Event::MouseButtonReleased:
				if (event.key.code != sf::Mouse::Left)
				{
					break;
				}

				if (button_sprite[0].getGlobalBounds().contains(pos.x, pos.y) || button_sprite[1].getGlobalBounds().contains(pos.x, pos.y))
					return 1;
			}
		}

		startMenu.clear();
		startMenu.draw(background_spr);

		if (button_sprite[0].getGlobalBounds().contains(pos.x, pos.y) || button_sprite[1].getGlobalBounds().contains(pos.x, pos.y))
		{
			startMenu.draw(button_sprite[1]);
		}

		if (!button_sprite[0].getGlobalBounds().contains(pos.x, pos.y) && !button_sprite[1].getGlobalBounds().contains(pos.x, pos.y))
		{
			startMenu.draw(button_sprite[0]);
		}

		startMenu.display();
	}

	return -1;
}

int Game::menuWindow(sf::RenderWindow& game)
{
	game.clear();
	sf::Texture menu;
	sf::Texture startB_inbound;
	sf::Texture startB;
	sf::Texture exitButton[2];
	sf::Texture menuB[2];
	sf::Texture sounds[2];

	sf::Sprite menu_sprite;
	sf::Sprite startB_spr;
	sf::Sprite startB_ib_spr;
	sf::Sprite exit_spr[2];
	sf::Sprite menuB_spr[2];
	sf::Sprite sound_spr[2];

	menu.loadFromFile("sprites/menu_BG.png");
	menuB[0].loadFromFile("sprites/menuB.png");
	menuB[1].loadFromFile("sprites/menuB_inBound.png");
	startB.loadFromFile("sprites/startB.png");
	startB_inbound.loadFromFile("sprites/startB_inBound.png");


	menu_sprite.setTexture(menu);
	startB_spr.setTexture(startB);
	startB_ib_spr.setTexture(startB_inbound);


	for (int i = 0; i < 2; ++i)
	{
		exitButton[i].loadFromFile("sprites/exit_button.png", sf::IntRect(300 * i, 0, 300, 110));
		sounds[i].loadFromFile("sprites/sound_turn.png", sf::IntRect(0 + 100 * i, 300, 100, 100));
		exit_spr[i].setTexture(exitButton[i]);
		menuB_spr[i].setTexture(menuB[i]);
		sound_spr[i].setTexture(sounds[i]);
		sound_spr[i].setPosition(460, 600.5);
	}

	startB_spr.setPosition(362, 330.5);
	startB_ib_spr.setPosition(362, 330.5);
	exit_spr[0].setPosition(362, 450.5);
	exit_spr[1].setPosition(362, 450.5);

	int choice = -1;

	while (true)
	{
		sf::Event event;
		sf::Vector2i pos = sf::Mouse::getPosition(game);
		game.waitEvent(event);
		while (game.pollEvent(event));
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				game.close();

				return 0;
			case sf::Event::MouseButtonPressed:
				if (event.key.code != sf::Mouse::Left)
				{
					break;
				}

				if (sound_spr[0].getGlobalBounds().contains(pos.x, pos.y) || sound_spr[1].getGlobalBounds().contains(pos.x, pos.y))
				{
					this->setSound(!this->soundOn);

					break;
				}
			case sf::Event::MouseButtonReleased:
				if (event.key.code != sf::Mouse::Left)
				{
					break;
				}

				if (startB_spr.getGlobalBounds().contains(pos.x, pos.y) || startB_ib_spr.getGlobalBounds().contains(pos.x, pos.y))
				{
					choice = 1;

					break;
				}

				if (exit_spr[0].getGlobalBounds().contains(pos.x, pos.y) || exit_spr[1].getGlobalBounds().contains(pos.x, pos.y))
				{
					choice = 0;

					break;
				}
			}
		}

		if (choice >= 0)
		{
			break;
		}

		game.clear();
		game.draw(menu_sprite);

		if (!startB_spr.getGlobalBounds().contains(pos.x, pos.y) && !startB_ib_spr.getGlobalBounds().contains(pos.x, pos.y))
		{
			game.draw(startB_spr);
		}

		if (startB_spr.getGlobalBounds().contains(pos.x, pos.y) || startB_ib_spr.getGlobalBounds().contains(pos.x, pos.y))
		{
			game.draw(startB_ib_spr);
		}

		if (exit_spr[0].getGlobalBounds().contains(pos.x, pos.y) || exit_spr[1].getGlobalBounds().contains(pos.x, pos.y))
		{
			game.draw(exit_spr[1]);
		}

		if (!exit_spr[0].getGlobalBounds().contains(pos.x, pos.y) && !exit_spr[1].getGlobalBounds().contains(pos.x, pos.y))
		{
			game.draw(exit_spr[0]);
		}

		
		this->soundOn ? game.draw(sound_spr[0]) : game.draw(sound_spr[1]);

		game.display();
	}

	return choice;
}

void Game::drawPossibleMoves(sf::RenderWindow& game, const sf::Sprite* square, std::vector<std::pair<int, int>> movesList, char piePre)
{
	for (auto i : movesList)
	{
		if (piePre == 'P') {
			if ((i.first == 7 || i.first == 0))
			{
				sf::Sprite p(square[4]);
				p.setPosition(44 + 64 * i.second, 44 + 64 * i.first);
				game.draw(p);

				continue;
			}
		}
		

		if (piePre == 'K' && this->board.getPiece(i.first, i.second)) {
			if (this->board.getPiece(i.first, i.second)->getPiece() == 'R') {
				sf::Sprite c(square[4]);
				c.setPosition(44 + 64 * i.second, 44 + 64 * i.first);
				game.draw(c);

				continue;
			}
		}

		if (this->board.getPiece(i.first, i.second))
		{
			if (this->board.getPiece(i.first, i.second)->getPiece() == 'K') {
				sf::Sprite c(square[4]);
				c.setPosition(44 + 64 * i.second, 44 + 64 * i.first);
				game.draw(c);

				continue;
			}
			sf::Sprite c(square[2]);
			c.setPosition(44 + 64 * i.second, 44 + 64 * i.first);
			game.draw(c);

			continue;
		}

		sf::Sprite p(square[0]);
		p.setPosition(44 + 64 * i.second, 44 + 64 * i.first);
		game.draw(p);
	}
}

int Game::playWithPC(sf::RenderWindow& game, bool checkLoad)
{
	game.clear();
	sf::Texture interfaceBackground;
	sf::Texture turn[2];
	sf::Texture volume[2];
	sf::Texture save[2];
	sf::Texture undo_redo[4];
	sf::Texture close[2];
	sf::Texture resign[2];

	sf::Sprite close_spr[2];
	sf::Sprite resign_spr[2];
	sf::Sprite undo_redo_spr[4];
	sf::Sprite interfaceBackground_spr;
	sf::Sprite turn_spr[2];
	sf::Sprite vol_spr[2];
	sf::Sprite save_spr[2];
	sf::Vector2f oldPos, newPos, offset(16, 16);

	for (int i = 0; i < 2; ++i)
	{
		close[i].loadFromFile("sprites/resign.png", sf::IntRect(100 * i, 241, 100, 100));
		resign[i].loadFromFile("sprites/resign.png", sf::IntRect(0, 120 * i, 280, 120));

		close_spr[i].setTexture(close[i]);
		resign_spr[i].setTexture(resign[i]);

		resign_spr[i].setPosition(672, 630);
		close_spr[i].setPosition(840, 45);


		undo_redo[i].loadFromFile("sprites/undo_redo.png", sf::IntRect(130 * i, 0, 130, 110));
		undo_redo[i + 2].loadFromFile("sprites/undo_redo.png", sf::IntRect(130 * i, 110, 130, 110));

		undo_redo_spr[i].setTexture(undo_redo[i]);
		undo_redo_spr[i + 2].setTexture(undo_redo[i + 2]);

		undo_redo_spr[i * 2].setPosition(650, 510);
		undo_redo_spr[1 + i * 2].setPosition(830, 510);

		turn[i].loadFromFile("sprites/sound_turn.png", sf::IntRect(0, 150 * i, 350, 150));
		turn_spr[i].setTexture(turn[i]);
		turn_spr[i].setPosition(640, 170);

		volume[i].loadFromFile("sprites/sound_turn.png", sf::IntRect(i * 100, 300, 100, 100));
		vol_spr[i].setTexture(volume[i]);
		vol_spr[i].setPosition(680, 45);

		save[i].loadFromFile("sprites/save.png", sf::IntRect(0, 260 + i * 130, 400, 130));
		save_spr[i].setTexture(save[i]);
		save_spr[i].setPosition(615, 345);
	}

	int board_x = 0, board_y = 0;
	bool moved = false;
	bool startB_ib = false;

	interfaceBackground.loadFromFile("sprites/board_int.png");

	interfaceBackground_spr.setTexture(interfaceBackground);

	board.draw(game);

	if (checkLoad)
	{
		outfile.open("moves.txt", ios::app);
	}
	else
	{
		outfile.open("moves.txt", ios::out);
		outfile << "P" << endl;
	}

	bool selected = false;
	std::pair<int, int> chosenSquare(-1, -1);
	sf::Texture posSquare;
	sf::Texture selSquare;
	sf::Texture squares[5];
	sf::Sprite sq_spr[5];

	for (int i = 0; i < 5; ++i)
	{
		squares[i].loadFromFile("sprites/sq_status.png", sf::IntRect(0, 64 * i, 64, 64));
		sq_spr[i].setTexture(squares[i]);
	}


	sf::Sprite posSq_spr;
	sf::Sprite selSq_spr;
	posSq_spr.setTexture(posSquare);
	selSq_spr.setTexture(selSquare);

	int choiceResult;

	while (game.isOpen())
	{
		if (!this->board.canMoveNext(whiteTurn))
		{
			break;
		}
		sf::Event event;
		sf::Vector2i pos = sf::Mouse::getPosition(game) - sf::Vector2i(offset);

		if (!whiteTurn) {
			srand(time(NULL));
			while (true) {
				int i = rand() % board.getBlackPieces().size();
				Piece* randPiece = board.getBlackPieces()[i];
				Square squareSrc = board.getSquare(randPiece);
				std::cout << squareSrc.getX() << " " << squareSrc.getY() << " ";

				if (board.getPossibleMoves(squareSrc.getX(), squareSrc.getY()).size() > 0) {
					i = rand() % board.getPossibleMoves(squareSrc.getX(), squareSrc.getY()).size();

					Square squareDest;
					squareDest.setX(board.getPossibleMoves(squareSrc.getX(), squareSrc.getY())[i].first);
					squareDest.setY(board.getPossibleMoves(squareSrc.getX(), squareSrc.getY())[i].second);
					int oldRow = squareSrc.getX();
					int oldCol = squareSrc.getY();
					int newRow = squareDest.getX();
					int newCol = squareDest.getY();

					std::cout << " move to " << squareDest.getX() << " " << squareDest.getY() << endl;
					if (this->board.canMove(oldRow, oldCol, newRow, newCol, this->whiteTurn)) {
						this->board.isCaptureMove() ? playMusic(3) : playMusic(2);
						this->whiteTurn = !this->whiteTurn;
						this->recordMove(oldRow, oldCol, newRow, newCol, this->board.getMoveType());

						board.makeMove(oldRow, oldCol, newRow, newCol, this->whiteTurn);

						if (this->board.getPiece(newRow, newCol)) {
							if ((this->board.getPiece(newRow, newCol)->isWhite() && newRow == 0) || (!this->board.getPiece(newRow, newCol)->isWhite() && newRow == 7)) {
								if (this->board.getPiece(newRow, newCol)->getPiece() == 'P') {
									string p = "RBNQ";
									int i = rand() % 4;
									char chosen = p[i];
									this->board.promotePawn(newRow, newCol, chosen);

									std::string move = this->moves.back();
									move += chosen;
									this->moves.pop_back();
									this->moves.push_back(move);

									Sleep(1000);
								}
							}
						}

						break;
					}
				}
				std::cout << endl;
			}
		}

		while (game.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				game.close();
				outfile.close();
				return 0;

			case sf::Event::MouseButtonPressed: //Nhan chuot trai
				if (event.key.code != sf::Mouse::Left)
				{
					break;
				}

				if (undo_redo_spr[0].getGlobalBounds().contains(pos.x, pos.y) || undo_redo_spr[2].getGlobalBounds().contains(pos.x, pos.y))
				{
					this->undo();
					this->undo();

					break;
				}


				if (close_spr[0].getGlobalBounds().contains(pos.x, pos.y) || close_spr[1].getGlobalBounds().contains(pos.x, pos.y))
				{
					game.clear();

					return 0;
				}


				if (resign_spr[0].getGlobalBounds().contains(pos.x, pos.y) || resign_spr[1].getGlobalBounds().contains(pos.x, pos.y))
				{
					choiceResult = this->resultWindow(-1);
					if (choiceResult == 1)
					{
						replayGame(game, 1);
						game.clear();
					}

					return 0;
				}

				if (undo_redo_spr[1].getGlobalBounds().contains(pos.x, pos.y) || undo_redo_spr[3].getGlobalBounds().contains(pos.x, pos.y))
				{
					this->redo();
					this->redo();

					break;
				}

				if (save_spr[0].getGlobalBounds().contains(pos.x, pos.y) || save_spr[1].getGlobalBounds().contains(pos.x, pos.y))
				{
					saveGame();
					return 3;
				}

				if (vol_spr[0].getGlobalBounds().contains(pos.x, pos.y) || vol_spr[1].getGlobalBounds().contains(pos.x, pos.y))
				{
					this->setSound(!this->soundOn);

					break;
				}

				for (int i = 0; i < 8; ++i)
					for (int j = 0; j < 8; ++j)
					{
						if (!board.getPiece(i, j))
						{
							continue;
						}

						if (board.getPiece(i, j)->getSprite().getGlobalBounds().contains(pos.x, pos.y))
						{
							if (this->board.canMove(board_x, board_y, i, j, this->whiteTurn))
							{
								break;
							}

							moved = true;
							board_x = i;
							board_y = j;
							oldPos = board.getPiece(board_x, board_y)->getSprite().getPosition();

							if (board.getPiece(board_x, board_y)->isWhite() != whiteTurn)
							{
								selected = false;
								break;
							}

							if (selected && i == chosenSquare.first && j == chosenSquare.second)
							{
								selected = !selected;
								break;
							}

							if (!selected || (selected && (i != chosenSquare.first || j != chosenSquare.second)))
							{
								selected = !selected;
								chosenSquare = { i, j };
							}
						}
					}
				break;
			case sf::Event::MouseButtonReleased: // Tha chuot trai
				if (event.key.code != sf::Mouse::Left)
				{
					break;
				}
				selected != selected;
				moved = false;
				sf::Vector2f p = sf::Vector2f(sf::Mouse::getPosition(game)) + sf::Vector2f(32, 32);
				newPos = sf::Vector2f(64 * int(p.x / 64), 64 * int(p.y / 64));

				int newRow = (newPos.y - 44) / 64;
				int newCol = (newPos.x - 44) / 64;

				if (this->board.canMove(board_x, board_y, newRow, newCol, this->whiteTurn)) {
					this->board.isCaptureMove()? playMusic(3) : playMusic(2);
					this->recordMove(board_x, board_y, newRow, newCol, this->board.getMoveType());
					board.makeMove(board_x, board_y, newRow, newCol, this->whiteTurn);
					if (this->board.getPiece(newRow, newCol)) {
						if ((this->board.getPiece(newRow, newCol)->isWhite() && newRow == 0) || (!this->board.getPiece(newRow, newCol)->isWhite() && newRow == 7)) {
							if (this->board.getPiece(newRow, newCol)->getPiece() == 'P') {
								char chosen = this->promoteWindow();
								std::cout << chosen << endl;
								this->board.promotePawn(newRow, newCol, chosen);

								std::string move = this->moves.back();
								move += chosen;
								this->moves.pop_back();
								this->moves.push_back(move);

							}
						}
					}
					this->whiteTurn = !this->whiteTurn;
				}

				break;
			}
		}

		game.clear();
		game.draw(interfaceBackground_spr);

		if (selected)
		{
			sq_spr[1].setPosition(44 + 64 * board_y, 44 + 64 * board_x);
			game.draw(sq_spr[1]);
			std::vector<std::pair<int, int>> possibleSquares = this->board.getPossibleMoves(board_x, board_y);
			char piece = 0;
			if (this->board.getPiece(board_x, board_y)) {
				piece = this->board.getPiece(board_x, board_y)->getPiece();
			}
			this->drawPossibleMoves(game, sq_spr, possibleSquares, piece);

		}

		(undo_redo_spr[0].getGlobalBounds().contains(pos.x, pos.y) || undo_redo_spr[2].getGlobalBounds().contains(pos.x, pos.y)) ? game.draw(undo_redo_spr[2]) : game.draw(undo_redo_spr[0]);

		(undo_redo_spr[1].getGlobalBounds().contains(pos.x, pos.y) || undo_redo_spr[3].getGlobalBounds().contains(pos.x, pos.y)) ? game.draw(undo_redo_spr[3]) : game.draw(undo_redo_spr[1]);

		this->soundOn ? game.draw(vol_spr[0]) : game.draw(vol_spr[1]);

		this->whiteTurn ? game.draw(turn_spr[0]) : game.draw(turn_spr[1]);

		(save_spr[0].getGlobalBounds().contains(pos.x, pos.y) || save_spr[1].getGlobalBounds().contains(pos.x, pos.y)) ? game.draw(save_spr[1]) : game.draw(save_spr[0]);

		(close_spr[0].getGlobalBounds().contains(pos.x, pos.y) || close_spr[1].getGlobalBounds().contains(pos.x, pos.y)) ? game.draw(close_spr[1]) : game.draw(close_spr[0]);

		(resign_spr[0].getGlobalBounds().contains(pos.x, pos.y) || resign_spr[1].getGlobalBounds().contains(pos.x, pos.y)) ? game.draw(resign_spr[1]) : game.draw(resign_spr[0]);


		board.draw(game);
		game.display();
		if (!whiteTurn) {
			Sleep(1200);
		}
	}

	if (!this->board.isChecked(whiteTurn))
	{
		this->resultWindow(0);
		game.clear();
		return 0;
	}

	whiteTurn ? choiceResult = this->resultWindow(-1) : choiceResult = this->resultWindow(1);
	if (choiceResult == 1)
	{
		replayGame(game, 1);
	}
	return 1;
}

int Game::playWithHuman(sf::RenderWindow& game, bool checkLoad)
{
	game.clear();
	int board_x = 0, board_y = 0;
	bool moved = false;
	bool startB_ib = false;

	sf::Texture close[2];
	sf::Texture resign[2];

	sf::Sprite close_spr[2];
	sf::Sprite resign_spr[2];
	sf::Texture turn[2];
	sf::Texture volume[2];
	sf::Texture save[2];
	sf::Texture undo_redo[4];
	sf::Sprite turn_spr[2];
	sf::Sprite vol_spr[2];
	sf::Sprite save_spr[2];
	sf::Sprite undo_redo_spr[4];
	sf::Vector2f oldPos, newPos, offset(16, 16);

	for (int i = 0; i < 2; ++i)
	{
		close[i].loadFromFile("sprites/resign.png", sf::IntRect(100 * i, 241, 100, 100));
		resign[i].loadFromFile("sprites/resign.png", sf::IntRect(0, 120 * i, 280, 120));

		close_spr[i].setTexture(close[i]);
		resign_spr[i].setTexture(resign[i]);

		resign_spr[i].setPosition(672, 630);
		close_spr[i].setPosition(840, 45);


		undo_redo[i].loadFromFile("sprites/undo_redo.png", sf::IntRect(130 * i, 0, 130, 110));
		undo_redo[i + 2].loadFromFile("sprites/undo_redo.png", sf::IntRect(130 * i, 110, 130, 110));

		undo_redo_spr[i].setTexture(undo_redo[i]);
		undo_redo_spr[i + 2].setTexture(undo_redo[i + 2]);

		undo_redo_spr[i * 2].setPosition(650, 510);
		undo_redo_spr[1 + i * 2].setPosition(830, 510);

		turn[i].loadFromFile("sprites/sound_turn.png", sf::IntRect(0, 150 * i, 350, 150));
		turn_spr[i].setTexture(turn[i]);
		turn_spr[i].setPosition(640, 170);

		volume[i].loadFromFile("sprites/sound_turn.png", sf::IntRect(i * 100, 300, 100, 100));
		vol_spr[i].setTexture(volume[i]);
		vol_spr[i].setPosition(680, 45);

		save[i].loadFromFile("sprites/save.png", sf::IntRect(0, 260 + i * 130, 400, 130));
		save_spr[i].setTexture(save[i]);
		save_spr[i].setPosition(615, 345);
	}
	sf::Texture interfaceBackground;
	sf::Sprite interfaceBackground_spr;
	interfaceBackground.loadFromFile("sprites/board_int.png");
	interfaceBackground_spr.setTexture(interfaceBackground);

	if (checkLoad)
	{
		outfile.open("moves.txt", ios::app);
	}
	else
	{
		outfile.open("moves.txt", ios::out);
		outfile << "H" << endl;
	}

	bool selected = false;
	std::pair<int, int> chosenSquare(-1, -1);
	sf::Texture posSquare;
	sf::Texture selSquare;
	sf::Texture squares[5];
	sf::Sprite sq_spr[5];

	for (int i = 0; i < 5; ++i)
	{
		squares[i].loadFromFile("sprites/sq_status.png", sf::IntRect(0, 64 * i, 64, 64));
		sq_spr[i].setTexture(squares[i]);
	}

	posSquare.loadFromFile("sprites/sq_possible.png");
	selSquare.loadFromFile("sprites/sq_present.png");

	sf::Sprite posSq_spr;
	sf::Sprite selSq_spr;
	posSq_spr.setTexture(posSquare);
	selSq_spr.setTexture(selSquare);
	int choiceResult;

	while (game.isOpen())
	{
		if (!this->board.canMoveNext(whiteTurn))
		{
			break;
		}

		sf::Event event;
		sf::Vector2i pos = sf::Mouse::getPosition(game);

		while (game.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				game.close();
				outfile.close();
				return 0;
			case sf::Event::MouseButtonPressed: //Nhan chuot trai
				if (event.key.code != sf::Mouse::Left)
				{
					break;
				}

				if (close_spr[0].getGlobalBounds().contains(pos.x, pos.y) || close_spr[1].getGlobalBounds().contains(pos.x, pos.y))
				{
					game.clear();

					return 0;
				}


				if (resign_spr[0].getGlobalBounds().contains(pos.x, pos.y) || resign_spr[1].getGlobalBounds().contains(pos.x, pos.y))
				{
					whiteTurn ? choiceResult = this->resultWindow(-1) : choiceResult = this->resultWindow(1);
					if (choiceResult == 1)
					{
						replayGame(game, 1);
						game.clear();
					}

					return 0;
				}

				if (undo_redo_spr[0].getGlobalBounds().contains(pos.x, pos.y) || undo_redo_spr[2].getGlobalBounds().contains(pos.x, pos.y))
				{
					this->undo();

					break;
				}

				if (undo_redo_spr[1].getGlobalBounds().contains(pos.x, pos.y) || undo_redo_spr[3].getGlobalBounds().contains(pos.x, pos.y))
				{
					this->redo();

					break;
				}

				if (save_spr[0].getGlobalBounds().contains(pos.x, pos.y) || save_spr[1].getGlobalBounds().contains(pos.x, pos.y))
				{
					saveGame();
					return 3;
				}

				if (vol_spr[0].getGlobalBounds().contains(pos.x, pos.y) || vol_spr[1].getGlobalBounds().contains(pos.x, pos.y))
				{
					this->setSound(!this->soundOn);

					break;
				}

				for (int i = 0; i < 8; ++i)
					for (int j = 0; j < 8; ++j)
					{
						if (!board.getPiece(i, j))
						{
							continue;
						}

						if (board.getPiece(i, j)->getSprite().getGlobalBounds().contains(pos.x, pos.y))
						{
							if (this->board.canMove(board_x, board_y, i, j, this->whiteTurn)) {
								break;
							}

							moved = true;
							board_x = i;
							board_y = j;
							oldPos = board.getPiece(board_x, board_y)->getSprite().getPosition();

							if (board.getPiece(board_x, board_y)->isWhite() != whiteTurn)
							{
								selected = false;
								break;
							}

							if (selected && i == chosenSquare.first && j == chosenSquare.second)
							{
								selected = !selected;
								break;
							}

							if (!selected || (selected && (i != chosenSquare.first || j != chosenSquare.second)))
							{
								selected = !selected;
								chosenSquare = { i, j };
							}

							break;
						}
					}
				break;
			case sf::Event::MouseButtonReleased: // Tha chuot trai
				if (event.key.code != sf::Mouse::Left)
				{
					break;
				}
				selected != selected;
				sf::Vector2f p = sf::Vector2f(sf::Mouse::getPosition(game));
				newPos = sf::Vector2f(64 * int(p.x / 64), 64 * int(p.y / 64));

				int newRow = (newPos.y - 44) / 64;
				int newCol = (newPos.x - 44) / 64;

				if (this->board.canMove(board_x, board_y, newRow, newCol, this->whiteTurn)) {
					this->canRedo = false;
					this->whiteTurn = !this->whiteTurn;
					this->recordMove(board_x, board_y, newRow, newCol, this->board.getMoveType());
					board.makeMove(board_x, board_y, newRow, newCol, this->whiteTurn);
					this->board.isCaptureMove() ? playMusic(3) : playMusic(2);

					if (this->board.getPiece(newRow, newCol)) {
						if ((this->board.getPiece(newRow, newCol)->isWhite() && newRow == 0) || (!this->board.getPiece(newRow, newCol)->isWhite() && newRow == 7)) {
							if (this->board.getPiece(newRow, newCol)->getPiece() == 'P')
							{
								char piece;
								this->board.promotePawn(newRow, newCol, piece = this->promoteWindow());

								std::string move = this->moves.back();
								move += piece;
								this->moves.pop_back();
								this->moves.push_back(move);

								std::cout << "\n" << move << "\n";
							}
						}
					}

					std::cout << "Move made: " + this->moves.back() << "\n";
				}

				break;
			}
		}

		game.clear();
		game.draw(interfaceBackground_spr);

		if (selected)
		{
			sq_spr[1].setPosition(44 + 64 * board_y, 44 + 64 * board_x);
			game.draw(sq_spr[1]);

			if (this->board.isChecked(whiteTurn))
			{
				for (int i = 0; i < 8; ++i)
					for (int j = 0; j < 8; ++j)
						if (this->board.getPiece(i, j))
							if (this->board.getPiece(i, j)->getPiece() == 'K' && this->board.getPiece(i, j)->isWhite() == whiteTurn)
							{
								sf::Sprite tmp(sq_spr[3]);
								tmp.setPosition(44 + 64 * j, 44 + 64 * i);
								game.draw(tmp);

								break;
							}
			}

			std::vector<std::pair<int, int>> possibleSquares = this->board.getPossibleMoves(board_x, board_y);
			char piece = 0;
			if (this->board.getPiece(board_x, board_y)) {
				piece = this->board.getPiece(board_x, board_y)->getPiece();
			}
			this->drawPossibleMoves(game, sq_spr, possibleSquares, piece);

		}

		this->soundOn ? game.draw(vol_spr[0]) : game.draw(vol_spr[1]);

		this->whiteTurn ? game.draw(turn_spr[0]) : game.draw(turn_spr[1]);

		(undo_redo_spr[0].getGlobalBounds().contains(pos.x, pos.y) || undo_redo_spr[2].getGlobalBounds().contains(pos.x, pos.y)) ? game.draw(undo_redo_spr[2]) : game.draw(undo_redo_spr[0]);

		(undo_redo_spr[1].getGlobalBounds().contains(pos.x, pos.y) || undo_redo_spr[3].getGlobalBounds().contains(pos.x, pos.y)) ? game.draw(undo_redo_spr[3]) : game.draw(undo_redo_spr[1]);

		(save_spr[0].getGlobalBounds().contains(pos.x, pos.y) || save_spr[1].getGlobalBounds().contains(pos.x, pos.y)) ? game.draw(save_spr[1]) : game.draw(save_spr[0]);

		(close_spr[0].getGlobalBounds().contains(pos.x, pos.y) || close_spr[1].getGlobalBounds().contains(pos.x, pos.y)) ? game.draw(close_spr[1]) : game.draw(close_spr[0]);

		(resign_spr[0].getGlobalBounds().contains(pos.x, pos.y) || resign_spr[1].getGlobalBounds().contains(pos.x, pos.y)) ? game.draw(resign_spr[1]) : game.draw(resign_spr[0]);

		board.draw(game);
		game.display();
	}
	if (!this->board.isChecked(whiteTurn))
	{
		this->resultWindow(0);
		game.clear();
		return 0;
	}
	whiteTurn ? choiceResult = this->resultWindow(-1) : choiceResult = this->resultWindow(1);
	if (choiceResult == 1)
	{
		replayGame(game, 1);
		game.clear();
	}
	return 1;
}

void Game::replayGame(sf::RenderWindow& game, int mode)
{
	game.clear();
	sf::Vector2f oldPos, newPos, offset(16, 16);
	sf::Texture interfaceBackground;
	sf::Sprite interfaceBackground_spr;
	interfaceBackground.loadFromFile("sprites/board_int.png");
	interfaceBackground_spr.setTexture(interfaceBackground);

	board.setBoard();

	board.draw(game);
	int i = 0;
	this->whiteTurn = true;


	while (game.isOpen() && i < this->moves.size())
	{
		sf::Event event;
		sf::Vector2i pos = sf::Mouse::getPosition(game) - sf::Vector2i(offset);

		while (game.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				game.close();
				return;

			}
		}

		int o_row, o_col, n_row, n_col;
		o_row = moves[i][0] - '0';
		o_col = moves[i][2] - '0';
		n_row = moves[i][4] - '0';
		n_col = moves[i][6] - '0';

		cout << "whiteTurn: " << this->whiteTurn << endl;
		cout << o_row << " " << o_col << " " << " " << n_row << " " << n_col << endl;
		cout << "Piece: " << this->board.getPiece(o_row, o_col)->getPiece() << " " << this->board.getPiece(o_row, o_col)->isWhite() << endl;

		if (this->board.canMove(o_row, o_col, n_row, n_col, this->whiteTurn)) {
			cout << "Move!\n";
			cout << o_row << " " << o_col << " " << n_row << " " << n_col << endl;
			if (mode == 1)
			{
				Sleep(500);
			}

			board.makeMove(o_row, o_col, n_row, n_col, this->whiteTurn);
			int m_Type = this->board.moveType;
			cout << "Move Type: " << m_Type << endl;
			if (m_Type == 5 || m_Type == 6)
			{
				char piece = moves[i][10];

				this->board.promotePawn(n_row, n_col, piece);
			}
			this->whiteTurn = !this->whiteTurn;
			

		}
		i++;
		game.clear();
		game.draw(interfaceBackground_spr);
		board.draw(game);
		game.display();
		if (mode == 1)
		{
			Sleep(700);
		}
	}


}

void Game::setSound(bool setSound)
{
	this->soundOn = setSound;
	!isMenu ? setSound ? this->soundTrack[0].second.play() : this->soundTrack[0].second.pause() : setSound ? this->soundTrack[1].second.play() : this->soundTrack[1].second.pause();
}

void Game::saveGame()
{
	cout << "Save game!\n";
	for (int i = 0; i < moves.size(); i++)
	{
		outfile << moves[i] << endl;
		cout << moves[i] << endl;
	}
	outfile.close();
}

void Game::loadGame(sf::RenderWindow& game)
{
	game.clear();
	sf::Vector2f oldPos, newPos, offset(16, 16);
	sf::Texture interfaceBackground;
	sf::Sprite interfaceBackground_spr;
	interfaceBackground.loadFromFile("sprites/board_int.png");
	interfaceBackground_spr.setTexture(interfaceBackground);

	this->whiteTurn = true;

	board.setBoard();

	board.draw(game);
	bool checkMove = false;
	ifstream infile;
	infile.open("moves.txt");
	infile.seekg(0, ios::end);
	if (infile.tellg() == 0) {
		std::cout << "No move!\n";
		checkMove = true;
		return;
	}

	infile.seekg(0, ios::beg);

	char old_row, old_col, new_row, new_col, moveType, has_moved;

	char play;
	infile >> play;
	cout << "Replay Play: " << play << endl;

	while (game.isOpen() && infile >> old_row >> old_col >> new_row >> new_col >> moveType >> has_moved && checkMove == false)
	{
		sf::Event event;
		sf::Vector2i pos = sf::Mouse::getPosition(game) - sf::Vector2i(offset);

		while (game.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				game.close();
				return;

			}
		}
		int o_row, o_col, n_row, n_col;
		o_row = old_row - '0';
		o_col = old_col - '0';
		n_row = new_row - '0';
		n_col = new_col - '0';

		if (this->board.canMove(o_row, o_col, n_row, n_col, this->whiteTurn)) {
			cout << "Move!\n";
			cout << o_row << " " << o_col << " " << n_row << " " << n_col << endl;

			board.makeMove(o_row, o_col, n_row, n_col, this->whiteTurn);
			int m_Type = this->board.moveType;
			cout << "Move Type: " << m_Type << endl;
			if (m_Type == 5 || m_Type == 6)
			{
				char piece;
				infile >> piece;
				cout << "piece: " << piece << endl;
				this->board.promotePawn(n_row, n_col, piece);
			}
			this->whiteTurn = !this->whiteTurn;

		}

		game.clear();
		game.draw(interfaceBackground_spr);

		board.draw(game);
		game.display();
	}

	infile.close();
	if (play == 'H')
	{
		playWithHuman(game, true);
		this->moves.clear();
		this->redoMoves.clear();
		this->canRedo = false;
	}
	if (play == 'P')
	{
		playWithPC(game, true);
		this->moves.clear();
		this->redoMoves.clear();
		this->canRedo = false;
	}
}


