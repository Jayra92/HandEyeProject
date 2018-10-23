/*
* RSD_starter_video.cpp
*
* This code was developed for students studying RME40003 Robot Systems Design at Swinburne
* University of Technology in Melbourne, Australia
* 
* This code is largely based on the 'starter_video.cpp' code created by Ethan Rublee, Nov 23, 
* 2010 for use with OpenCV software. Ethan Rublee's code comes packaged with the OpenCV files 
* and can be found in ..\OpenCV\sources\samples\cpp\
*
* This is a simple starter sample  for using opencv, getting a video stream, displaying images,
* reading and outputting pixel values, and modifying pixel values.
* 
*/

/*
* CODE MODIFICATIONS AND BUG FIXES
* 24/07/2014 Code added by Michelle Dunn to read, output and modify pixel values
* 21/08/2014 Comments added by Michelle Dunn for Swinburne students
*/

#include "opencv2/highgui/highgui.hpp"
#include "stdafx.h"
#include "windows.h" 
#include <iostream>
#include <vector>
#include <stdio.h>
#include "block.h"

#using <mscorlib.dll>
#using <system.dll>


using namespace System;
using namespace System::IO::Ports;
using namespace cv;
using namespace std;




//hide the local functions in an anon namespace
namespace {
    void help(char** av) {
        cout << "\nThis program justs gets you started reading images from video\n"
            "Usage:\n./" << av[0] << " <video device number>\n"
            << "q,Q,esc -- quit\n"
            << "space   -- save frame\n\n"
            << "\tThis is a starter sample, to get you up and going in a copy pasta fashion\n"
            << "\tThe program captures frames from a camera connected to your computer.\n"
            << "\tTo find the video device number, try ls /dev/video* \n"
            << "\tYou may also pass a video file, like my_vide.avi instead of a device number"
            << endl;
    }

   

}

void sortBlocks(Block* redBlock, Block* blueBlock, Block* yellowBlock)
{
	//sort the blocks
	//assume values
	int positions[3] = { redBlock->getPosx(), yellowBlock->getPosx(), blueBlock->getPosx() };
	
	int redPos = 1, bluePos = 2, yellowPos = 3;
	
	//if any position isnt found, return 0 for each position.
	if (positions[0] = 0 || positions[1] = 0 || positions[2] = 0)
	{
		redPos = 0; 
		bluePos = 0; 
		yellowPos = 0;
	}
	
	else
	{

		sort(positions, positions + 3);

		redPos = distance(positions, (find(positions, positions + 3, redBlock->getPosx()))) + 1;
		bluePos = distance(positions, (find(positions, positions + 3, blueBlock->getPosx()))) + 1;
		yellowPos = distance(positions, (find(positions, positions + 3, yellowBlock->getPosx()))) + 1;

		blueBlock->setPosition(bluePos);
		redBlock->setPosition(redPos);
		yellowBlock->setPosition(yellowPos);
	}
	
	return;
}

int process(VideoCapture& capture)
{
	int n = 0;
	char filename[200];
	string window_name = "video | q or esc to quit";
	cout << "press space to save a picture. q or esc to quit" << endl;
	namedWindow(window_name, CV_WINDOW_KEEPRATIO); //resizable window;
	Mat frame;

	Block * redBlock = new Block();
	Block * blueBlock = new Block();
	Block * yellowBlock = new Block();

	//initalise frame, sometimes takes a while to colour properly
	for (int i = 0; i < 1; i++) {
		//needs a few frames to stabilise image first would recommend a dew cycles before detecting postions
		for (int i = 0; i < 20; i++) {
			capture >> frame;
			if (frame.empty())
				break;
		}
		capture >> frame;
		if (frame.empty())
			break;

		/****************** Entry point for Swinburne RSD students ****************/

		

		Mat bgr = frame;

		Mat hsv;
		vector<Mat> channels;

		cvtColor(bgr, hsv, COLOR_BGR2HSV);

		split(hsv, channels);
		Scalar m = mean(channels[1]);
		cout << "light intensity = " << m[0] << endl;

		//adjust the lower bounds of blue to find in light or dark
		int blueAdj;
		blueAdj = (m[0] * -7.2) + 330;

		Scalar Red_lower = Scalar(0, 70, 50);
		Scalar Red_higher = Scalar(10, 255, 255);
		Scalar Red_lower2 = Scalar(170, 70, 50);
		Scalar Red_higher2 = Scalar(180, 255, 255);

		Scalar Yellow_lower = Scalar(10, 100, 100);
		Scalar Yellow_higher = Scalar(30, 255, 255);

		Scalar Blue_lower = Scalar(100, blueAdj, 0);
		Scalar Blue_higher = Scalar(140, 255, 255);


		Mat maskred1, maskred2, maskyellow, maskblue;

		inRange(hsv, Red_lower, Red_higher, maskred1);
		inRange(hsv, Red_lower2, Red_higher2, maskred2);

		Mat maskRed = maskred1 | maskred2;

		inRange(hsv, Yellow_lower, Yellow_higher, maskyellow);
		inRange(hsv, Blue_lower, Blue_higher, maskblue);

		Mat mask = maskRed | maskred2 | maskyellow | maskblue;

		Mat maskout;

		bitwise_and(bgr, bgr, maskout, mask);

		Mat RedBlockMap, YellowBlockMap, BlueBlockMap;

		bitwise_and(bgr, bgr, RedBlockMap, maskRed);
		bitwise_and(bgr, bgr, YellowBlockMap, maskyellow);
		bitwise_and(bgr, bgr, BlueBlockMap, maskblue);


		Vec3b intensityRed, intensityBlue, intensityYellow;

		//go through each row of pixels
		for (int i = 0; i < (RedBlockMap.cols - 1); i++)
		{
			for (int j = 0; j < (RedBlockMap.rows - 1); j++)
			{
				intensityRed = RedBlockMap.at<Vec3b>(j, i);
				intensityBlue = BlueBlockMap.at<Vec3b>(j, i);
				intensityYellow = YellowBlockMap.at<Vec3b>(j, i);

				//check for first instance of each colour
				{
					if (redBlock->checkBlock(RedBlockMap, i, j) && redBlock->checkPosition())
					{
						redBlock->setPosx(i);
						redBlock->setPosy(j);
					}

					else if (intensityYellow.val[0] > 0)
					{
						if (yellowBlock->checkBlock(YellowBlockMap, i, j) && yellowBlock->checkPosition())
						{
							yellowBlock->setPosx(i);
							yellowBlock->setPosy(j);
						}
					}

					else if (intensityBlue.val[0] > 0)
					{
						if (blueBlock->checkBlock(BlueBlockMap, i, j) && blueBlock->checkPosition())
						{
							blueBlock->setPosx(i);
							blueBlock->setPosy(j);
						}
					}
				}
			}


		}


		sortBlocks(redBlock, blueBlock, yellowBlock);

		/****************** End of Swinburne modifications ****************/

		imshow(window_name, frame);
	}

	int BlockPositions = (redBlock->getPosition() * 100) + (blueBlock->getPosition() * 10) + (yellowBlock->getPosition() * 1);

	cout << "Red = " << redBlock->getPosition() << "Blue = " << blueBlock->getPosition() << "Yellow " << yellowBlock->getPosition() << endl;

	return BlockPositions;
}

int Comms(int BlockPositions)
{
	auto texBufArray = gcnew cli::array<unsigned char>(1);
	int baudRate = 9600;
	// robot interpreter box settings
	SerialPort^ robot_int;
	robot_int = gcnew SerialPort("COM4", baudRate);

	int LeftPos, MiddlePos, RightPos, waitTimer = 15000;
	

	switch (BlockPositions)
	{
	//   RBY
	case 123: {LeftPos = 33; MiddlePos = 66; RightPos = 132; break; };
	case 132: {LeftPos = 33; MiddlePos = 130; RightPos = 68; break; };
	case 213: {LeftPos = 65; MiddlePos = 34; RightPos = 132; break; };
	case 231: {LeftPos = 129; MiddlePos = 34; RightPos = 68; break; };
	case 312: {LeftPos = 65; MiddlePos = 130; RightPos = 36; break; };
	case 321: {LeftPos = 129; MiddlePos = 66; RightPos = 36; break; };
	default: { LeftPos = 0; MiddlePos = 0; RightPos= 0; break;};
	}

	// open port
	try
	{
		// Open port to robot interpreter box
		robot_int->Open();

		// Send Red Pos
		texBufArray[0] = LeftPos;

		// Write number to the port
		robot_int->Write(texBufArray, 0, 1);

		//Wait
		Sleep(waitTimer);

		// Send Blue Pos
		texBufArray[0] = MiddlePos;

		// Write number to the port
		robot_int->Write(texBufArray, 0, 1);

		//Wait
		Sleep(waitTimer);

		// Send Yellow Pos
		texBufArray[0] = RightPos;

		// Write number to the port
		robot_int->Write(texBufArray, 0, 1);

		//Wait
		Sleep(waitTimer);

		// Im Done
		texBufArray[0] = 8;

		// Write number to the port
		robot_int->Write(texBufArray, 0, 1);

		//Wait
		Sleep(1000);

		// Im Clear
		texBufArray[0] = 0;

		// Write number to the port
		robot_int->Write(texBufArray, 0, 1);


		// close port to robot interpreter box
		robot_int->Close();
	}
	catch (IO::IOException^ e)
	{
		Console::WriteLine(e->GetType()->Name + ": Port is not ready");
	}
	// end program
	// Console::Write("Press enter to close the program");
	// Console::Read();
	Console::Write("Closing...");
	return 0;
}

int main() {

    //std::string arg = av[1];
    VideoCapture capture(0); //try to open string, this will attempt to open it as a video file
    if (!capture.isOpened()) //if this fails, try to open as a video camera, through the use of an integer param
        capture.open(atoi(0));
    if (!capture.isOpened()) {
        cerr << "Failed to open a video device or video file!\n" << endl;
        help(0);
        return 1;
    }

	int BlockPositions;

	BlockPositions = process(capture);

	capture.release();

	Comms(BlockPositions);

	return 0;
}


