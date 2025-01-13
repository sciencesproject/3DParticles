#ifndef PARTICLE_H
#define PARTICLE_H

#include "group.h"
#include "vector3D.h"

class Particle {
	public:
		Particle(Group* group, float x, float y, float z);
		Particle(Group* group, Vector3D pos);
		void old_copy();
		void apply_speed();
		float get_distance(Particle* particle, bool use_self_old, bool use_other_old);
		Vector3D get_normal(Particle* particle, bool use_self_old, bool use_other_old);
		
		Group* group;
		Vector3D pos;
		Vector3D old;
		Vector3D speed;
		
};

#endif