#include "solution.h"
#include <sstream>
#include <string>
using namespace std;
solution::solution( char * topo[MAX_EDGE_NUM])
{
	string num;
	istringstream record(topo[0]);
	record >> num;
	net.netnodes = stoi(num);
	record >> num;
	net.links = stoi(num);
	record >> num;
	net.comsumers = stoi(num);
	record.str(topo[2]);
	record >> num;
	net.cost_of_server = stoi(num);
	int n = 4;
	nodes.resize(net.netnodes);
	string source, end, bandwidth, price;
	for (int i = n; i != n + net.links; ++i)
	{
		record.str(topo[i]);
		record >> source >> end >> bandwidth >> price;
		link_status[make_pair(stoi(source), stoi(end))] = make_pair(stoi(bandwidth), stoi(price));
		nettopo[stoi(source)].insert(stoi(end));
	}
	for (int j = n + 1 + net.links; j != n + 1 + net.links + net.comsumers; ++j)
	{
		record.str(topo[j]);
		record >> source >> end >> bandwidth;
		nodes[stoi(end)].set_consume_node(stoi(source), stoi(bandwidth));
	}
}

void solution::print()
{
#ifdef _DEBUG 
	cout << net.netnodes << " " << net.links << " " << net.comsumers << " " << net.cost_of_server << endl;
	for (auto it = nettopo.begin(); it != nettopo.end(); ++it)
	{
		cout << endl << it->first << "\t";
		for (auto it_sec = it->second.begin(); it_sec != it->second.end(); ++it_sec)
			cout << *it_sec << ends;
	}
	cout << endl;
	for (auto it = link_status.begin(); it != link_status.end(); ++it)
	{
		cout << it->first.first << " " << it->first.second << " " << it->second.first << " " << it->second.second << endl;
	}

	for (auto & node : nodes)
		cout << node << endl << endl;
#endif
}

set<int> solution::search_dev_node(const int max_hops)
{
	vector<set<int>> path(net.netnodes);
	//for (auto )

	return std::set<int>();
}
