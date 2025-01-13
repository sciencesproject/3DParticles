#include "group.h"

Group::Group()
	: self_interaction(0), interactions((vector<pair<Group*, float>>){}) {}

Group::Group(pair<Group*, float> interaction)
	: self_interaction(0), interactions((vector<pair<Group*, float>>){interaction}) {}

Group::Group(vector<pair<Group*, float>> interactions)
	: self_interaction(0), interactions(interactions) {}

Group::Group(float self_interaction)
	: self_interaction(self_interaction), interactions((vector<pair<Group*, float>>){}) {}

Group::Group(float self_interaction, pair<Group*, float> interaction)
	: self_interaction(self_interaction), interactions((vector<pair<Group*, float>>){interaction}) {}

Group::Group(float self_interaction, vector<pair<Group*, float>> interactions)
	: self_interaction(self_interaction), interactions(interactions) {}