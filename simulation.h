#ifndef SIMULATION_H
#define SIMULATION_H

#include <vector>
using namespace std;

#include "particle.h"

class Simulation {
	public:
		Simulation(float air_resistance, float max_range);
		Simulation(float air_resistance, float max_range, vector<Particle*> particles);
		void run(int steps); // execute run steps times
		void run();
		
		float air_resistance;
		float max_range;
		vector<Particle*> particles;
};

#endif