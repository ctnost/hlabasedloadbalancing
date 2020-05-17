#ifndef WIRELESSSENSOR_H_
#define WIRELESSSENSOR_H_

#include "RTI.hh"

class WirelessSensor
{
public:
	WirelessSensor(RTI::ObjectHandle _objectID )
	{
		objectID = _objectID;
		rc = 0;
		sc = 0;
	}
	WirelessSensor(int x, int y, int _radius, double _power, RTI::ObjectHandle _objectID )
	{
		posx = x;
		posy = y;
		power = _power;
		radius = _radius;
		objectID = _objectID;
		rc = 0;
		sc = 0;
	}
public:
	int posx,posy;
	int radius;
	double power;
	double nextSendingTime;
	RTI::ObjectHandle objectID;
	string name;
	int rc;
	int sc;
};
#endif