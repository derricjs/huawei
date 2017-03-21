#include "network.h"
#include <sstream>
#include <string>
using namespace std;
network netbrief (char * topo[MAX_EDGE_NUM], map<int, set<int>> & nettopology, map<pair<int, int>, pair<int, int>> & linkstatus)
{
	network net;
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
	string source, end, bandwidth, price;
	for (int i = n; i != n + net.links; ++i)
	{
		record.str(topo[i]);
		record >> source >> end >> bandwidth >> price ;
		linkstatus[make_pair(stoi(source), stoi(end))] = make_pair(stoi(bandwidth), stoi(price));
		nettopology[stoi(source)].insert(stoi(end));
	}
	return net;
}