#include "node.h"
using namespace std;
int node::node_exist = 0;

int node::seqnumber() const
{
	return sequence_number;
}

bool node::service_node() const
{
	return is_service_node;
}

int node::consume_number() const
{

	return con_number;
}

double node::show_level() const
{
	return level;
}

void node::set_service_node()
{
	is_service_node = true;
}

int node::set_consume_node(int consume_number, int band_consumption)
{
	con_number = consume_number;
	consumption = band_consumption;
	return consumption;
}

double node::set_level()
{
	return 0.0;
}

int node::show_bandwidth(int seq_number) const
{
	return 0;
}

ostream & operator << (std::ostream & os, const node &item)
{
	os << "节点序号：" << "\t" << item.sequence_number << endl << "服务节点:" << "\t" << (item.is_service_node ? "是" : "否") << endl;
	cout << "消费节点：" << "\t";
	item.consume_number() >= 0 ? (cout << "是 " << item.con_number << endl << "消费带宽为:" << "\t" << item.consumption << "Gbps") : cout << "否";
	cout<<endl << "优先级：" << "\t" << item.level;
	return os;
}
