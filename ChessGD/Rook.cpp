#include "Piece.h"

bool Rook::checkCanCastling()
{
    return this->canCastle;
}

void Rook::setCanCastle(const bool& in_canCastle)
{
    this->canCastle = in_canCastle;
}

bool Rook::isValidMove(Piece* board[8][8], int32_t rowSrc, int32_t colSrc, int32_t rowDest, int32_t colDest)
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

    if (rowSrc == rowDest) {
        // Make sure that all invervening squares are empty
        int iColOffset = (colDest - colSrc > 0) ? 1 : -1;
        for (int iCheckCol = colSrc + iColOffset; iCheckCol != colDest; iCheckCol = iCheckCol + iColOffset) {
            if (board[rowSrc][iCheckCol] != nullptr) {
                return false;
            }
        }
        return true;
    }
    else if (colDest == colSrc) {
        // Make sure that all invervening squares are empty
        int iRowOffset = (rowDest - rowSrc > 0) ? 1 : -1;
        for (int iCheckRow = rowSrc + iRowOffset; iCheckRow != rowDest; iCheckRow = iCheckRow + iRowOffset) {
            if (board[iCheckRow][colSrc] != nullptr) {
                return false;
            }
        }
        return true;
    }
    return false;
}

char Rook::getPiece() {
    return 'R';
}