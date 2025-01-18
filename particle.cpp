#include <cmath>

#include "particle.h"

Particle::Particle(Group* group, float x, float y, float z)
	: group(group), pos(Vector3D(x, y, z)), old(Vector3D(x, y, z)), speed(Vector3D(0, 0, 0)) {}

Particle::Particle(Group* group, Vector3D pos)
	: group(group), pos(pos), old(pos), speed(Vector3D(0, 0, 0)) {}

void Particle::old_copy() {
	this->old = this->pos;
}

void Particle::apply_speed() {
	this->pos += this->speed;
}

float Particle::get_distance(Particle* particle, bool use_self_old, bool use_other_old) {
	if (use_self_old && use_other_old) {
		float d = pow((
			pow(this->old.x - particle->old.x, 2) +
			pow(this->old.y - particle->old.y, 2) +
			pow(this->old.z - particle->old.z, 2)
		), 0.5);
		return d;
	}
	 
	if (use_self_old && !use_other_old) {
		float d = pow((
			pow(this->old.x - particle->pos.x, 2) +
			pow(this->old.y - particle->pos.y, 2) +
			pow(this->old.z - particle->pos.z, 2)
		), 0.5);
		return d;
	}
	
	if (!use_self_old && use_other_old) {
		float d = pow((
			pow(this->pos.x - particle->old.x, 2) +
			pow(this->pos.y - particle->old.y, 2) +
			pow(this->pos.z - particle->old.z, 2)
		), 0.5);
		return d;
	}
	
	float d = pow((
		pow(this->pos.x - particle->pos.x, 2) +
		pow(this->pos.y - particle->pos.y, 2) +
		pow(this->pos.z - particle->pos.z, 2)
	), 0.5);
	return d;
}

Vector3D Particle::get_normal(Particle* particle, bool use_self_old, bool use_other_old) {
	if (use_self_old && use_other_old) {
		float sum = (particle->old - this->old).get_absolute_sum();
		if (sum != 0) return (particle->old - this->old) / sum;
		else return Vector3D(0, 0, 0);
	}
	if (use_self_old && !use_other_old) return (particle->old - this->pos) / this->get_distance(particle, use_self_old, use_other_old);
	if (!use_self_old && use_other_old) return (particle->pos - this->old) / this->get_distance(particle, use_self_old, use_other_old);
	return (particle->pos - this->pos) / this->get_distance(particle, use_self_old, use_other_old);
}