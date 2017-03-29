#pragma once
#include <iostream>
#include <set>
class node
{
public:
	node(int snum, bool ser_node, int con_node, int consumption, int level) :sequence_number(snum), is_service_node(ser_node), con_number(con_node),consumption(consumption), level(level) { ++node_exist; }
	node() :node(node_exist, false, -1, 0, 1000) {};
	//node(int snum, double level ) :node(snum,false,-1,0,0){};
	int seqnumber() const;                                                            //返回节点序号
	bool service_node() const;                                                        //是否为服务节点
	int consume_number() const;                                                       //显示所连接的消费节点序号
	double show_level() const;                                                        //显示优先级
	void set_service_node(bool is_server);                                                          //设置为服务节点
	int set_consume_node(int consume_number, int band_consumption);                   //设置为与消费节点相邻，并设置消费带宽
	void set_level(int level);                                                               //设置节点优先级；
	int show_bandwidth(int seq_number) const;                                         //显示除输入节点外其他相邻节点到该节点的带宽余量；
	friend std::ostream & operator << (std::ostream & os, const node &item);          //重载输出运算符；
private:
	static int node_exist;  //当前网络节点数量，初始为零，用于默认构造函数
	int sequence_number;    //节点序号；
	bool is_service_node;   //是否为服务节点；
	int con_number;         //消费节点号，不连接消费节点则为-1；
	int consumption;        //连接的消费节点所需带宽；
 	//int bandwidth;          //连接的带宽
	int level;           //节点优先级；
};
std::ostream & operator << (std::ostream & os, const node &item);