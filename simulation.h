#ifndef SIMULATION_H
#define SIMULATION_H

#include <vector>
using namespace std;

#include "particle.h"

class Simulation {
	public:
		Simulation();
		Simulation(vector<Particle*> particles);
		Simulation(float air_resistance);
		Simulation(float air_resistance, vector<Particle*> particles);
		void run(int steps); // execute run steps times
		void run();
		
		float air_resistance;
		vector<Particle*> particles;
};

#endif