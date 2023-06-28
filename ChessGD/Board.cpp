#include "Board.h"
#include "Game.h"
#include <sstream>
#include <SFML/Graphics.hpp>

Board::Board()
{
	for (int32_t i = 0; i < 8; ++i)
	{
		this->board[1][i] = new Pawn(Color::black);
		this->board[6][i] = new Pawn(Color::white);

		this->blackPieces.push_back(this->board[1][i]);
		this->whitePieces.push_back(this->board[6][i]);
	}

	this->board[0][0] = new Rook(Color::black);
	this->board[0][7] = new Rook(Color::black);

	this->board[0][1] = new Knight(Color::black);
	this->board[0][6] = new Knight(Color::black);

	this->board[0][2] = new Bishop(Color::black);
	this->board[0][5] = new Bishop(Color::black);

	this->board[0][3] = new Queen(Color::black);
	this->board[0][4] = new King(Color::black);

	this->board[7][0] = new Rook(Color::white);
	this->board[7][7] = new Rook(Color::white);

	this->board[7][1] = new Knight(Color::white);
	this->board[7][6] = new Knight(Color::white);

	this->board[7][2] = new Bishop(Color::white);
	this->board[7][5] = new Bishop(Color::white);

	this->board[7][3] = new Queen(Color::white);
	this->board[7][4] = new King(Color::white);

	for (int i = 0; i < 8; ++i)
	{
		this->blackPieces.push_back(this->board[0][i]);
		this->whitePieces.push_back(this->board[7][i]);
	}

	for (int i = 2; i < 6; ++i)
		for (int j = 0; j < 8; ++j)
			this->board[i][j] = nullptr;

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (this->board[i][j] != nullptr) {
				this->board[i][j]->setHasMoved(false); 
			}
		}
	}
}

void Board::setBoard() {
	for (std::vector<Piece*>::iterator i = this->blackPieces.begin(); i != this->blackPieces.end(); ++i)
	{
		Piece* tmp = *i;
		delete tmp;
	}
	this->blackPieces.clear();
	std::cout << this->blackPieces.size() << "All cleared\n";
	for (std::vector<Piece*>::iterator i = this->whitePieces.begin(); i != this->whitePieces.end(); ++i)
	{
		Piece* tmp = *i;
		delete tmp;
	}
	this->whitePieces.clear();
	std::cout << this->whitePieces.size() << "All cleared\n";
	for (std::vector<Piece*>::iterator i = this->captured.begin(); i != this->captured.end(); ++i)
	{
		Piece* tmp = *i;
		delete tmp;
	}
	this->captured.clear();
	std::cout << this->captured.size() << "All cleared\n";
	for (std::vector<Piece*>::iterator i = this->l_promote.begin(); i != this->l_promote.end(); ++i)
	{
		Piece* tmp = *i;
		delete tmp;
	}
	std::cout << this->l_promote.size() << "All cleared\n";
	this->l_promote.clear();
	for (int i = 0; i < 8; ++i)
	{
		for (int j = 0; j < 8; ++j)
		{
			this->board[i][j] = nullptr;
		}
	}

	for (int32_t i = 0; i < 8; ++i)
	{
		this->board[1][i] = new Pawn(Color::black);
		this->board[6][i] = new Pawn(Color::white);

		this->blackPieces.push_back(this->board[1][i]);
		this->whitePieces.push_back(this->board[6][i]);
	}

	this->board[0][0] = new Rook(Color::black);
	this->board[0][7] = new Rook(Color::black);

	this->board[0][1] = new Knight(Color::black);
	this->board[0][6] = new Knight(Color::black);

	this->board[0][2] = new Bishop(Color::black);
	this->board[0][5] = new Bishop(Color::black);

	this->board[0][3] = new Queen(Color::black);
	this->board[0][4] = new King(Color::black);

	this->board[7][0] = new Rook(Color::white);
	this->board[7][7] = new Rook(Color::white);

	this->board[7][1] = new Knight(Color::white);
	this->board[7][6] = new Knight(Color::white);

	this->board[7][2] = new Bishop(Color::white);
	this->board[7][5] = new Bishop(Color::white);

	this->board[7][3] = new Queen(Color::white);
	this->board[7][4] = new King(Color::white);

	for (int i = 0; i < 8; ++i)
	{
		this->blackPieces.push_back(this->board[0][i]);
		this->whitePieces.push_back(this->board[7][i]);
	}

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (this->board[i][j] != nullptr) {
				this->board[i][j]->setHasMoved(false);  
			}
		}
	}
}

void Board::draw(sf::RenderWindow& game)
{
	for (int i = 0; i < 8; ++i)
	{
		for (int j = 0; j < 8; ++j)
		{
			if (!this->board[i][j])
			{
				continue;
			}

			this->board[i][j]->setSpritePos(44 + 64 * j, 44 + 64 * i);

			game.draw(this->board[i][j]->getSprite());
		}
	}

	int pos_x_white = 28;
	int pos_x_black = 28;

	for (auto i = this->captured.begin(); i != this->captured.end(); ++i)
	{
		if ((*i)->isWhite())
		{
			(*i)->setSpritePos(pos_x_white, 644);
			pos_x_white += 32;
			game.draw((*i)->getSprite());
			continue;
		}

		(*i)->setSpritePos(pos_x_black, 720);
		pos_x_black += 32;
		game.draw((*i)->getSprite());
	}
}

MoveType Board::getMoveType()
{
	return this->moveType;
}

Piece* Board::getPiece(int x, int y) const
{
	if (x < 0 || x > 7 || y < 0 || y > 7)
	{
		return nullptr;
	}

	return this->board[x][y];
}

bool Board::canMove(int old_row, int old_col, int new_row, int new_col, bool whiteTurn)
{
	if (new_row > 7 || new_row < 0 || new_col > 7 || new_col < 0 || (old_row == new_row && old_col == new_col))
	{
		return false;
	}

	Piece* piece = this->getPiece(old_row, old_col);

	if (!piece)
	{
		return false;
	}

	if ((piece->isWhite() != whiteTurn))
	{
		return false;
	}

	if (((checkKingSquare(old_row, old_col) && checkRookSquare(new_row, new_col)) || (checkRookSquare(old_row, old_col) && checkKingSquare(new_row, new_col)))) {
		
		if ((checkRookSquare(old_row, old_col) && checkKingSquare(new_row, new_col))) {
			std::swap(old_row, new_row);
			std::swap(old_col, new_col);
		}
		if (board[old_row][old_col]->isWhite() == board[new_row][new_col]->isWhite()) {
			if (this->isChecked(whiteTurn)) {
				return false;
			}
			if ((old_row == 0 && old_col == 4 && new_row == old_row && (new_col == 0 || new_col == 7)) || (old_row == 7 && old_col == 4 && new_row == old_row && (new_col == 0 || new_col == 7))) {
				int distance = std::abs(old_col - new_col);
				bool isKingSide = (distance == 3) ? true : false;
				int emptyColKing = (isKingSide == true) ? 6 : 2;
				int emptyColRook = (isKingSide == true) ? 5 : 3;
				if (this->canCastle(isKingSide, whiteTurn)) {
					moveType = (isKingSide == true) ? shortCastling : longCastling;
					int emptyColKing = (moveType == shortCastling) ? 6 : 2;
					int emptyColRook = (moveType == shortCastling) ? 5 : 3;

					this->board[new_row][emptyColKing] = this->board[old_row][old_col]; // di chuyen King
					this->board[old_row][old_col] = nullptr;
					this->board[new_row][emptyColKing]->setHasMoved(true);

					this->board[new_row][emptyColRook] = this->board[new_row][new_col]; // di chuyen Rook
					this->board[new_row][new_col] = nullptr;
					this->board[new_row][emptyColRook]->setHasMoved(true);

					bool isInCheck = this->isChecked(whiteTurn); 

					this->board[old_row][old_col] = this->board[new_row][emptyColKing]; // undo King
					this->board[new_row][emptyColKing] = nullptr;
					this->board[old_row][old_col]->setHasMoved(false);

					this->board[new_row][new_col] = this->board[new_row][emptyColRook]; // di chuyen Rook
					this->board[new_row][emptyColRook] = nullptr;
					this->board[new_row][new_col]->setHasMoved(false);
					return !isInCheck;
				}
				return false;
			}
		}
	}

	if (!piece->isValidMove(this->board, old_row, old_col, new_row, new_col))
	{
		return false;
	}

	if (piece->getPiece() == 'P') {
		if ((old_col == new_col + 1 || old_col == new_col - 1) && (old_row == new_row + 1 || old_row == new_row - 1)) {
			Piece* besidePiece = this->board[old_row][new_col];
			if (besidePiece) {
				if (besidePiece->getPiece() == 'P') {
					if (besidePiece->isWhite() != board[old_row][old_col]->isWhite()) {
						if (static_cast<Pawn*>(besidePiece)->canEnpassant()) {
							moveType = enPassant;
							Piece* pieceDestTemp = board[new_row][new_col];
							this->board[new_row][new_col] = this->board[old_row][old_col]; // di chuyển thử
							this->board[old_row][old_col] = this->board[old_row][new_col] = nullptr;
							bool isInCheck = this->isChecked(whiteTurn);  //check thử xem nếu đi nước này thì King của mình có bị chiếu hay không

							this->board[old_row][old_col] = this->board[new_row][new_col]; // undo bước di chuyển thử
							this->board[new_row][new_col] = pieceDestTemp;
							this->board[old_row][new_col] = besidePiece;
							return !isInCheck;
						}
					}
				}
			}
		}
	}

	this->board[new_row][new_col] ? moveType = capture : moveType = normal;

	if (piece) {
		if (this->board[old_row][old_col]->getPiece() == 'P') {
			if ((board[old_row][old_col]->isWhite() && new_row == 0) || (!board[old_row][old_col]->isWhite() && new_row == 7)) {
				moveType = promote;
				if (new_col == old_col - 1 || new_col == old_col + 1) {
					moveType = capture_promote;
				}
			}
		}
	}

	Piece* pieceDestTemp = board[new_row][new_col];
	this->board[new_row][new_col] = this->board[old_row][old_col]; // di chuyển thử
	this->board[old_row][old_col] = nullptr;
	bool isInCheck = this->isChecked(whiteTurn);  //check thử xem nếu đi nước này thì King của mình có bị chiếu hay không

	this->board[old_row][old_col] = this->board[new_row][new_col]; // undo bước di chuyển thử
	this->board[new_row][new_col] = pieceDestTemp;
	return !isInCheck;
}

void Board::makeMove(int old_row, int old_col, int new_row, int new_col, bool whiteTurn) {
	for (auto piece : this->whitePieces) {
		if (piece->getPiece() == 'P') {
			static_cast<Pawn*>(piece)->setPosibleEnpassant(false);
		}
	}
	for (auto piece : this->blackPieces) {
		if (piece->getPiece() == 'P') {
			static_cast<Pawn*>(piece)->setPosibleEnpassant(false);
		}
	}
	if (moveType == longCastling || moveType == shortCastling) {
		int emptyColKing = (moveType == shortCastling) ? 6 : 2;
		int emptyColRook = (moveType == shortCastling) ? 5 : 3;

		this->board[new_row][emptyColKing] = this->board[old_row][old_col]; // di chuyen King
		this->board[old_row][old_col] = nullptr;
		this->board[new_row][emptyColKing]->setHasMoved(true);

		this->board[new_row][emptyColRook] = this->board[new_row][new_col]; // di chuyen Rook
		this->board[new_row][new_col] = nullptr;
		this->board[new_row][emptyColRook]->setHasMoved(true);

	}
	else if (moveType == normal || moveType == capture || moveType == promote || moveType == capture_promote) {
		if (this->board[old_row][old_col]->getPiece() == 'P') {
			static_cast<Pawn*>(this->board[old_row][old_col])->setPosibleMoveTwoSquare(false);
			if (old_col == new_col && (old_row == new_row - 2 || old_row == new_row + 2)) {
				static_cast<Pawn*>(this->board[old_row][old_col])->setPosibleEnpassant(true);
			}
		}

		if (this->board[old_row][old_col]->getPiece() == 'K') {
			this->board[old_row][old_col]->setHasMoved(true);
		}
		if (this->board[old_row][old_col]->getPiece() == 'R') {
			this->board[old_row][old_col]->setHasMoved(true);
		}

		if (moveType == capture || moveType == capture_promote) {
			Piece* capturedPiece = board[new_row][new_col];
			std::vector<Piece*>::iterator it;
			if (this->board[new_row][new_col]->isWhite()) {
				it = find(this->whitePieces.begin(), this->whitePieces.end(), capturedPiece);
				whitePieces.erase(it);
			}
			else {
				it = find(this->blackPieces.begin(), this->blackPieces.end(), capturedPiece);
				blackPieces.erase(it);
			}

			this->captured.push_back(capturedPiece);
		}
			
		this->board[new_row][new_col] = this->board[old_row][old_col];
		this->board[old_row][old_col] = nullptr;
	}
	else if (moveType == enPassant) {
		if (board[old_row][new_col]) {
			Piece* capturedPiece = board[old_row][new_col];
			std::vector<Piece*>::iterator it;
			if (this->board[old_row][new_col]->isWhite()) {
				it = find(this->whitePieces.begin(), this->whitePieces.end(), capturedPiece);
				whitePieces.erase(it);

			}
			else {
				it = find(this->blackPieces.begin(), this->blackPieces.end(), capturedPiece);
				blackPieces.erase(it);
			}
			this->captured.push_back(capturedPiece);
			this->board[old_row][new_col] = nullptr;
		}
		this->board[new_row][new_col] = this->board[old_row][old_col];
		this->board[old_row][old_col] = nullptr;
	}
}

vector<pair<int, int>> Board::getPossibleMoves(int row, int column)
{
	Piece* prePiece = board[row][column];
	if (!prePiece)
		return vector<pair<int, int>>();
	bool whiteTurn = prePiece->isWhite();
	vector<pair<int, int>> listPossibleMoves;
	switch (prePiece->getPiece()) {
	case 'P':
		for (int32_t index_row = row - 2; index_row <= row + 2; index_row++) {
			if (index_row == row || index_row < 0 || index_row > 7)
				continue;
			if (canMove(row, column, index_row, column, whiteTurn))
				listPossibleMoves.push_back(make_pair(index_row, column));
		}
		if (canMove(row, column, row - 1, column + 1, whiteTurn))
			listPossibleMoves.push_back(make_pair(row - 1, column + 1));
		if (canMove(row, column, row - 1, column - 1, whiteTurn))
			listPossibleMoves.push_back(make_pair(row - 1, column - 1));
		if (canMove(row, column, row + 1, column + 1, whiteTurn))
			listPossibleMoves.push_back(make_pair(row + 1, column + 1));
		if (canMove(row, column, row + 1, column - 1, whiteTurn))
			listPossibleMoves.push_back(make_pair(row + 1, column - 1));
		return listPossibleMoves;

	case 'R':
		for (int32_t index_row = 0; index_row < 8; index_row++) {
			if (index_row == row)
				continue;
			if (canMove(row, column, index_row, column, whiteTurn))
				listPossibleMoves.push_back(make_pair(index_row, column));
		}
		for (int32_t index_col = 0; index_col < 8; index_col++) {
			if (index_col == column)
				continue;
			if (canMove(row, column, row, index_col, whiteTurn))
				listPossibleMoves.push_back(make_pair(row, index_col));
		}
		return listPossibleMoves;

	case 'N':
		for (int32_t index = 1; index < 3; index++) {
			if (0 <= row - index) {
				if (0 <= column - 3 + index) {
					if (canMove(row, column, row - index, column - 3 + index, whiteTurn))
						listPossibleMoves.push_back(make_pair(row - index, column - 3 + index));
				}
				if (column + 3 - index <= 7) {
					if (canMove(row, column, row - index, column + 3 - index, whiteTurn))
						listPossibleMoves.push_back(make_pair(row - index, column + 3 - index));
				}
			}
			if (row + index <= 7) {
				if (0 <= column - 3 + index) {
					if (canMove(row, column, row + index, column - 3 + index, whiteTurn))
						listPossibleMoves.push_back(make_pair(row + index, column - 3 + index));
				}
				if (column + 3 - index <= 7) {
					if (canMove(row, column, row + index, column + 3 - index, whiteTurn))
						listPossibleMoves.push_back(make_pair(row + index, column + 3 - index));
				}
			}
		}
		return listPossibleMoves;

	case 'B':
		for (int32_t index = -7; index < 8; index++) {
			if (index == 0)
				continue;
			if (0 <= row + index && row + index <= 7) {
				if (0 <= column + index && column + index <= 7) {
					if (canMove(row, column, row + index, column + index, whiteTurn))
						listPossibleMoves.push_back(make_pair(row + index, column + index));
				}
				if (0 <= column - index && column - index <= 7) {
					if (canMove(row, column, row + index, column - index, whiteTurn))
						listPossibleMoves.push_back(make_pair(row + index, column - index));
				}
			}
		}
		return listPossibleMoves;

	case 'Q':
		for (int32_t index_row = 0; index_row < 8; index_row++) {
			if (index_row == row)
				continue;
			if (canMove(row, column, index_row, column, whiteTurn))
				listPossibleMoves.push_back(make_pair(index_row, column));
		}
		for (int32_t index_col = 0; index_col < 8; index_col++) {
			if (index_col == column)
				continue;
			if (canMove(row, column, row, index_col, whiteTurn))
				listPossibleMoves.push_back(make_pair(row, index_col));
		}
		for (int32_t index = -7; index < 8; index++) {
			if (index == 0)
				continue;
			if (0 <= row + index && row + index <= 7) {
				if (0 <= column + index && column + index <= 7) {
					if (canMove(row, column, row + index, column + index, whiteTurn))
						listPossibleMoves.push_back(make_pair(row + index, column + index));
				}
				if (0 <= column - index && column - index <= 7) {
					if (canMove(row, column, row + index, column - index, whiteTurn))
						listPossibleMoves.push_back(make_pair(row + index, column - index));
				}
			}
		}
		return listPossibleMoves;

	case 'K':
		for (int32_t i_row = max(row - 1, 0); i_row <= min(row + 1, 7); i_row++) {
			for (int32_t i_col = 0; i_col <= 7; i_col++) {
				if (i_row == row && i_col == column) {
					continue;
				}
				if (canMove(row, column, i_row, i_col, whiteTurn))
					listPossibleMoves.push_back(make_pair(i_row, i_col));
			}
		}
		return listPossibleMoves;
	}
	return vector<pair<int, int>>();
}

bool Board::promotePawn(int32_t row, int32_t col, char piece)
{
	std::cout << piece << std::endl;
	if (board[row][col]) {
		if (board[row][col]->getPiece() == 'P') {
			if ((row == 0 && board[row][col]->isWhite()) || (row == 7 && !board[row][col]->isWhite())) {
				Piece* temp = board[row][col];
				switch (piece) {
				case 'Q':
					board[row][col] = new Queen((temp->isWhite()) ? Color::white : black);
					break;

				case 'B':
					board[row][col] = new Bishop((temp->isWhite()) ? Color::white : black);
					break;

				case 'R':
					board[row][col] = new Rook((temp->isWhite()) ? Color::white : black);
					break;

				case 'N':
					board[row][col] = new Knight((temp->isWhite()) ? Color::white : black);
					break;

				default:
					return false;
				}

				

				std::vector<Piece*>::iterator it;
				if (board[row][col]->isWhite()) {
					it = find(this->whitePieces.begin(), this->whitePieces.end(), temp);
					whitePieces.erase(it);
					whitePieces.push_back(board[row][col]);
				}
				else {
					it = find(this->blackPieces.begin(), this->blackPieces.end(), temp);
					blackPieces.erase(it);
					blackPieces.push_back(board[row][col]);
				}

				this->l_promote.push_back(temp);

				return true;
			}
		}
	}
	return false;
}

bool Board::isCaptureMove()
{
	return moveType == enPassant || moveType == capture || moveType == capture_promote;
}

vector<Piece*> Board::getBlackPieces()
{
	return this->blackPieces;
}

Square Board::getSquare(Piece* piece)
{
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (this->board[i][j] && this->board[i][j] == piece) {
				return Square(i, j);
			}
		}
	}
	return Square(0, 0);
}

Board::~Board()
{
	for (std::vector<Piece*>::iterator i = this->blackPieces.begin(); i != this->blackPieces.end(); ++i)
	{
		Piece* tmp = *i;
		delete tmp;
	}
	this->blackPieces.clear();
	std::cout << this->blackPieces.size() << "All cleared\n";
	for (std::vector<Piece*>::iterator i = this->whitePieces.begin(); i != this->whitePieces.end(); ++i)
	{
		Piece* tmp = *i;
		delete tmp;
	}
	this->whitePieces.clear();
	std::cout << this->whitePieces.size() << "All cleared\n";
	for (std::vector<Piece*>::iterator i = this->captured.begin(); i != this->captured.end(); ++i)
	{
		Piece* tmp = *i;
		delete tmp;
	}
	this->captured.clear();
	std::cout << this->captured.size() << "All cleared\n";
	for (std::vector<Piece*>::iterator i = this->l_promote.begin(); i != this->l_promote.end(); ++i)
	{
		Piece* tmp = *i;
		delete tmp;
	}
	std::cout << this->l_promote.size() << "All cleared\n";
	this->l_promote.clear();
	for (int i = 0; i < 8; ++i)
	{
		for (int j = 0; j < 8; ++j)
		{
			this->board[i][j] = nullptr;
		}
	}
}

bool Board::checkKingSquare(int x, int y) {
	if (!this->getPiece(x, y)) return false;
	return (board[x][y]->getPiece() == 'K');
}

bool Board::checkRookSquare(int x, int y) {
	if (!this->getPiece(x, y)) return false;
	return (board[x][y]->getPiece() == 'R');
}

bool Board::canCastle(const bool& isKingSide, bool whiteTurn)
{
	int king_col = 4;
	int row = (whiteTurn) ? 7 : 0;
	int rook_col = isKingSide ? 7 : 0;
	int row_offset = isKingSide ? 1 : -1;


	if (this->getPiece(row, king_col)->hasMoved() || this->getPiece(row, rook_col)->hasMoved()) {
		return false;
	}

	for (int col = king_col + row_offset; col != rook_col; col += row_offset) {
		if (board[row][col] != nullptr) {
			return false;
		}
	}

	return true;
}

bool Board::isChecked(bool whiteTurn) {
	int kingRow = 0;
	int kingCol = 0;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if ((checkKingSquare(i, j) && (this->getPiece(i, j)->isWhite() == whiteTurn))) {
				kingRow = i;
				kingCol = j;
				break;
			}
		}
	}
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (!board[i][j])
			{
				continue;
			}

			if (board[i][j]->isWhite() == whiteTurn)
			{
				continue;
			}

			if (this->getPiece(i, j)->isValidMove(board, i, j, kingRow, kingCol)) {
				return true;
			}
		}
	}
	return false;
}

int Board::findAnotherMove(int old_row, int old_column, int new_row, int new_col, bool whiteTurn) { // tìm xem có con nào cùng loại có thể đi đến ô dest ko
	char c = this->getPiece(old_row, old_column)->getPiece();
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (i == old_row && j == old_column) {
				continue;
			}
			if (this->canMove(i, j, new_row, new_col, whiteTurn) && this->getPiece(i, j)->getPiece() == c) {
				if (i == old_row || (i != old_row && j != old_column)) {
					return 1;
				}
				if (j == new_col) {
					return 2;
				}
			}
		}
	}
	return 0;
}

bool Board::canMoveNext(bool whiteTurn) {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (this->getPiece(i, j)) {
				if ((this->getPiece(i, j)->isWhite() == whiteTurn)) {
					for (int i_move = 0; i_move < 8; i_move++) {
						for (int j_move = 0; j_move < 8; j_move++) {
							if (this->canMove(i, j, i_move, j_move, whiteTurn)) {
								return true;
							}
						}
					}
				}
			}
		}
	}
	return false;
}

void Board::printPieces(bool whiteTurn)
{
	std::vector<Piece*> vec;
	whiteTurn ? vec = this->whitePieces : vec = this->blackPieces;

	for (auto i : vec)
	{
		std::cout << i->getPiece() << " ";
	}

	std::cout << "\n";
}

void Board::undo(int old_row, int old_col, int new_row, int new_col, MoveType moveType, bool whiteTurn)
{
	do
	{
		if (moveType != shortCastling)
			break;

		if (!whiteTurn)
		{
			this->board[new_row][new_col] = this->board[7][6];
			this->board[old_row][old_col] = this->board[7][5];
			this->board[old_row][old_col]->setHasMoved(false);
			this->board[new_row][new_col]->setHasMoved(false);

			this->board[7][6] = this->board[7][5] = nullptr;

			return;
		}

		this->board[new_row][new_col] = this->board[0][6];
		this->board[old_row][old_col] = this->board[0][5];
		this->board[old_row][old_col]->setHasMoved(false);
		this->board[new_row][new_col]->setHasMoved(false);

		this->board[0][6] = this->board[0][5] = nullptr;

		return;
	} while (false);

	do
	{
		if (moveType != longCastling)
			break;

		if (!whiteTurn)
		{
			this->board[new_row][new_col] = this->board[7][2];
			this->board[old_row][old_col] = this->board[7][3];
			this->board[old_row][old_col]->setHasMoved(false);
			this->board[new_row][new_col]->setHasMoved(false);

			this->board[7][2] = this->board[7][3] = nullptr;

			return;
		}

		this->board[new_row][new_col] = this->board[0][2];
		this->board[old_row][old_col] = this->board[0][3];
		this->board[old_row][old_col]->setHasMoved(false);
		this->board[new_row][new_col]->setHasMoved(false);

		this->board[0][2] = this->board[0][3] = nullptr;

		return;
	} while (false);

	do
	{
		if (moveType != promote && moveType != capture_promote)
			break;

		delete this->board[old_row][old_col]; //xoa quan thang cap
		this->board[old_row][old_col] = nullptr; //set o bang rong

		if (moveType == capture_promote) //set lai o thang cap bang quan o capture, pop_back()
		{
			this->board[old_row][old_col] = this->captured.back();

			this->captured.back()->isWhite() ? this->whitePieces.insert(this->whitePieces.begin(), this->captured.back()) : this->blackPieces.insert(this->blackPieces.begin(), this->captured.back());
			this->captured.pop_back();
		}


		if (whiteTurn)
		{
			this->blackPieces.pop_back();

			this->board[new_row][new_col] = this->l_promote.back();
			this->blackPieces.insert(this->blackPieces.begin(), this->l_promote.back());
			this->l_promote.pop_back();
			return;
		}

		this->whitePieces.pop_back();

		this->board[new_row][new_col] = this->l_promote.back();
		this->whitePieces.insert(this->whitePieces.begin(), this->l_promote.back());
		this->l_promote.pop_back();
		return;
	} while (false);

	this->board[new_row][new_col] = this->board[old_row][old_col];

	this->board[old_row][old_col] = nullptr;

	if (this->board[new_row][new_col]->getPiece() == 'P')
	{
		if (new_row == 6 && this->board[new_row][new_col]->isWhite())
		{
			static_cast<Pawn*>(this->board[new_row][new_col])->setPosibleMoveTwoSquare(true);
			static_cast<Pawn*>(this->board[new_row][new_col])->setPosibleEnpassant(false);
		}

		if (new_row == 1 && !this->board[new_row][new_col]->isWhite())
		{
			static_cast<Pawn*>(this->board[new_row][new_col])->setPosibleMoveTwoSquare(true);
			static_cast<Pawn*>(this->board[new_row][new_col])->setPosibleEnpassant(false);
		}
	}

	do
	{
		if (moveType != enPassant)
			break;
		this->board[new_row][old_col] = this->captured.back();
		static_cast<Pawn*>(this->board[new_row][old_col])->setPosibleEnpassant(true);

		this->captured.pop_back();

		whiteTurn ? this->whitePieces.insert(this->whitePieces.begin(), this->board[new_row][old_col]) : this->blackPieces.insert(this->blackPieces.begin(), this->board[new_row][old_col]);

		return;
	} while (false);
	
	if (moveType == capture)
	{
		this->board[old_row][old_col] = this->captured.back();
		this->captured.pop_back();

		this->board[old_row][old_col]->isWhite() ? this->whitePieces.insert(this->whitePieces.begin(), this->board[old_row][old_col]) : this->blackPieces.insert(this->blackPieces.begin(), this->board[old_row][old_col]);

		return;
	}
}
