#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
using namespace std;
#include <cmath>

#include "group.h"
#include "particle.h"
#include "simulation.h"
#include "vector3D.h"

int total_particles = 100;

int main(int argc, char** argv) {
	Group g(.8);
	Simulation sim(.8, 15);
	vector<Particle> particles;
	particles.reserve(total_particles);
	for (int i = 0; i<total_particles; i++) {
		particles.push_back(Particle(&g, i%(int)pow(total_particles, 0.5), floor(i/pow(total_particles, 0.5)), 0));
		std::remove(((string)("p" + to_string(i) + ".obj")).c_str());
		ofstream pf("p" + to_string(i) + ".obj");
		pf.close();
	}
	for (auto &p : particles) sim.particles.push_back(&p);
	
	for (int i = 0; i<200; i++) {
		std::remove(("all" + to_string(i) + ".obj").c_str());
		ofstream allf("all" + to_string(i) + ".obj");
		for (auto &p : particles) allf << "v " << p.pos.x << " " << p.pos.y << " " << p.pos.z << endl;
		allf.close();
		
		sim.run();
		for (int i = 0; i<particles.size(); i++) {
			ofstream pf("p" + to_string(i) + ".obj", std::ios_base::app);
			pf << "v " << particles[i].pos.x << " " << particles[i].pos.y << " " << particles[i].pos.z << endl;
			pf.close();
		}
	}
	
	return 0;
}