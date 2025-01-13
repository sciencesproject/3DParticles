#include <iostream>
#include <string>
#include <vector>
using namespace std;
#include <cmath>

#include "group.h"
#include "particle.h"
#include "simulation.h"
#include "vector3D.h"

int main(int argc, char** argv) {
	Group g(.5);
	Simulation sim(.2);
	vector<Particle> particles;
	for (int i = 0; i<2; i++) {
		particles.push_back(Particle(&g, i%10, floor(i/10), 0));
		sim.particles.push_back(&particles[i]);
	}
	Particle* p2 = &particles[0];
	cout << "v " << p2->pos.x << " " << p2->pos.y << " " << p2->pos.z << endl;
	for (int i = 0; i<10; i++) {
		sim.run();
		// cout << "v " << p2->pos.x << " " << p2->pos.y << " " << p2->pos.z << endl;
	}                                      
	return 0;
}