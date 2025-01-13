#include <cmath>

#include "simulation.h"
#include "vector3D.h"

#include <iostream>
using namespace std;

Simulation::Simulation()
	: air_resistance(0), particles((vector<Particle*>){}) {}

Simulation::Simulation(vector<Particle*> particles)
	: air_resistance(0), particles(particles) {}

Simulation::Simulation(float air_resistance)
	: air_resistance(air_resistance), particles((vector<Particle*>){}) {}

Simulation::Simulation(float air_resistance, vector<Particle*> particles)
	: air_resistance(air_resistance),  particles(particles) {}

void Simulation::run(int steps) {
	for (int step; step != steps; step++) this->run();
}

void Simulation::run() {
	for (auto &particle : this->particles) {
		particle->old_copy();
		particle->speed *= (1-air_resistance);
	}
	
	for (auto &particle : this->particles) {
		vector<Particle*> near_particles = {};
		for (auto &other : this->particles) if (particle != other) {
			if (particle->get_distance(other, true, true) < 10) near_particles.push_back(other);
		}
		
		for (auto &other : near_particles) {
			float distance = particle->get_distance(other, true, true);
			float interaction_strenght;
			if (particle->group == particle->group) interaction_strenght = pow(particle->group->self_interaction, 2) / distance;
			else {
				for (auto &other_interaction : particle->group->interactions) {
					if (other_interaction.first == other->group) {
						interaction_strenght = pow(other_interaction.second, 2) / distance;
					}
				}
			}
			Vector3D add_speed = particle->get_normal(other, true, true) * interaction_strenght;
			particle->speed += add_speed;
			particle->apply_speed();
		}
	}
}