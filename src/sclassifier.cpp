/*!
 * \file sclassifier.cpp
 * \brief Consist methods to classify shots
 */

#include "sclassifier.hpp"



Sclassifier:: Sclassifier()
{

}

Sclassifier:: ~Sclassifier()
{

}

int Sclassifier::classifier(int beta, Point contact, int alpha)
{
    /*! alpha =>
     * angle between white ball segment before collision
     * & segment of white ball after collision
     * beta =>
     * angle between normal to point of contact  and white final
     * point of contact =>
     * point of collision between white ball and target ball
     * point should be precise considering intersection between
     * circumference of two circles
     */
    return 1;
}
