#include "Piece.h"

bool Bishop::isValidMove(Piece* board[8][8], int32_t rowSrc, int32_t colSrc, int32_t rowDest, int32_t colDest)
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

    if (std::abs(rowDest - rowSrc) == std::abs(colDest - colSrc)) {
        int direcX = (rowDest - rowSrc > 0) ? 1 : -1;
        int directY = (colDest - colSrc > 0) ? 1 : -1;
        for (int rowi = rowSrc + direcX, coli = colSrc + directY; rowi != rowDest; rowi = rowi + direcX, coli = coli + directY) {
            if (board[rowi][coli] != nullptr) {
                return false;
            }
        }
        return true;
    }
    return false;
}

char  Bishop::getPiece() {
    return 'B';
}

