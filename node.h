#pragma once
#include <iostream>
#include <set>
class node
{
public:
	node(int snum, bool ser_node, int con_node, int consumption, int level) :sequence_number(snum), is_service_node(ser_node), con_number(con_node),consumption(consumption), level(level) { ++node_exist; }
	node() :node(node_exist, false, -1, 0, 1000) {};
	//node(int snum, double level ) :node(snum,false,-1,0,0){};
	int seqnumber() const;                                                            //���ؽڵ����
	bool service_node() const;                                                        //�Ƿ�Ϊ����ڵ�
	int consume_number() const;                                                       //��ʾ�����ӵ����ѽڵ����
	double show_level() const;                                                        //��ʾ���ȼ�
	void set_service_node(bool is_server);                                                          //����Ϊ����ڵ�
	int set_consume_node(int consume_number, int band_consumption);                   //����Ϊ�����ѽڵ����ڣ����������Ѵ���
	void set_level(int level);                                                               //���ýڵ����ȼ���
	int show_bandwidth(int seq_number) const;                                         //��ʾ������ڵ����������ڽڵ㵽�ýڵ�Ĵ���������
	friend std::ostream & operator << (std::ostream & os, const node &item);          //��������������
private:
	static int node_exist;  //��ǰ����ڵ���������ʼΪ�㣬����Ĭ�Ϲ��캯��
	int sequence_number;    //�ڵ���ţ�
	bool is_service_node;   //�Ƿ�Ϊ����ڵ㣻
	int con_number;         //���ѽڵ�ţ����������ѽڵ���Ϊ-1��
	int consumption;        //���ӵ����ѽڵ��������
 	//int bandwidth;          //���ӵĴ���
	int level;           //�ڵ����ȼ���
};
std::ostream & operator << (std::ostream & os, const node &item);