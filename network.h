#pragma once
#include "lib_io.h"
#include <map>
#include <set>
struct network
{
	int netnodes;
	int links;
	int comsumers;
	int cost_of_server;
};

network netbrief(char * topo[MAX_EDGE_NUM], std::map<int, std::set<int>> & nettopology, std::map<std::pair<int, int>, std::pair<int ,int>> & linkstatus);