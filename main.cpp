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

int main(int argc, char** argv) {
	Group g(.1);
	Simulation sim(0);
	vector<Particle> particles;
	for (int i = 0; i<4; i++) particles.push_back(Particle(&g, i%2-.5, floor(i/2)-0.5, 0));
	for (auto &p : particles) sim.particles.push_back(&p);
	
	std::remove("p2.obj");
	ofstream p2f("p2.obj");
	Particle* p2 = &particles[1];
	p2f << "v " << p2->pos.x << " " << p2->pos.y << " " << p2->pos.z << endl;
	for (int i = 0; i<10; i++) {
		std::remove(("all" + to_string(i) + ".obj").c_str());
		ofstream allf("all" + to_string(i) + ".obj");
		for (auto &p : particles) allf << "v " << p.pos.x << " " << p.pos.y << " " << p.pos.z << endl;
		allf.close();
		
		sim.run();
		p2f << "v " << p2->pos.x << " " << p2->pos.y << " " << p2->pos.z << endl;
	}
	p2f.close();
	return 0;
}