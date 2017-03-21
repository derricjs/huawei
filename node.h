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
	int show_bandwidth(int seq_number); //显示除输入节点外其他相邻节点到该节点的带宽余量；
private:
	int sequence_number;    //节点序号；
	bool is_service_node;   //是否为服务节点；
	int con_number;         //消费节点号，不连接消费节点则为-1；
	int consumption;        //连接的消费节点所需带宽；
	double level;           //节点优先级；


};