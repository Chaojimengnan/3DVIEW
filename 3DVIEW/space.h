#ifndef SPACE_H
#define SPACE_H
#include <vector>
#include <string>
#include <math.h>
#include <graphics.h>

const int PMX = 640;
const int PMY = 480;




class Vector1x3
{
public:
	float value_x;
	float value_y;
	float value_z;
	Vector1x3();
	Vector1x3(float value_x, float value_y, float value_z);
	Vector1x3 operator+(Vector1x3 &other);
	Vector1x3 operator-(Vector1x3 &other);
};

struct the_Point
{
	bool visible;
	float PM_X;
	float PM_Y;
	Vector1x3 Point;				//相对于自身的坐标
	//Vector1x3 WorldPoint;			//相对于空间的坐标
	std::vector<int> connection;	//储存连接的其他点的索引
};

class Matrix3x3
{
public:
	Matrix3x3(float theta, int surface);
	Matrix3x3(float x1, float x2, float x3, float x4, float x5, float x6, float x7, float x8, float x9);
	void changeData(float theta, int surface);
	void changeData(float theta);
	//void changeData(int surface);
	Matrix3x3 inverse();										//该矩阵的逆矩阵（就是旋转矩阵的转置）
	Matrix3x3 operator*(Matrix3x3 other);							//旋转矩阵*旋转矩阵
private:
	friend Vector1x3 operator*(Matrix3x3 a, Vector1x3 b);			//旋转矩阵*向量
	float theValue[3][3];
	int the_axis;	//内部值
};



class pureObj 
{
public:
	Vector1x3 theCenterPoint;		//中心点
	float xy_theta;			//   rad
	float xz_theta;			//
	float yz_theta;			//
	pureObj(float value_x, float value_y, float value_z);
	void rotate(int axis, float angel);		//axis-1 xy		axis-2 xz	axis=3 yz
	void move(float x, float y, float z,int spaceTo);
};

class tObject :public pureObj
{
public:
	tObject(int Object_TYPE);			//Object_TYPE-1 cube
	tObject(int Object_TYPE, float x, float y, float z);
	//tObject(int Object_TYPE, float x_scale, float y_scale, float z_scale);
	~tObject();
	void putPoint(int Object_TYPE);
private:
	friend class Camera;
	std::vector<the_Point> object_points;	//物体结构点
};

class Camera:public pureObj
{
public:
	int pm;
	float k;      //tan theta    y/x
	Camera(float k, int pm);
	Camera(float k, int pm, float x, float y, float z);
	~Camera();
	void draw(std::vector<tObject*> &theObjects);
};

class Space_Creator
{
public:
	void add_object(tObject& object);
	std::vector<tObject*> theObjects;     //存放物体的容器
};



#endif
