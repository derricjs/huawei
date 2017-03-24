#include "solution.h"
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
	double total_bandwidth = 0;
	nodes.resize(net.netnodes);
	string source, end, bandwidth, price;
	for (int i = n; i != n + net.links; ++i)
	{
		record.str(topo[i]);
		record >> source >> end >> bandwidth >> price;
		nettopo[stoi(source)][stoi(end)] = make_pair(stoi(bandwidth), stoi(price));
		nettopo[stoi(end)][stoi(source)] = make_pair(stoi(bandwidth), stoi(price));

		net.price_of_perGbit += stod(bandwidth)*stod(price);
		total_bandwidth += stod(bandwidth);
	}
	for (int j = n + 1 + net.links; j != n + 1 + net.links + net.comsumers; ++j)
	{
		record.str(topo[j]);
		record >> source >> end >> bandwidth;
		nodes[stoi(end)].set_consume_node(stoi(source), stoi(bandwidth));

		net.total_consumption += stoi(bandwidth);
	}
	net.price_of_perGbit = net.price_of_perGbit / total_bandwidth;
}

void solution::print(ostream & os)
{
	os << net.netnodes << " " << net.links << " " << net.comsumers << " " << net.cost_of_server << " " << net.price_of_perGbit << " " << net.total_consumption << endl;
	for (auto it = nettopo.begin(); it != nettopo.end(); ++it)
	{
		os << endl << it->first << "\t";
		for (auto it_sec = it->second.begin(); it_sec != it->second.end(); ++it_sec)
			os << it_sec->first << ends;
	}
	os << endl;
	for (auto it = nettopo.begin(); it != nettopo.end(); ++it)
	{
		os << endl;
		for (auto it_sec = it->second.begin(); it_sec != it->second.end(); ++it_sec)
			os << it->first << ends << it_sec->first << ends << it_sec->second.first << ends << it_sec->second.second << endl;
	}

	for (auto & node : nodes)
		os << node << endl << endl;
}

set<int> solution::search_dev_node(const int max_hops)
{
	vector<set<int>> reached_nodes(net.netnodes);                           //���ÿ���ڵ��ڹ涨�����ڵ�������ѽڵ�
	for (int i = 0; i != net.netnodes; ++i)
	{
		vector<set<int>> node_of_path(max_hops + 1);                         //��ĳһ�ڵ���������ڵ�
		node_of_path[0].insert(i);
		for (int j = 0; j != max_hops + 1; ++j)
		{
			for (auto it = node_of_path[j].cbegin(); it != node_of_path[j].cend(); ++it)
			{
				if (max_hops != j)
					for (auto p = nettopo[*it].cbegin(); p != nettopo[*it].cend(); ++p) //���뵱ǰ�ڵ�������һ���ڵ�
						node_of_path[j + 1].insert(p->first);
				if (-1 != nodes[*it].consume_number())                                 //�жϵ�ǰ�ڵ��Ƿ�Ϊ���ѽڵ�
					reached_nodes[i].insert(nodes[*it].consume_number());
			}

		}



		//int num_of_server = 
#ifdef _DEBUG
		for (auto& hops : node_of_path)
		{
			for (auto & node : hops)
				cout << node << ends;
			cout << endl;
		}
		cout << endl;
#endif
	}

#ifdef _DEBUG
	int k = 0;
	for (auto & nodes : reached_nodes)
	{
		cout << k << "�ڵ���" << max_hops << "�����ڵ�������ѽڵ㣺";
		for (auto & node : nodes)
			cout << node << ends;
		cout << endl;
		++k;
	}
#endif
	return std::set<int>();
}

int solution::get_hops_tables()
{
	hops_tables.resize(net.netnodes);                                                     //��ʾ���нڵ���һϵ�������µ�������ѽڵ�
	vector<set<int>> reached_nodes(net.netnodes);                                         //���ÿ���ڵ��ڹ涨�����ڵ�������ѽڵ�
	int max_hops = 0;                                                                         //�������
	for (int i = 0; i != net.netnodes; ++i)
	{
		auto hops_table = make_shared<vector<set<int>>>();
		hops_table->push_back(set<int>());
		vector<set<int>> node_of_path;                                                   //��ĳһ�ڵ���������ڵ�
		node_of_path.push_back(set<int>{i});

		for (int j = 0; j != INT_MAX; ++j)
		{
			node_of_path.push_back(set<int>());
			for (auto it = node_of_path[j].cbegin(); it != node_of_path[j].cend(); ++it)
			{
				for (auto p = nettopo[*it].cbegin(); p != nettopo[*it].cend(); ++p)      //���뵱ǰ�ڵ�������һ���ڵ�
					node_of_path[j + 1].insert(p->first);
				if (-1 != nodes[*it].consume_number())                                   //�жϵ�ǰ�ڵ��Ƿ�Ϊ���ѽڵ�
					(*hops_table)[j].insert(nodes[*it].consume_number());
			}
			if (net.comsumers == (*hops_table)[j].size())
				break;
			hops_table->push_back(set<int>((*hops_table)[j].cbegin(), (*hops_table)[j].cend()));

		}
		hops_tables[i] = hops_table;
		max_hops = (*hops_table).size() - 1 > max_hops ? (*hops_table).size() - 1 : max_hops;

	}
#ifdef _DEBUG
	int k = 0;
	ofstream os("hops_of_tables.txt");
	os << "���нڵ���������������ѽڵ��ϵ��" << endl << endl;
	for (auto hops_table : hops_tables)
	{
		os << k << "�Žڵ�:" << endl;
		int j = 0;
		for (auto & hop_table : *hops_table)
		{
			os << j << "����" << "\t";
			for (auto & end : hop_table)
			{
				os << end << ends;
			}
			++j;
			os << endl;
		}
		++k;
		os << endl << endl;
	}
#endif
	return max_hops % 2 == 1 ? (max_hops + 1) / 2 : max_hops / 2;

}