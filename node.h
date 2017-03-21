#pragma once
class node
{
public:
	node(int snum, bool ser_node, int con_node, int consumption, double level) :sequence_number(snum), is_service_node(ser_node),con_number(con_node),level(level) {};
	node() :node(0,false,-1,0,0){};
	//node(int snum, double level ) :node(snum,false,-1,0,0){};
	int seqnumber();
	bool service_node();
	int consume_number();
	double show_level();
	void set_service_node();
	int set_consume_node(int consume_number, int band_consumption);
	double set_level();
	int show_bandwidth(int seq_number); //��ʾ������ڵ����������ڽڵ㵽�ýڵ�Ĵ���������
private:
	int sequence_number;    //�ڵ���ţ�
	bool is_service_node;   //�Ƿ�Ϊ����ڵ㣻
	int con_number;         //���ѽڵ�ţ����������ѽڵ���Ϊ-1��
	int consumption;        //���ӵ����ѽڵ��������
	double level;           //�ڵ����ȼ���


};