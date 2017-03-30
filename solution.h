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
	int netnodes;       //����ڵ���
	int links;          //������·��
	int comsumers;      //���ѽڵ����� 
	int cost_of_server; //����������
	int total_consumption; //����������
	double price_of_perGbit; //��λ����۸�
};

class solution
{
public:
	solution(char * topo[MAX_EDGE_NUM]); //����������ת��Ϊ��Ӧ���ݽṹ
	void print(std::ostream & os);
	std::vector<int> search_dev_node(const int hops);        //�������нڵ���max_hops�����ڵ�������ѽڵ�
	int get_hops_tables(int mhops = 100);                                //�����������������ȡ���������ѽڵ��ϵ��
	std::vector<std::vector<int>> routing(std::vector<int>& servers);                                       //·��
private:
	void set_nodes_level(std::vector<int>& servers);
	network net;                                                       //����ſ�
	std::map<int, std::map<int, std::pair<int, int>>> nettopo;         //�������˽ṹ,�ֱ��ʾ�ڵ�š��ڵ�š���һ�ڵ�����ǰһ�ڵ�Ĵ�����λ�������÷���
	std::map<int, std::vector<int>> links;
	std::vector<node> nodes;                                           //�ڵ�
	std::vector<std::shared_ptr<std::vector<std::set<int>>>> hops_tables;                  //���������ѵ��ϵ��
	std::map<int, std::pair<int, int>> consumers;                            //���ѽڵ������������ͨ�ڵ����������
	std::vector<int> order_of_consumer;             //���ѽڵ����˳��
	int MAX_HOPS = 0;
	unsigned long s;
	struct timeb rawtime;
};


