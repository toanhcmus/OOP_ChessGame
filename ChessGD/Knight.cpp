#include "Piece.h"


bool Knight::isValidMove(Piece* board[8][8], int32_t rowSrc, int32_t colSrc, int32_t rowDest, int32_t colDest)
{
    Piece* dest = board[rowDest][colDest];
    if (!dest) {
        if ((colSrc == colDest + 1) || (colSrc == colDest - 1)) {
            if ((rowSrc == rowDest + 2) || (rowSrc == rowDest - 2)) {
                return true;
            }
        }
        if ((colSrc == colDest + 2) || (colSrc == colDest - 2)) {
            if ((rowSrc == rowDest + 1) || (rowSrc == rowDest - 1)) {
                return true;
            }
        }
    }
    else {
        if ((colSrc == colDest + 1) || (colSrc == colDest - 1)) {
            if ((rowSrc == rowDest + 2) || (rowSrc == rowDest - 2)) {
                if (dest->isWhite() != this->isWhite()) {
                    return true;
                }
            }
        }
        if ((colSrc == colDest + 2) || (colSrc == colDest - 2)) {
            if ((rowSrc == rowDest + 1) || (rowSrc == rowDest - 1)) {
                if (dest->isWhite() != this->isWhite()) {
                    return true;
                }
            }
        }
    }
    return false;
}


char Knight::getPiece() {
    return 'N';
}