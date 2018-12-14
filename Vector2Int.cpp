#include <iostream>
#include "Vector2Int.h"

Vector2Int::Vector2Int() {
	x = 0;
	y = 0;
}

Vector2Int::Vector2Int(int initX, int initY) {
	x = initX;
	y = initY;
}

Vector2Int Vector2Int::fromDirection(Direction direction) {
	return Vector2Int::forDirection[direction]; 
}

Vector2Int Vector2Int::operator+(const Vector2Int & other)
{
	Vector2Int sum; 
	sum.x = x + other.x; 
	sum.y = y + other.y;
	return sum; 
}

Vector2Int Vector2Int::operator-(const Vector2Int & other)
{
	Vector2Int sum;
	sum.x = x - other.x;
	sum.y = y - other.y;
	return sum;
}

Vector2Int Vector2Int::operator*(const int scalar)
{
	Vector2Int res;
	res.x = x * scalar; 
	res.y = y * scalar; 
	return res;
}

const bool Vector2Int::operator==(const Vector2Int & other)
{
	return this->x == other.x && this->y == other.y; 
}

const Vector2Int Vector2Int::zero = Vector2Int(0, 0); 
const Vector2Int Vector2Int::one = Vector2Int(1, 1);
const Vector2Int Vector2Int::up = Vector2Int(0, 1);
const Vector2Int Vector2Int::right = Vector2Int(1, 0);
const Vector2Int Vector2Int::down = Vector2Int(0, -1);
const Vector2Int Vector2Int::left = Vector2Int(-1, 0);
const Vector2Int Vector2Int::forDirection[] = { Vector2Int::up, Vector2Int::right, Vector2Int::down, Vector2Int::left }; 
