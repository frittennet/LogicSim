#pragma once

#include "NodeType.h" 
#include "Vector2Int.h"
#include "Direction.h" 

class NumberAction;
class Number;
class Grid;

class NodeCustom; 

class Node
{
public:
	Vector2Int position;
	Direction direction;
	Grid* grid;
	NodeType type;

	// subclass data ( for alignment in mem pool ) 
	Grid* subGrid;
	NodeCustom* parentNode; 

	int id;

	Node() { }; 
	Node(NodeType type, Grid* grid);

	virtual NumberAction* getAction(Number* number);

};

class NodeMinus : public Node
{
public:
	NodeMinus(Grid* grid);

	NumberAction* getAction(Number* number);
};

class NodeSmallerThan : public Node
{
public:
	NodeSmallerThan(Grid* grid); 

	NumberAction* getAction(Number* number); 
};

class NodeInput : public Node
{
public: 
	NodeInput(Grid* grid);  

	NumberAction* getAction(Number* number); 
};

class NodeCustom : public Node
{
public:

	NodeCustom(Grid* grid, Grid* subGrid);
	~NodeCustom();

	NumberAction* getAction(Number* number);
};

class NodeOutput : public Node
{
public:

	NodeOutput(Grid* grid, NodeCustom* outputNode);

	NumberAction* getAction(Number* number);
};

class NodeGameInput : public Node {
public: 
	NodeGameInput(Grid* grid); 

	NumberAction* getAction(Number* number); 
};

class NodeGameOutput : public Node {
public:
	NodeGameOutput(Grid* grid);

	NumberAction* getAction(Number* number);
};
