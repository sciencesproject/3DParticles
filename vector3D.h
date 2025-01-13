#ifndef VECTOR3D_H
#define VECTOR3D_H

#include <string>
using namespace std;

class Vector3D {
	public:
		Vector3D();
		Vector3D(float x, float y, float z);
		string as_string();
		
		Vector3D operator+(float factor);
		Vector3D operator+(Vector3D* vector3d);
		void operator+=(float factor);
		void operator+=(Vector3D vector3d);
		void operator+=(Vector3D* vector3d);
		Vector3D operator-(float factor);
		Vector3D operator-(Vector3D vector3d);
		Vector3D operator-(Vector3D* vector3d);
		void operator-=(float factor);
		void operator-=(Vector3D* vector3d);
		Vector3D operator*(float factor);
		Vector3D operator*(Vector3D* vector3d);
		void operator*=(float factor);
		void operator*=(Vector3D* vector3d);
		Vector3D operator/(float factor);
		Vector3D operator/(Vector3D* vector3d);
		void operator/=(float factor);
		void operator/=(Vector3D* vector3d);
		
		float x;
		float y;
		float z;
};

#endif