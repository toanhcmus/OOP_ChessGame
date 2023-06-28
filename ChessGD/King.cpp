#include "Piece.h"


void King::setCanCastle(const bool& in_canCastle)
{
    m_canCastle = in_canCastle;
}

char King::getPiece() {
    return 'K';
}

bool King::isValidMove(Piece* board[8][8], int32_t rowSrc, int32_t colSrc, int32_t rowDest, int32_t colDest)
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

    int iRowDelta = rowDest - rowSrc;
    int iColDelta = colDest - colSrc;

    if (((iRowDelta >= -1) && (iRowDelta <= 1)) && ((iColDelta >= -1) && (iColDelta <= 1)))
    {
        return true;
    }
    return false;
}