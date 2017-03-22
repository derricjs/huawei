#pragma once
#include "lib_io.h"
#include <map>
#include <set>
#include <vector>
#include "node.h"
#include <iostream>
struct network
{
	int netnodes;       //����ڵ���
	int links;          //������·��
	int comsumers;      //���ѽڵ����� 
	int cost_of_server; //����������
};

class solution
{
public:
	solution(char * topo[MAX_EDGE_NUM]); //����������ת��Ϊ��Ӧ���ݽṹ
	void print(std::ostream & os);
	std::set<int> search_dev_node(const int max_hops);    //�������нڵ���max_hops�����ڵ�������ѽڵ�
private:
	network net;                                                       //����ſ�
	std::map<int, std::set<int>> nettopo;                              //�������˽ṹ
	std::map<std::pair<int, int>, std::pair<int, int>> link_status;    //��·״��
	std::vector<node> nodes;                                           //�ڵ�


};


