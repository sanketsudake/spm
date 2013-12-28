/*!
 * \file detectshot.cpp
 * \brief Consist methods to process shot
 */

#include "detectshot.hpp"

using namespace std;
using namespace cv;

ShotArray::ShotArray()
{
	white_positions.clear();
}

ShotArray::~ShotArray()
{

}

void ShotArray::addPosition(Point position)
{
	white_positions.push_back(position);
}

void ShotArray::clearArray()
{
	white_positions.clear();
}

void ShotArray::drawPath(Mat &frame)
{
	int white_positions_size = (int)white_positions.size();
	for(int i = 0; i < white_positions_size - 1; i++ )
	{
		line(frame, white_positions[i], white_positions[i+1],
			 Scalar(255, 255, 255), 1, CV_AA, 0);
		circle(frame, white_positions[i], 1, Scalar(255, 255 ,200), 2);
	}
}

DetectShot::DetectShot()
{
	shotcount = 0;
	shot_state = 0;
	shotstart = 0;
	prevPosition = Point(-1, -1);
	shottemp = 0;
	flag = 1;
	contour_temp = 0;
    // Innital tweaks required for easy shot identification
    cout << "Adjust SHOT_TEMP_COUNT if shot start & end not identified" << endl
         << "correctly.Standard time interval should be kept bet. two" << endl
         << "shots." << endl;
}

DetectShot::~DetectShot()
{

}

int DetectShot::BgSubtractor(Mat &frame)
{
	//! Perform background subtraction idenfies foreground objects
	bg.operator ()(frame, fore);
	bg.getBackgroundImage(back);

    //! Erode and dilate objects on foreground objects
	erode(fore, fore, Mat());
	dilate(fore, fore, Mat());

	//! Find contours on foreground image
	findContours(fore, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

	//drawContours(frame,  contours,  -1,  Scalar(0,  0,  255),  2);

	//! Return contours size
	return contours.size();
}

int DetectShot::shotChecker(Mat &frame, Point position)
{
	int returnval = 0;

	//! Get no of contours first
	int contours_size = BgSubtractor(frame);

	//! Is white ball moving
	int whiteMoving = isWhiteMoving(position);
	prevPosition = position;
	if(!shotcount)
	{
		if(flag == 2)
		{
			shotTrigger(frame);
			returnval = 1;
			flag = 0;
		}
		if(flag)
		{
			//preshotTrigger(frame);
			flag = 2;
		}
	}

	//! Let ball five times => shot start
	if(whiteMoving && !flag)
	{
		shottemp++;
		if(shottemp > 5)
			flag = 1;
	}

	//! Shot end => count background subtraction frames
	if(contours_size == 0 && flag)
	{
		contour_temp++;
		// if(contour_temp == 9)
		// 	preshotTrigger(frame);
		if(contour_temp == 10)
		{
			returnval = 1;
			shotTrigger(frame);
			// cout << "Contour" << contour_temp << shottemp << endl;
			flag = contour_temp = shottemp = 0;
		}
	}
	displayShotnumber(frame);
	return returnval;
}

void DetectShot::preshotTrigger(Mat &frame)
{
	putText(frame, "Press ESC.",
			Point(600, 500), 1, 1, Scalar(200, 200, 200), 2);
}

void DetectShot::shotTrigger(Mat &frame)
{
	shotcount++;
}

void DetectShot::displayShotnumber(Mat &frame)
{
	//! Show shot number on screen
	stringstream ss;
	ss << "Shot No : " << shotcount;
	putText(frame, ss.str(),
			Point(40, 15), 1, 1, Scalar(255, 255, 255), 2);
}

int DetectShot::isWhiteMoving(Point position)
{
	/*! Check is ball is moving with previous position
	 * Returns 0 if ball not moving
	 * Returns 1 if ball is moving
	 */
	if(prevPosition.x == -1)
		return 0;
	else
	{
		if(std::abs(position.x - prevPosition.x) == 0
		   && std::abs(position.y - prevPosition.y) == 0)
			return 0;
	}
	return 1;
}

void DetectShot::movingBall(Mat &frame, Point position)
{
	//! Now draw those contours on frame with red color
	// int contours_size = contours.size();
	// vector<Point2f>center( contours_size );
	// vector<float>radius( contours_size );
	// int ball_count = 0;

	// //! if whiteball is moving identify other ball which are moving
	// if(isWhiteMoving(position))
	// {
	// 	for(int k = 0; k < contours_size; k++)
	// 	{
	// 		//if(isContourConvex(contours[k]))
	// 		{
	// 			//! Find closest circle to given contours
	// 			//! Similar to convex hull problem
	// 			minEnclosingCircle((Mat)contours[k], center[k], radius[k]);
	// 			//! Draw circle within specified radius
	// 			if(7 < (int)radius[k] && (int)radius[k] < 13)
	// 			{
	// 				//std::cout << "Radius" << radius[k] << std::endl;
	// 				ball_count++;
	// 				circle(frame, center[k], radius[k], Scalar(0, 255, 0));
	// 			}
	// 		}
	// 	}
	// 	//! If ball moving
	// 	if(ball_count > 1)
	// 		putText(frame, "Moving",
	// 				Point(500, 500), 5, 1, Scalar(255, 255, 255), 2);
	// }
	// //! Show number of moving balls on screen
	// stringstream ss;
	// ss << "Moving Ball No : " << ball_count;
	// putText(frame, ss.str(),
	// 		Point(100, 630), 1, 1, Scalar(255, 255, 255), 2);
}

CollisionDetector:: CollisionDetector()
{
	reset();
}

CollisionDetector:: ~CollisionDetector()
{
}

void CollisionDetector:: reset()
{
	prevPoint = Point(-1, -1);
	prevSlope = 1000;
	collisionCount = 1;
	for(int i = 0; i < 10; i++)
		collPoints[i] = Point(-1, -1);
	slopeTheta = 0.19;
}

void CollisionDetector::setShotStartPoint(Point position)
{
	collPoints[0] = position;
}

void CollisionDetector::drawPrev(Mat &frame)
{
	for(int i = 1; i < collisionCount; i++)
		circle(frame, collPoints[i], 10, Scalar(255, 0 ,0), 2);
}

void CollisionDetector::drawPath(Mat &frame)
{
	if(collisionCount > 4)
	{
		line(frame, collPoints[0], collPoints[1], Scalar(0, 200, 220), 1, CV_AA);
		line(frame, collPoints[1], collPoints[2], Scalar(0, 200, 200), 1, CV_AA);
		line(frame, collPoints[2], collPoints[3], Scalar(0, 200, 200), 1, CV_AA);
		line(frame, collPoints[3], collPoints[4], Scalar(0, 200, 200), 1, CV_AA);
	}
}

void CollisionDetector::checkCollision(Point position)
{
	//! Identifying only 4 collisions
	if(collisionCount > 4)
		return;

	//! Code need to refactorized
	double currSlope = 1000;

	//! Temporary code need to removed
	cout << "W.Position " << position.x
		 << " "
		 << position.y << " ";
	cout << prevPoint.y - position.y << " "
		 << prevPoint.x - position.x << " ";

	int xdelta = position.x - prevPoint.x;
	int ydelta = position.y - prevPoint.y;

	if(xdelta)
	{
		currSlope = ((double)(ydelta)/(xdelta));
		cout << currSlope <<  " ";
		cout << prevSlope - currSlope ;
		// Redundant code => if statement can merged
		if(prevSlope < 900)
		{
			if(abs(prevSlope - currSlope) > slopeTheta)
			{
				if(!(abs(xdelta) == 1 && abs(ydelta) == 1))
				{
					cout << "Collision" ;
					collPoints[collisionCount] = prevPoint;
					collisionCount++;
					slopeTheta += 0.018;
					//! Wait after every collision
					//while(waitKey(1) != 27);
				}
			}
		}
	}
	cout << endl;
	prevPoint = position;
	prevSlope = currSlope;
}
