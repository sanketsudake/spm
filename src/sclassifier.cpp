/*!
 * \file sclassifier.cpp
 * \brief Consist methods to classify shots
 */

#include "sclassifier.hpp"



Sclassifier:: Sclassifier()
{
    shot_string.push_back ("NO_SPIN_HEADON");
    shot_string.push_back("NO_SPIN_CUT");

    shot_string.push_back("FORWARD_SPIN_HEADON");
    shot_string.push_back("FORWARD_SPIN_LEFT");
    shot_string.push_back("FORWARD_SPIN_RIGHT");

    shot_string.push_back("BACKWARD_SPIN_HEADON");
    shot_string.push_back("BACKWARD_SPIN_LEFT");
    shot_string.push_back("BACKWARD_SPIN_RIGHT");

    shot_string.push_back("LEFT_SPIN_HEADON"); //along right normal
    shot_string.push_back("LEFT_SPIN_LEFT");
    shot_string.push_back("LEFT_SPIN_RIGHT");

    shot_string.push_back("RIGHT_SPIN_HEADON");  //along left normal
    shot_string.push_back("RIGHT_SPIN_LEFT");
    shot_string.push_back("RIGHT_SPIN_RIGHT");
 
}

Sclassifier:: ~Sclassifier()
{

}


string Sclassifier::getShotString(unsigned int s)
{
    return (shot_string[s]);
}

unsigned int Sclassifier::shot_classifier(double angle,int side, bool iscollinear, int motion)
{
    /*! * beta =>
     * angle between normal to point of contact  and white final
     * side =>
     * Side(0 for left, 1 for right) of Cue ball final w.r.t tangent
     * iscollinear=>
     * point of collision and target ball collinear
     * motion=>
     * Motion of Cue Ball (0-Stop,1-Fwd,2-Bkwd) - for fwd,bkwd spin
     * */

    const float THRESHOLD_HEADON=3.0;
    const float THRESHOLD_SPIN_GREATER_THAN_90=93.0;
    const float THRESHOLD_SPIN_LESS_THAN_90=87.0;
//	unsigned int shot_result = -1;


    if(motion == int(0) && iscollinear && angle>=0.0 && angle <= THRESHOLD_HEADON)
    {
        printf("Shot Type : NO_SPIN_HEADON");
        shot_result = NO_SPIN_HEADON;
    }

    else if(motion == int(1) && iscollinear && angle>=0.0 && angle <= THRESHOLD_HEADON)
    {
        printf("Shot Type : FORWARD_SPIN_HEADON");
        shot_result = FORWARD_SPIN_HEADON;
    }

    else if(motion == int(2) && iscollinear && angle>=0.0 && angle <= THRESHOLD_HEADON)
    {
        printf("Shot Type : FORWARD_SPIN_HEADON");
        shot_result = BACKWARD_SPIN_HEADON;
    }

    else if(angle > THRESHOLD_SPIN_GREATER_THAN_90 && angle <= 180.0)
    {
        if(side == int(0)) { 
            printf("Shot Type : RIGHT_SPIN_LEFT");
            shot_result = RIGHT_SPIN_LEFT;
        }

        else if (side == int(1)) { 
            printf("Shot Type : LEFT_SPIN_RIGHT");
            shot_result = LEFT_SPIN_RIGHT;
        }
    }

    else if(angle >= 0.0 && angle < THRESHOLD_SPIN_LESS_THAN_90)
    {
        if(side == int(0)) { 
            printf("Shot Type : RIGHT_SPIN_RIGHT");
            shot_result = RIGHT_SPIN_RIGHT;
        }

        else if (side == int(1)) { 
            printf("Shot Type : LEFT_SPIN_LEFT");
            shot_result = LEFT_SPIN_LEFT;
        }
    }

    else if(angle >= THRESHOLD_SPIN_LESS_THAN_90 && angle <= THRESHOLD_SPIN_GREATER_THAN_90)
    {
        if(iscollinear)
        {
            if(side == int(0)) { 
                printf("Shot Type : RIGHT_SPIN_HEADON");
                shot_result = RIGHT_SPIN_HEADON;
            }

            else if (side == int(1)) { 
                printf("Shot Type : LEFT_SPIN_HEADON");
                shot_result = LEFT_SPIN_HEADON;
            }
        }
        else if(!iscollinear) {
            printf("Shot Type : NO_SPIN_CUTSHOT");
            shot_result = NO_SPIN_CUT;
        }
    }		


	cout<<endl;
    return shot_result;
}
