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

class solution 
{public:
	solution(char * topo[MAX_EDGE_NUM]); //将读入数据转换为相应数据结构
	void print();
	std::set<int> search_dev_node(const int max_hops);
private:
	network net;                                                       //网络概况
	std::map<int, std::set<int>> nettopo;                              //网络拓扑结构
	std::map<std::pair<int, int>, std::pair<int, int>> link_status;    //链路状况
	std::vector<node> nodes;                                           //节点


};


