#include <cmath>

#include "simulation.h"
#include "vector3D.h"

#include <thread>
#include <iostream>
using namespace std;

Simulation::Simulation(float air_resistance, float max_range, int max_thread)
	: air_resistance(air_resistance), max_range(max_range), max_thread(max_thread), particles((vector<Particle*>){}) {}

Simulation::Simulation(float air_resistance, float max_range, int max_thread, vector<Particle*> particles)
	: air_resistance(air_resistance), max_range(max_range), max_thread(max_thread), particles(particles) {}

void Simulation::run(int steps) {
	for (int step = 0; step < steps; step++) this->run();
}

void Simulation::run() {
	for (auto &particle : this->particles) {
		particle->old_copy();
		particle->speed *= (1-air_resistance);
	}
	
	vector<thread> threads = {};
	int split = this->particles.size() / this->max_thread;
	for (int t = 0; t < max_thread; t++) {
		threads.push_back(thread(compute, this, t*split, ((t+1)*split)-1));
	}
	for (auto& t : threads) { t.join(); }
}

void compute(Simulation* sim, int begin, int end) {
	for (int i = begin; i<end; i++) {
		auto &particle = sim->particles[i];
		vector<Particle*> near_particles = {};
		for (auto &other : sim->particles) if (particle != other) {
			if (particle->get_distance(other, true, true) < sim->max_range) near_particles.push_back(other);
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