#pragma once
#include <stdint.h>
#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>

using namespace std;

enum Color
{
    black = 0,
    white,

};

class Piece
{
protected:
    Color color;
    bool hasMoved_;
protected:
    sf::Sprite sprite;
    sf::Texture texture;
public:
    Piece(const Color& in_color);

    bool isWhite() const;
    void setColor(const Color& in_color);
    void setSpritePos(const double& x, const double& y);
    sf::Sprite getSprite();

    virtual char getPiece() = 0;
    virtual bool isValidMove(Piece* board[8][8], int32_t rowSrc, int32_t colSrc, int32_t rowDest, int32_t colDest) = 0;
    
    bool hasMoved();   
    void setHasMoved(bool); 

};

class King : public Piece
{
private:
    bool m_canCastle = true;
public:
    King(const Color& in_color): Piece(in_color)
    {
        this->texture;
        do
        {
            if (in_color)
            {
                this->texture.loadFromFile("sprites/wk.png");
                break;
            }

            this->texture.loadFromFile("sprites/bk.png");
        } while (false);

        this->sprite.setTexture(this->texture);

         
    }

    void setCanCastle(const bool& in_canCastle);
    char getPiece();
    bool isValidMove(Piece* board[8][8], int32_t rowSrc, int32_t colSrc, int32_t rowDest, int32_t colDest);
};

class Queen : public Piece
{
public:
    Queen(const Color& in_color) : Piece(in_color)
    {
        this->texture;
        do
        {
            if (in_color)
            {
                this->texture.loadFromFile("sprites/wq.png");
                break;
            }

            this->texture.loadFromFile("sprites/bq.png");
        } while (false);

        this->sprite.setTexture(this->texture);

        this->sprite.setTextureRect(sf::IntRect(0,0,64,64));
    }

    char getPiece();
    bool isValidMove(Piece* board[8][8], int32_t rowSrc, int32_t colSrc, int32_t rowDest, int32_t colDest);

};

class Bishop : public Piece
{
public:
    Bishop(const Color& in_color) : Piece(in_color)
    {
        this->texture;
        do
        {
            if (in_color)
            {
                this->texture.loadFromFile("sprites/wb.png");
                break;
            }

            this->texture.loadFromFile("sprites/bb.png");
        } while (false);

        this->sprite.setTexture(this->texture);

         
    }

    bool isValidMove(Piece* board[8][8], int32_t rowSrc, int32_t colSrc, int32_t rowDest, int32_t colDest);
    char getPiece();
};

class Knight : public Piece
{
public:
    Knight(const Color& in_color) : Piece(in_color)
    {

        this->texture;
        do
        {
            if (in_color)
            {
                this->texture.loadFromFile("sprites/wn.png");
                break;
            }

            this->texture.loadFromFile("sprites/bn.png");
        } while (false);

        this->sprite.setTexture(this->texture);

         
    }

    bool isValidMove(Piece* board[8][8], int32_t rowSrc, int32_t colSrc, int32_t rowDest, int32_t colDest);
    char getPiece();
};

class Rook : public Piece
{
protected:
    bool canCastle;
    bool m_canCastle = true;
public:
    Rook(const Color& in_color) : Piece(in_color)
    {
        this->texture;
        do
        {
            if (in_color)
            {
                this->texture.loadFromFile("sprites/wr.png");
                break;
            }

            this->texture.loadFromFile("sprites/br.png");
        } while (false);

        this->sprite.setTexture(this->texture);

         
    }

    bool checkCanCastling();
    void setCanCastle(const bool& in_canCastle);
    bool isValidMove(Piece* board[8][8], int32_t rowSrc, int32_t colSrc, int32_t rowDest, int32_t colDest);
    char getPiece();
};

class Pawn : public Piece
{
protected:
    bool posibleEnpassant;
    bool posibleMoveTwoSquare;
public:
    Pawn(const Color& in_color) : Piece(in_color)
    {
        this->posibleMoveTwoSquare = true;
        this->posibleEnpassant = false;

        this->texture;
        do
        {
            if (in_color)
            {
                this->texture.loadFromFile("sprites/wp.png");
                break;
            }

            this->texture.loadFromFile("sprites/bp.png");
        } while (false);

        this->sprite.setTexture(this->texture);

         
    }

    void setPosibleEnpassant(bool in_enpassant);
    void setPosibleMoveTwoSquare(bool in_moveTwoSquare);

    bool canEnpassant();
    bool canMoveTwoSquare();

    bool isValidMove(Piece* board[8][8], int32_t rowSrc, int32_t colSrc, int32_t rowDest, int32_t colDest);

    char getPiece();
};