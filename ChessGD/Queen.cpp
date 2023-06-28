#include "Piece.h"


char Queen::getPiece() {
    return 'Q';
}

bool Queen::isValidMove(Piece* board[8][8], int32_t rowSrc, int32_t colSrc, int32_t rowDest, int32_t colDest)
{
    do
    {
        if (!board[rowDest][colDest])
        {
            break;
        }

        if ((this->isWhite() && board[rowDest][colDest]->isWhite()) || (!this->isWhite() && !board[rowDest][colDest]->isWhite()))
        {
            return false;
        }
    } while (false);

    if (std::abs(rowDest - rowSrc) == std::abs(colDest - colSrc)) {   // hang cheo
        int directX = (rowDest - rowSrc > 0) ? 1 : -1;
        int directY = (colDest - colSrc > 0) ? 1 : -1;
        for (int rowi = rowSrc + directX, coli = colSrc + directY; rowi != rowDest; rowi += directX, coli += directY) {
            if (board[rowi][coli] != nullptr) {
                return false;
            }
        }
        return true;
    }

    if (rowDest == rowSrc) {  // cung dong
        int directX = (colDest - colSrc > 0) ? 1 : -1;
        for (int coli = colSrc + directX; coli != colDest; coli += directX) {
            if (board[rowSrc][coli] != nullptr) {
                return false;
            }
        }
        return true;
    }

    if (colDest == colSrc) {	// cung cot
        int directY = (rowDest - rowSrc > 0) ? 1 : -1;
        for (int rowi = rowSrc + directY; rowi != rowDest; rowi += directY) {
            if (board[rowi][colSrc] != nullptr) {
                return false;
            }
        }
        return true;
    }
    return false;
}