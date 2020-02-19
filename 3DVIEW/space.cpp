#include "pch.h"
#include "space.h"
#include <vector>
#include <string>
#include <math.h>
#include <graphics.h>
#include <iostream>

pureObj::pureObj(float value_x, float value_y, float value_z)
{
	theCenterPoint.value_x = value_x;
	theCenterPoint.value_y = value_y;
	theCenterPoint.value_z = value_z;
	xy_theta = 0;
	yz_theta = 0;
	xz_theta = 0;
}
void pureObj::rotate(int axis, float angel)
{
	switch (axis)
	{
	case 1:
		xy_theta += angel;
		break;
	case 2:
		xz_theta += angel;
		break;
	case 3:
		yz_theta += angel;
		break;
	default:
		break;
	}
}
void pureObj::move(float x, float y, float z,int spaceTo)
{
	Vector1x3 temp(x, y, z);
	if (spaceTo == 1)
		theCenterPoint = theCenterPoint + temp;
	if (spaceTo == 2)
	{
		Matrix3x3 a(xy_theta, 1);
		Matrix3x3 b(xz_theta, 2);
		Matrix3x3 c(yz_theta, 3);
		temp = a * b * c * temp;
		theCenterPoint = theCenterPoint + temp;
	}
}
Camera::Camera(float k, int pm) :pureObj(0, 0, 0)
{
	this->k = k;
	this->pm = pm;
}
Camera::Camera(float k, int pm, float x, float y, float z) : pureObj(x, y, z)
{
	this->k = k;
	this->pm = pm;
}
Camera::~Camera() {}
Vector1x3 operator*(Matrix3x3 a, Vector1x3 b)
{
	Vector1x3 result(
		a.theValue[0][0] * b.value_x + a.theValue[0][1] * b.value_y + a.theValue[0][2] * b.value_z,
		a.theValue[1][0] * b.value_x + a.theValue[1][1] * b.value_y + a.theValue[1][2] * b.value_z,
		a.theValue[2][0] * b.value_x + a.theValue[2][1] * b.value_y + a.theValue[2][2] * b.value_z
	);
	return result;
}
Matrix3x3 Matrix3x3::operator*(Matrix3x3 other)
{
	Matrix3x3 resultMat(
		theValue[0][0] * other.theValue[0][0] + theValue[0][1] * other.theValue[1][0] + theValue[0][2] * other.theValue[2][0],
		theValue[0][0] * other.theValue[0][1] + theValue[0][1] * other.theValue[1][1] + theValue[0][2] * other.theValue[2][1],
		theValue[0][0] * other.theValue[0][2] + theValue[0][1] * other.theValue[1][2] + theValue[0][2] * other.theValue[2][2],
		theValue[1][0] * other.theValue[0][0] + theValue[1][1] * other.theValue[1][0] + theValue[1][2] * other.theValue[2][0],
		theValue[1][0] * other.theValue[0][1] + theValue[1][1] * other.theValue[1][1] + theValue[1][2] * other.theValue[2][1],
		theValue[1][0] * other.theValue[0][2] + theValue[1][1] * other.theValue[1][2] + theValue[1][2] * other.theValue[2][2],
		theValue[2][0] * other.theValue[0][0] + theValue[2][1] * other.theValue[1][0] + theValue[2][2] * other.theValue[2][0],
		theValue[2][0] * other.theValue[0][1] + theValue[2][1] * other.theValue[1][1] + theValue[2][2] * other.theValue[2][1],
		theValue[2][0] * other.theValue[0][2] + theValue[2][1] * other.theValue[1][2] + theValue[2][2] * other.theValue[2][2]
	);
	return resultMat;
}
void Camera::draw(std::vector<tObject*> &theObjects)
{
	BeginBatchDraw();
	cleardevice();
	for (int i = 0; i < theObjects.size(); i++)
	{
		for (int o = 0; o < theObjects[i]->object_points.size(); o++)
		{
			Vector1x3 result;
			Matrix3x3 a(theObjects[i]->xy_theta, 1);
			Matrix3x3 b(theObjects[i]->xz_theta, 2);
			Matrix3x3 c(theObjects[i]->yz_theta, 3);
			result = a * b * c * theObjects[i]->object_points[o].Point + theObjects[i]->theCenterPoint;
			a.changeData(xy_theta, 1);
			b.changeData(xz_theta, 2);
			c.changeData(yz_theta, 3);
			result = c.inverse() * b.inverse() * a.inverse()*(result - theCenterPoint);
			if (result.value_x >= 0)
			{
				float pointx = (result.value_y / abs(result.value_x))*pm / k + PMX / 2;
				float pointy = -(result.value_z / abs(result.value_x))*pm / k + PMY / 2;
				//circle(int(pointx), int(pointy), 5);
				theObjects[i]->object_points[o].PM_X = pointx;
				theObjects[i]->object_points[o].PM_Y = pointy;
				//putpixel(int(pointx), int(pointy), RGB(255, 255, 255));
				//std::cout << "x=" << pointx << ",y=" << pointy << std::endl;
				theObjects[i]->object_points[o].visible = true;
			}
			else theObjects[i]->object_points[o].visible = false;
		}
		for (int o = 0; o < theObjects[i]->object_points.size(); o++)
		{
			for (int p = 0; p < theObjects[i]->object_points[o].connection.size(); p++)
			{
				if (theObjects[i]->object_points[o].visible&&theObjects[i]->object_points[theObjects[i]->object_points[o].connection[p]].visible)
				line(int(theObjects[i]->object_points[o].PM_X), int(theObjects[i]->object_points[o].PM_Y), 
					int(theObjects[i]->object_points[theObjects[i]->object_points[o].connection[p]].PM_X), 
					int(theObjects[i]->object_points[theObjects[i]->object_points[o].connection[p]].PM_Y));
			}
		}
	}
	EndBatchDraw();
}
tObject::tObject(int Object_TYPE) : pureObj(0, 0, 0)
{
	putPoint(Object_TYPE);
}
tObject::tObject(int Object_TYPE, float x, float y, float z) : pureObj(x, y, z)
{
	putPoint(Object_TYPE);
}
tObject::~tObject() {}
void tObject::putPoint(int Object_TYPE)
{
	the_Point* a;
	switch (Object_TYPE)
	{
	case 1:
	{
		a = new the_Point;
		a->Point.value_x = 1;
		a->Point.value_y = 1;
		a->Point.value_z = 1;
		a->connection.push_back(1);
		a->connection.push_back(3);
		a->connection.push_back(4);
		object_points.push_back(*a);
		a = new the_Point;
		a->Point.value_x = -1;
		a->Point.value_y = 1;
		a->Point.value_z = 1;
		a->connection.push_back(2);
		a->connection.push_back(5);
		object_points.push_back(*a);
		a = new the_Point;
		a->Point.value_x = -1;
		a->Point.value_y = -1;
		a->Point.value_z = 1;
		a->connection.push_back(3);
		a->connection.push_back(6);
		object_points.push_back(*a);
		a = new the_Point;
		a->Point.value_x = 1;
		a->Point.value_y = -1;
		a->Point.value_z = 1;
		a->connection.push_back(7);
		object_points.push_back(*a);
		a = new the_Point;
		a->Point.value_x = 1;
		a->Point.value_y = 1;
		a->Point.value_z = -1;
		a->connection.push_back(5);
		a->connection.push_back(7);
		object_points.push_back(*a);
		a = new the_Point;
		a->Point.value_x = -1;
		a->Point.value_y = 1;
		a->Point.value_z = -1;
		a->connection.push_back(6);
		object_points.push_back(*a);
		a = new the_Point;
		a->Point.value_x = -1;
		a->Point.value_y = -1;
		a->Point.value_z = -1;
		a->connection.push_back(7);
		object_points.push_back(*a);
		a = new the_Point;
		a->Point.value_x = 1;
		a->Point.value_y = -1;
		a->Point.value_z = -1;
		object_points.push_back(*a);
	}
	break;
	default:
		throw std::string("没有这个物体类型！！");
		break;
	}
}
Vector1x3::Vector1x3()
{
	value_x = 0;
	value_y = 0;
	value_z = 0;
}
Vector1x3::Vector1x3(float value_x, float value_y, float value_z)
{
	this->value_x = value_x;
	this->value_y = value_y;
	this->value_z = value_z;
}
Vector1x3 Vector1x3::operator+(Vector1x3 &other)
{
	Vector1x3 theValue;
	theValue.value_x = this->value_x + other.value_x;
	theValue.value_y = this->value_y + other.value_y;
	theValue.value_z = this->value_z + other.value_z;
	return theValue;
}
Vector1x3 Vector1x3::operator-(Vector1x3 &other)
{
	Vector1x3 theValue;
	theValue.value_x = this->value_x - other.value_x;
	theValue.value_y = this->value_y - other.value_y;
	theValue.value_z = this->value_z - other.value_z;
	return theValue;
}
Matrix3x3::Matrix3x3(float theta, int surface)
{
	the_axis = surface;
	switch (surface)	//axis-1 xy		axis-2 xz	axis=3 yz
	{
	case 1:
		theValue[0][0] = cos(theta), theValue[0][1] = -sin(theta), theValue[0][2] = 0;
		theValue[1][0] = sin(theta), theValue[1][1] = cos(theta), theValue[1][2] = 0;
		theValue[2][0] = 0, theValue[2][1] = 0, theValue[2][2] = 1;
		break;
	case 2:
		theValue[0][0] = cos(theta), theValue[0][1] = 0, theValue[0][2] = -sin(theta);
		theValue[1][0] = 0, theValue[1][1] = 1, theValue[1][2] = 0;
		theValue[2][0] = sin(theta), theValue[2][1] = 0, theValue[2][2] = cos(theta);
		break;
	case 3:
		theValue[0][0] = 1, theValue[0][1] = 0, theValue[0][2] = 0;
		theValue[1][0] = 0, theValue[1][1] = cos(theta), theValue[1][2] = -sin(theta);
		theValue[2][0] = 0, theValue[2][1] = sin(theta), theValue[2][2] = cos(theta);
		break;
	default:
		throw std::string("参数输入错误!");
		break;
	}
}
void Matrix3x3::changeData(float theta, int surface)
{
	the_axis = surface;
	switch (surface)	//axis-1 xy		axis-2 xz	axis=3 yz
	{
	case 1:
		theValue[0][0] = cos(theta), theValue[0][1] = -sin(theta), theValue[0][2] = 0;
		theValue[1][0] = sin(theta), theValue[1][1] = cos(theta), theValue[1][2] = 0;
		theValue[2][0] = 0, theValue[2][1] = 0, theValue[2][2] = 1;
		break;
	case 2:
		theValue[0][0] = cos(theta), theValue[0][1] = 0, theValue[0][2] = -sin(theta);
		theValue[1][0] = 0, theValue[1][1] = 1, theValue[1][2] = 0;
		theValue[2][0] = sin(theta), theValue[2][1] = 0, theValue[2][2] = cos(theta);
		break;
	case 3:
		theValue[0][0] = 1, theValue[0][1] = 0, theValue[0][2] = 0;
		theValue[1][0] = 0, theValue[1][1] = cos(theta), theValue[1][2] = -sin(theta);
		theValue[2][0] = 0, theValue[2][1] = sin(theta), theValue[2][2] = cos(theta);
		break;
	default:
		throw std::string("参数输入错误!");
		break;
	}
}
void Matrix3x3::changeData(float theta)
{
	switch (the_axis)	//axis-1 xy		axis-2 xz	axis=3 yz
	{
	case 1:
		theValue[0][0] = cos(theta), theValue[0][1] = -sin(theta), theValue[0][2] = 0;
		theValue[1][0] = sin(theta), theValue[1][1] = cos(theta), theValue[1][2] = 0;
		theValue[2][0] = 0, theValue[2][1] = 0, theValue[2][2] = 1;
		break;
	case 2:
		theValue[0][0] = cos(theta), theValue[0][1] = 0, theValue[0][2] = -sin(theta);
		theValue[1][0] = 0, theValue[1][1] = 1, theValue[1][2] = 0;
		theValue[2][0] = sin(theta), theValue[2][1] = 0, theValue[2][2] = cos(theta);
		break;
	case 3:
		theValue[0][0] = 1, theValue[0][1] = 0, theValue[0][2] = 0;
		theValue[1][0] = 0, theValue[1][1] = cos(theta), theValue[1][2] = -sin(theta);
		theValue[2][0] = 0, theValue[2][1] = sin(theta), theValue[2][2] = cos(theta);
		break;
	default:
		throw std::string("参数输入错误!");
		break;
	}
}
Matrix3x3::Matrix3x3(float x1, float x2, float x3, float x4, float x5, float x6, float x7, float x8, float x9)
{
	theValue[0][0] = x1, theValue[0][1] = x2, theValue[0][2] = x3;
	theValue[1][0] = x4, theValue[1][1] = x5, theValue[1][2] = x6;
	theValue[2][0] = x7, theValue[2][1] = x8, theValue[2][2] = x9;
}
Matrix3x3 Matrix3x3::inverse()
{
	Matrix3x3 inMat(theValue[0][0], theValue[1][0], theValue[2][0],
		theValue[0][1], theValue[1][1], theValue[2][1],
		theValue[0][2], theValue[1][2], theValue[2][2]);
	return inMat;
}
void Space_Creator::add_object(tObject& object)
{
	//object.move(100, 0, 0);
	theObjects.push_back(&object);
}