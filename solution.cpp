#include "solution.h"
#include <sstream>
#include <string>
using namespace std;
solution::solution(char * topo[MAX_EDGE_NUM])
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
		link_status[make_pair(stoi(end), stoi(source))] = make_pair(stoi(bandwidth), stoi(price));
		nettopo[stoi(source)].insert(stoi(end));
		nettopo[stoi(end)].insert(stoi(source));
	}
	for (int j = n + 1 + net.links; j != n + 1 + net.links + net.comsumers; ++j)
	{
		record.str(topo[j]);
		record >> source >> end >> bandwidth;
		nodes[stoi(end)].set_consume_node(stoi(source), stoi(bandwidth));
	}
}

void solution::print(ostream & os)
{
#ifdef _DEBUG 
	os << net.netnodes << " " << net.links << " " << net.comsumers << " " << net.cost_of_server << endl;
	for (auto it = nettopo.begin(); it != nettopo.end(); ++it)
	{
		os << endl << it->first << "\t";
		for (auto it_sec = it->second.begin(); it_sec != it->second.end(); ++it_sec)
			os << *it_sec << ends;
	}
	os << endl;
	for (auto it = link_status.begin(); it != link_status.end(); ++it)
	{
		os << it->first.first << " " << it->first.second << " " << it->second.first << " " << it->second.second << endl;
	}

	for (auto & node : nodes)
		os << node << endl << endl;
#endif
}

set<int> solution::search_dev_node(const int max_hops)
{
	vector<set<int>> reached_nodes(net.netnodes);                           //存放每个节点在规定跳数内到达的消费节点
	for (int i = 0; i != net.netnodes; ++i)
	{
		vector<set<int>> node_of_path(max_hops + 1);                         //从某一节点出发所经节点
		node_of_path[0].insert(i);
		for (int j = 0; j != max_hops + 1; ++j)
		{
			for (auto it = node_of_path[j].cbegin(); it != node_of_path[j].cend(); ++it)
			{
				if (max_hops != j)
					node_of_path[j + 1].insert(nettopo[*it].cbegin(), nettopo[*it].cend());
				if (-1 != nodes[*it].consume_number())                                 //判断当前节点是否为消费节点
					reached_nodes[i].insert(nodes[*it].consume_number());
			}

		}
		for (auto& hops : node_of_path)
		{
			for (auto & node : hops)
				cout << node << ends;
			cout << endl;
		}
		cout << endl;
	}
	int k = 0;
	for (auto & nodes : reached_nodes)
	{
		cout << k << "节点在" << max_hops << "跳数内到达的消费节点：";
		for (auto & node : nodes)
			cout << node << ends;
		cout << endl;
		++k;
	}

	return std::set<int>();
}
