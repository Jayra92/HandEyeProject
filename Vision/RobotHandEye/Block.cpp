#include "Block.h"



Block::Block()
{
	Position = 0;
	xPos = 0;
	yPos = 0;
}


Block::~Block()
{
}

bool Block::checkBlock(Mat Block, int x, int y)
{
	Vec3b intensityBlue = Block.at<Vec3b>(y, x);
		for (int i = x; i < x + 5; i++)
		{
			for (int j = y; j < y + 5; j++)
			{
				if (j > Block.rows-1 || j < 0 || i > Block.cols-1 || i < 0)
				{
					return false;
				}
				intensityBlue = Block.at<Vec3b>(j, i);
				if (intensityBlue.val[0] == 0)
				{
					return false;
				}
			}
		}

	return true;
}

bool Block::checkPosition()
{
	if (xPos == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

int Block::getPosy()
{
	return yPos;
}

int Block::getPosx()
{
	return xPos;
}

void Block::setPosy(int y)
{
	yPos = y;
}

void Block::setPosx(int x)
{
	xPos = x;
}

int Block::getPosition()
{
	return Position;
}

void Block::setPosition(int pos)
{
	Position = pos;
}