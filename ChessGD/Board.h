#pragma once

#include <iostream>
#include <stdint.h>
#include "Piece.h"
#include <vector>
#include <Windows.h>
#include <utility>

enum MoveType {
    normal = 0,
    capture = 1,
    enPassant,
    longCastling,
    shortCastling,
    promote,
    capture_promote,
};


class Square {
    int x;
    int y;
public:
    Square() : x(0), y(0) {};
    Square(int x_, int y_) : x(x_), y(y_) {};
    int getX() { return x; }
    int getY() { return y; }
    void setX(int value) { x = value; }
    void setY(int value) { y = value; }
    ~Square() {};
};


class Board
{
private:
    Piece* board[8][8];
    std::vector<Piece*> whitePieces;
    std::vector<Piece*> blackPieces;
    std::vector<Piece*> captured;
    std::vector<Piece*> l_promote;

public:
    MoveType moveType;
    bool hasCapture;
    Board();

    void setBoard();

    void draw(sf::RenderWindow& game);
    MoveType getMoveType();

    Piece* getPiece(int x, int y) const;

    bool canMove(int old_row, int old_column, int new_row, int new_col, bool whiteTurn);
    void makeMove(int old_row, int old_column, int new_row, int new_col, bool whiteTurn);

    bool checkKingSquare(int x, int y);
    bool checkRookSquare(int x, int y); 
    bool canCastle(const bool& isKingSide, bool whiteTurn);
    bool isChecked(bool whiteTurn);
    int findAnotherMove(int old_row, int old_column, int new_row, int new_col, bool whiteTurn);
    bool canMoveNext(bool whiteTurn);
    vector<pair<int, int>> getPossibleMoves(int row, int column);
    bool promotePawn(int32_t row, int32_t col, char piece);
    bool isCaptureMove();
    vector<Piece*> getBlackPieces();
    Square getSquare(Piece* piece);
    void printPieces(bool whiteTurn);
    void undo(int old_row, int old_col, int new_row, int new_col, MoveType moveType, bool whiteTurn);
    ~Board();
};