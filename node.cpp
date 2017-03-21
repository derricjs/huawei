#include "node.h"

int node::seqnumber()
{
	return sequence_number;
}

bool node::service_node()
{
	return is_service_node;
}

int node::consume_number()
{

	return con_number;
}

double node::show_level()
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

int node::show_bandwidth(int seq_number)
{
	return 0;
}

