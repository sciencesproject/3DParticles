#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
using namespace std;
#include <cmath>
#include<cstdlib>

#include "group.h"
#include "particle.h"
#include "simulation.h"
#include "vector3D.h"

int total_particles = 512;

int main(int argc, char** argv) {
	Group g(.6);
	Simulation sim(.7, 15);
	vector<Particle> particles;
	particles.reserve(total_particles);
	for (int i = 0; i<total_particles; i++) {
		srand(i);
		particles.push_back(Particle(
			&g,
			i%(int)ceil(pow(total_particles, 0.3333333)) + ((float)(rand()%100)/100),
			(int)floor(i/pow(total_particles, 0.3333333))%(int)ceil(pow(total_particles, 0.3333333)) + ((float)(rand()%100)/100),
			(int)floor(i/(int)floor(pow((int)ceil(pow(total_particles, 0.3333333)), 2))) + ((float)(rand()%100)/100)
		));
		cout << particles[i].pos.as_string() << endl;
		std::remove(((string)("p" + to_string(i) + ".obj")).c_str());
		ofstream pf("p" + to_string(i) + ".obj");
		pf.close();
	}
	for (auto &p : particles) sim.particles.push_back(&p);
	
	for (int i = 0; i<20; i++) {
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
		cout << i << endl;
	}
	
	return 0;
}