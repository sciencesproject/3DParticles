#include "vector3D.h"

Vector3D::Vector3D()
	: x(0), y(0), z(0) {}

Vector3D::Vector3D(float x, float y, float z)
	: x(x), y(y), z(z) {}

string Vector3D::as_string() {
	return to_string(this->x) + "; " + to_string(this->y) + "; " + to_string(this->z);
}

Vector3D Vector3D::operator=(Vector3D vector3d) { return Vector3D(vector3d.x, vector3d.y, vector3d.z); }
Vector3D Vector3D::operator=(Vector3D* vector3d) { return Vector3D(vector3d->x, vector3d->y, vector3d->z); }

Vector3D Vector3D::operator+(float factor) { return Vector3D(this->x+factor, this->y+factor, this->z+factor); }
Vector3D Vector3D::operator+(Vector3D* vector3d) { return Vector3D(this->x+vector3d->x, this->y+vector3d->y, this->z+vector3d->z); }
void Vector3D::operator+=(float factor) { this->x+=factor; this->y+=factor; this->z+=factor; }
void Vector3D::operator+=(Vector3D vector3d) { this->x+=vector3d.x; this->y+=vector3d.y; this->z+=vector3d.z; }
void Vector3D::operator+=(Vector3D* vector3d) { this->x+=vector3d->x; this->y+=vector3d->y; this->z+=vector3d->z; }

Vector3D Vector3D::operator-(float factor) { return Vector3D(this->x-factor, this->y-factor, this->z-factor); }
Vector3D Vector3D::operator-(Vector3D vector3d) { return Vector3D(this->x-vector3d.x, this->y-vector3d.y, this->z-vector3d.z); }
Vector3D Vector3D::operator-(Vector3D* vector3d) { return Vector3D(this->x-vector3d->x, this->y-vector3d->y, this->z-vector3d->z); }
void Vector3D::operator-=(float factor) { this->x-=factor; this->y-=factor; this->z-=factor; }
void Vector3D::operator-=(Vector3D* vector3d) { this->x-=vector3d->x; this->y-=vector3d->y; this->z-=vector3d->z; }

Vector3D Vector3D::operator*(float factor) { return Vector3D(this->x*factor, this->y*factor, this->z*factor); }
Vector3D Vector3D::operator*(Vector3D* vector3d) { return Vector3D(this->x*vector3d->x, this->y*vector3d->y, this->z*vector3d->z); }
void Vector3D::operator*=(float factor) { this->x*=factor; this->y*=factor; this->z*=factor; }
void Vector3D::operator*=(Vector3D* vector3d) { this->x*=vector3d->x; this->y*=vector3d->y; this->z*=vector3d->z; }

Vector3D Vector3D::operator/(float factor) { return Vector3D(this->x/factor, this->y/factor, this->z/factor); }
Vector3D Vector3D::operator/(Vector3D* vector3d) { return Vector3D(this->x/vector3d->x, this->y/vector3d->y, this->z/vector3d->z); }
void Vector3D::operator/=(float factor) { this->x/=factor; this->y/=factor; this->z/=factor; }
void Vector3D::operator/=(Vector3D* vector3d) { this->x/=vector3d->x; this->y/=vector3d->y; this->z/=vector3d->z; }