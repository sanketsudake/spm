/*!
 * \file buildprofile.hpp
 * \brief
 */

#ifndef __BUILDPROFILE_H_INCLUDED__
#define __BUILDPROFILE_H_INCLUDED__
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/core/core.hpp>

class Shot
{
	private:
	public:
	Shot();
	~Shot();
	void angleError();
	void velocityError();
	void rateShot();
	void shottype();
};

/*!
 * \class BuildProfile
 * \brief
o */
class BuildProfile
{
private:

public:
	BuildProfile();
	~BuildProfile();
};


#endif
