#pragma once

#include <opencv2\opencv.hpp>

using namespace cv;
using namespace std;

class Block
{
private:
	int xPos, yPos, Position;
public:
	Block();
	~Block();
	bool checkBlock(Mat Block, int x, int y);

	int getPosy();

	int getPosx();

	void setPosy(int y);

	void setPosx(int x);

	int getPosition();

	void setPosition(int pos);

	bool checkPosition();

};




