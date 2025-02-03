#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
#include <chrono>
#include <numeric>
using namespace std;
#include <cmath>
#include <cstdlib>
#include <csignal>

#include "group.h"
#include "particle.h"
#include "simulation.h"
#include "vector3D.h"

bool all_obj = true;
bool group_obj = false;
bool particle_obj = false;
bool file_log = false;
bool term_log = true;
int seed = 100;
float simulation_range = 15;
float simulation_air_resistance = .7;
int total_steps = 30;
int step_log = 1;
int step_start = 0; // TODO : implement
vector<int> total_particles = {};
vector<Group> groups = {};

void ex() {
	ofstream outfile("D:\\fin.log", std::ios_base::app);
	outfile << "OK" << endl;
	outfile.close();
}

int main(int argc, char** argv) {
	std::atexit(ex);
	auto lam = [] (int s) { ex(); };
	signal(SIGINT, lam);
	
	ifstream config(argv[1]);
	if (config.is_open()) {
		try {
			config >> (bool&)all_obj;
			config >> (bool&)group_obj;
			config >> (bool&)particle_obj;
			config >> (bool&)file_log;
			config >> (bool&)term_log;
			config >> (int&)seed;
			config >> (float&)simulation_air_resistance;
			config >> (int&)total_steps;
			config >> (int&)step_log;
			string line;
			config >> line;
			do {
				total_particles.push_back(stof(line));
				config >> line;
				groups.push_back(Group(stof(line)));
				config >> line;
			} while (line.find("#") == string::npos);
		} catch (exception e) {
			cout << "[ERROR] unable to correctly parse file [ERROR]" << endl;
			exit(1);
			return 1;
		}	
	} else {
		cout << "[WARNING] invalid input file, using default settings [WARNING]" << endl;
		total_particles.push_back(10);
		groups.push_back(Group(.3));
	}
	config.close();
	for (int p = 0; p<total_particles.size(); p++) total_particles[p] = (int)pow(total_particles[p], 3);
	
	if (file_log) std::remove("steps.log");
	Simulation sim(simulation_air_resistance, simulation_range);
	vector<Particle> particles;
	particles.reserve(accumulate(total_particles.begin(), total_particles.end(), (int)0));
	srand(seed);
	for (int p = 0; p<total_particles.size(); p++) {
		for (int i = 0; i<total_particles[p]; i++) {
			particles.push_back(Particle(
				&groups[p],
				i%(int)ceil(pow(total_particles[p], 0.3333333)) + (seed ? ((float)(rand()%100)/100) : 0),
				(int)floor(i/pow(total_particles[p], 0.3333333))%(int)ceil(pow(total_particles[p], 0.3333333)) + (seed ? ((float)(rand()%100)/100) : 0),
				(int)floor(i/(int)floor(pow((int)ceil(pow(total_particles[p], 0.3333333)), 2))) + (seed ? ((float)(rand()%100)/100) : 0)
			));
			if (particle_obj) {
				std::remove(((string)("p" + to_string(i) + ".obj")).c_str());
				ofstream pf("p" + to_string(i) + ".obj");
				pf.close();
			}
		}
	}
	for (auto &p : particles) sim.particles.push_back(&p);
	
	for (int step = step_start; step<total_steps; step+=step_log) {
		if (all_obj) {
			for (int j = step-step_log+1; j!=step; j++) { std::remove(("all" + to_string(j) + ".obj").c_str()); }
			ofstream allf("all" + to_string(step) + ".obj");
			for (auto &p : particles) allf << "v " << p.pos.x << " " << p.pos.y << " " << p.pos.z << "\n";
			allf.close();
		}
		if (group_obj) {
			vector<ofstream> gf{};
			for (int i = 0; i<groups.size(); i++) { gf.push_back(ofstream("group" + to_string(i) + "_" + to_string(step) + ".obj")); }
			for (auto &p : particles) {
				int groupi;
				for (int i = 0; i<groups.size(); i++) { if (&(groups[i]) == p.group) { groupi = i; }}
				gf[groupi] << "v " << p.pos.x << " " << p.pos.y << " " << p.pos.z << "\n";
			}
			for (auto &f : gf) { f.close(); }
		}
		
		auto t1 = chrono::high_resolution_clock::now();
		sim.run(step_log);
		auto t2 = chrono::high_resolution_clock::now();
		
		for (int i = 0; i<particles.size() && particle_obj; i++) {
			ofstream pf("p" + to_string(i) + ".obj", std::ios_base::app);
			pf << "v " << particles[i].pos.x << " " << particles[i].pos.y << " " << particles[i].pos.z << endl;
			pf.close();
		}
		if (file_log) {
			ofstream outfile("steps.log", std::ios_base::app);
			outfile << step << "\n" << (chrono::duration_cast<chrono::milliseconds>(t2 - t1)).count() << endl;
			outfile.close();
		}
		if (term_log) cout << step << "\t" << (chrono::duration_cast<chrono::milliseconds>(t2 - t1)).count() << " ms"<< endl;
	}
	
	exit(0);
	return 0;
}