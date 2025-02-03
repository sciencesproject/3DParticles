#include <cmath>

#include "simulation.h"
#include "vector3D.h"

#include <iostream>
using namespace std;

Simulation::Simulation(float air_resistance, float max_range)
	: air_resistance(air_resistance), max_range(max_range), particles((vector<Particle*>){}) {}

Simulation::Simulation(float air_resistance, float max_range, vector<Particle*> particles)
	: air_resistance(air_resistance), max_range(max_range), particles(particles) {}

void Simulation::run(int steps) {
	for (int step = 0; step < steps; step++) this->run();
}

void Simulation::run() {
	for (auto &particle : this->particles) {
		particle->old_copy();
		particle->speed *= (1-air_resistance);
	}
	
	for (auto &particle : this->particles) {
		vector<Particle*> near_particles = {};
		for (auto &other : this->particles) if (particle != other) {
			if (particle->get_distance(other, true, true) < this->max_range) near_particles.push_back(other);
		}
		
		for (auto &other : near_particles) {
			float distance = particle->get_distance(other, true, true);
			float interaction_value;
			if (particle->group == other->group) interaction_value = particle->group->self_interaction;
			else {
				bool found = false;
				for (auto &other_interaction : other->group->interactions) {
					if (other_interaction.first == particle->group) {
						interaction_value = other_interaction.second;
						found = true;
						break;
					}
				}
				if (!found) for (auto &other_interaction : particle->group->interactions) {
					if (other_interaction.first == other->group) {
						interaction_value = other_interaction.second;
						break;
					}
				}
			}
			float interaction_strength = interaction_value / pow(distance, distance);
			Vector3D add_speed = particle->get_normal(other, true, true) * interaction_strength;
			particle->speed += add_speed;
		}
		particle->apply_speed();
	}
}