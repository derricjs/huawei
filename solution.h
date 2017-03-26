#pragma once
#include "lib_io.h"
#include <map>
#include <set>
#include <vector>
#include "node.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <memory>
#include <algorithm>
struct network
{
	int netnodes;       //网络节点数
	int links;          //网络链路数
	int comsumers;      //消费节点数量 
	int cost_of_server; //服务器费用
	int total_consumption; //带宽总需求
	double price_of_perGbit; //单位带宽价格
};

class solution
{
public:
	solution(char * topo[MAX_EDGE_NUM]); //将读入数据转换为相应数据结构
	void print(std::ostream & os);
	std::set<int> search_dev_node(const int hops);        //返回所有节点在max_hops跳数内到达的消费节点
	int get_hops_tables();                                //返回最大条数，并获取跳数与消费节点关系表
private:
	network net;                                                       //网络概况
	std::map<int, std::map<int, std::pair<int, int>>> nettopo;         //网络拓扑结构
	std::vector<node> nodes;                                           //节点
	std::vector<std::shared_ptr<std::vector<std::set<int>>>> hops_tables;                  //跳数与消费点关系表
	std::map<int, std::pair<int, int>> consumers;                            //消费节点与相关联的普通节点与带宽需求
	int MAX_HOPS = 0;
};


