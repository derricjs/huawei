#pragma once
#include "lib_io.h"
#include <map>
#include <set>
#include <vector>
#include "node.h"
struct network
{
	int netnodes;       //网络节点数
	int links;          //网络链路数
	int comsumers;      //消费节点数量 
	int cost_of_server; //服务器费用
};

network netbrief(char * topo[MAX_EDGE_NUM], std::map<int, std::set<int>> & nettopology, std::map<std::pair<int, int>, std::pair<int, int>> & linkstatus, std::vector<node> & nodes);