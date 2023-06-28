#include "Piece.h"


char Pawn::getPiece() {
    return 'P';
}

void Pawn::setPosibleEnpassant(bool in_enpassant)
{
    this->posibleEnpassant = in_enpassant;
}

void Pawn::setPosibleMoveTwoSquare(bool in_moveTwoSquare)
{
    this->posibleMoveTwoSquare = in_moveTwoSquare;
}

bool Pawn::canEnpassant()
{
    return (!this->posibleEnpassant) ? false : true;
}

bool Pawn::canMoveTwoSquare()
{
    return this->posibleMoveTwoSquare;
}

bool Pawn::isValidMove(Piece* board[8][8], int32_t rowSrc, int32_t colSrc, int32_t rowDest, int32_t colDest)
{
    Piece* dest = board[rowDest][colDest];
    if (!dest) {
        if (colDest == colSrc) {
            if (this->posibleMoveTwoSquare) {
                if (rowDest == rowSrc - 2) {
                    if (this->isWhite() && !board[rowSrc -1][colDest])
                        return true;
                }
                if (rowDest == rowSrc + 2) {
                    if (!this->isWhite() && !board[rowSrc+1][colDest])
                        return true;
                }
            }
            if (rowDest == rowSrc - 1) {
                if (this->isWhite())
                    return true;
            }
            if (rowDest == rowSrc + 1) {
                if (!this->isWhite())
                    return true;
            }
        }
        if (colDest == colSrc + 1 || colDest == colSrc - 1) {
            if (rowDest == rowSrc + 1 || rowDest == rowSrc - 1) {
                Piece* beside = board[rowSrc][colDest];
                if (beside) {
                    if (beside->getPiece() == 'P' && beside->isWhite() != this->isWhite()) {
                        if (static_cast<Pawn*>(beside)->canEnpassant()) {
                            if (rowDest == rowSrc - 1 && this->isWhite()) {
                                return true;
                            }
                            if (rowDest == rowSrc + 1 && !this->isWhite()) {
                                return true;
                            }
                        }
                    }
                }
            }
        }
    }
    else {
        if ((colSrc == colDest + 1) || (colSrc == colDest - 1)) {
            if (dest->isWhite() != this->isWhite()) {
                if (this->isWhite()) {
                    if (rowDest == rowSrc - 1)
                        return true;
                }
                else {
                    if (rowDest == rowSrc + 1)
                        return true;
                }
                
            }
        }
    }
    return false;
}