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

int node::show_level() const
{
	return level;
}

void node::set_service_node(bool is_server)
{
	is_service_node = is_server;
}

int node::set_consume_node(int consume_number, int band_consumption)
{
	con_number = consume_number;
	consumption = band_consumption;
	return consumption;
}

void node::set_level(int new_level)
{
	level = new_level;
}

int node::show_bandwidth(int seq_number) const
{
	return 0;
}

ostream & operator << (std::ostream & os, const node &item)
{
	os << "�ڵ���ţ�" << "\t" << item.sequence_number << endl << "����ڵ�:" << "\t" << (item.is_service_node ? "��" : "��") << endl;
	os << "���ѽڵ㣺" << "\t";
	item.consume_number() >= 0 ? (os << "�� " << item.con_number << endl << "���Ѵ���Ϊ:" << "\t" << item.consumption << "Gbps") : os << "��";
	os<<endl << "���ȼ���" << "\t" << item.level;
	return os;
}
