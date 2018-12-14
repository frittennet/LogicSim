#pragma once

#include <iostream>

#include "Direction.h" 

struct __declspec(dllexport) Vector2Int {
public:
	Vector2Int();
	Vector2Int(int x, int y);

	int32_t x;
	int32_t y;

	static const Vector2Int zero; 
	static const Vector2Int one;
	static const Vector2Int up;
	static const Vector2Int right;
	static const Vector2Int down;
	static const Vector2Int left;
	static const Vector2Int forDirection[4]; 

	static Vector2Int fromDirection(Direction direction); 

	Vector2Int operator+(const Vector2Int& other); 
	Vector2Int operator-(const Vector2Int& other);
	Vector2Int operator*(const int other);
	const bool operator==(const Vector2Int& other);

};

