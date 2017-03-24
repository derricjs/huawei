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
	std::set<int> search_dev_node(const int max_hops);    //�������нڵ���max_hops�����ڵ�������ѽڵ�
	int get_hops_tables();                                //�����������������ȡ���������ѽڵ��ϵ��
private:
	network net;                                                       //����ſ�
	std::map<int, std::map<int, std::pair<int, int>>> nettopo;         //�������˽ṹ
	std::vector<node> nodes;                                           //�ڵ�
	std::vector<std::shared_ptr<std::vector<std::set<int>>>> hops_tables;                  //���������ѵ��ϵ��

};


