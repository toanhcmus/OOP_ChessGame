#include "Piece.h"

Piece::Piece(const Color &in_color)
{
    this->color = in_color;
}

bool Piece::isWhite() const
{
    return this->color;
}

void Piece::setColor(const Color &in_color)
{
    this->color = in_color;
}

void Piece::setSpritePos(const double& x, const double& y)
{
    this->sprite.setPosition(x, y);
}

sf::Sprite Piece::getSprite()
{
    return this->sprite;
}

bool Piece::hasMoved() {
    return this->hasMoved_;
}

void Piece::setHasMoved(bool moved) {
    this->hasMoved_ = moved;
}