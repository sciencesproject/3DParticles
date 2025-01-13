#ifndef GROUP_H
#define GROUP_H

#include <vector>
#include <utility>
using namespace std;

class Group {
	public:
		Group();
		Group(pair<Group*, float> interaction);
		Group(vector<pair<Group*, float>> interactions);
		Group(float self_interaction);
		Group(float self_interaction, pair<Group*, float> interaction);
		Group(float self_interaction, vector<pair<Group*, float>> interactions);
		
		float self_interaction; // strenght, positiv for atraction
		vector<pair<Group*, float>> interactions; // float is strenght, positiv for atraction
};

#endif