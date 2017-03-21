#pragma once
#include "lib_io.h"
#include <map>
#include <set>
#include <vector>
#include "node.h"
struct network
{
	int netnodes;       //����ڵ���
	int links;          //������·��
	int comsumers;      //���ѽڵ����� 
	int cost_of_server; //����������
};

network netbrief(char * topo[MAX_EDGE_NUM], std::map<int, std::set<int>> & nettopology, std::map<std::pair<int, int>, std::pair<int, int>> & linkstatus, std::vector<node> & nodes);