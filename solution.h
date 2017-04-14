#pragma once
#include "lib_io.h"
#include <map>
#include <set>
#include <unordered_set>
#include <vector>
#include "node.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <memory>
#include <algorithm>
#include <time.h>
#include <sys/timeb.h>
//#define _TEST
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
	int networktype();
	void print(std::ostream & os);
	std::vector<int> search_dev_node(const int hops);        //返回所有节点在max_hops跳数内到达的消费节点
	int get_hops_tables(int mhops = 10);                                //返回最大条数，并获取跳数与消费节点关系表
	std::vector<std::vector<int>> routing(std::vector<int>& servers, int& cost);                                       //路由
private:
	void set_nodes_level(std::vector<int>& servers);
	network net;                                                       //网络概况
	std::map<int, std::map<int, std::pair<int, int>>> nettopo;         //网络拓扑结构,分别表示节点号、节点号、后一节点流向前一节点的带宽、单位带宽租用费用
	std::map<int, std::vector<int>> links;
	std::vector<node> nodes;                                           //节点
	std::vector<std::shared_ptr<std::vector<std::set<int>>>> hops_tables;                  //跳数与消费点关系表
	std::map<int, std::pair<int, int>> consumers;                            //消费节点与相关联的普通节点与带宽需求
	std::vector<int> order_of_consumer;             //消费节点遍历顺序
	int MAX_HOPS = 0;
	unsigned long s;
	struct timeb rawtime;
};


