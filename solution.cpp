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
		links[stoi(source)].insert(stoi(end));
		links[stoi(end)].insert(stoi(source));
		net.price_of_perGbit += stod(bandwidth)*stod(price);
		total_bandwidth += stod(bandwidth);
	}
	for (int j = n + 1 + net.links; j != n + 1 + net.links + net.comsumers; ++j)
	{
		record.str(topo[j]);
		record >> source >> end >> bandwidth;
		nodes[stoi(end)].set_consume_node(stoi(source), stoi(bandwidth));
		consumers[stoi(source)] = make_pair(stoi(end), stoi(bandwidth));
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

set<int> solution::search_dev_node(const int current_hops)
{
	//int amount_of_server = ceil((1 - net.comsumers)*current_hops / static_cast<double>(MAX_HOPS)) + net.comsumers;  //服务器数量
	int amount_of_server = net.comsumers / 4;
	vector<int> candidate_server(net.netnodes);        //候选服务节点
	set<int> servers;
	int i = 0;
	for (auto & element : candidate_server)
	{
		element = i++;
	}
	auto f = [this, current_hops](const int j, const int k) {return (*hops_tables[j])[current_hops].size() > (*hops_tables[k])[current_hops].size(); };
	auto g = [this](const int j, const int k) -> bool
	{
		int j_bandwidth = 0, k_bandwidth = 0;
		for (auto it = nettopo[j].cbegin(); it != nettopo[j].cend(); ++it)
		{
			j_bandwidth += it->second.first;
		}
		for (auto it = nettopo[k].cbegin(); it != nettopo[k].cend(); ++it)
		{
			k_bandwidth += it->second.first;
		}
		return j_bandwidth > k_bandwidth;
	};
	sort(candidate_server.begin(), candidate_server.end(), g);           // 按节点吞吐量排序         
	stable_sort(candidate_server.begin(), candidate_server.end(), f);   //在上次排序的基础上，按当前跳数到达消费节点数量排序

	set<int> complementary;      //互补性检查
	multiset<int> count_of_consumer; //重复关键字的complementary
	int throughtput = 0;  //候选服务点输出总带宽；
	set<int>::size_type size_of_complementary = 0;
	vector<int> order_of_consumer(net.comsumers); //寻路时遍历消费节点的顺序；
	int j = 0;
	for (auto & ele : order_of_consumer)
		ele = j++;
	auto h = [&](const int & j, const int & k) {return consumers[j].second > consumers[k].second; };
	sort(order_of_consumer.begin(), order_of_consumer.end(), h);         //消费节点寻路排序，以消耗量为依据
	int k = 0;
	for (auto it = candidate_server.cbegin(); it != candidate_server.cend(); ++it)
	{
		++k;
		complementary.insert((*hops_tables[*it])[current_hops].cbegin(), (*hops_tables[*it])[current_hops].cend());
		if (size_of_complementary < complementary.size() || net.comsumers == size_of_complementary)
		{
			servers.insert(*it);
			count_of_consumer.insert((*hops_tables[*it])[current_hops].cbegin(), (*hops_tables[*it])[current_hops].cend());
			size_of_complementary = complementary.size();
			nodes[*it].set_service_node(true);

			for (auto node = nettopo[*it].cbegin(); node != nettopo[*it].cend(); ++node)
				throughtput += node->second.first;

			if (amount_of_server  == servers.size()  ||(net.comsumers == size_of_complementary && throughtput > net.total_consumption))
			{
				auto q = [&](const int & j, const int & k) {return count_of_consumer.count(j) < count_of_consumer.count(k); };
				stable_sort(order_of_consumer.begin(), order_of_consumer.end(), q);               //对消费节点寻路排序，以当前跳数下到达设施点数为依据
				return servers;
			}
		}
	}
	k;
	return servers;



	//	auto seed = candidate_server.begin() + amount_of_server - 1;
	//	
	//	for (auto source = seed; source != candidate_server.cbegin(); --source)
	//	{
	//		for (auto dest = seed; dest != candidate_server.end(); ++dest)
	//		{
	//			swap(*source, *dest);
	//			complementary.clear();
	//			count_of_consumer.clear();
	//			for (auto it = candidate_server.cbegin(); it != candidate_server.cbegin() + amount_of_server; ++it)
	//			{
	//				complementary.insert((*hops_tables[*it])[current_hops].cbegin(), (*hops_tables[*it])[current_hops].cend());
	//				count_of_consumer.insert((*hops_tables[*it])[current_hops].cbegin(), (*hops_tables[*it])[current_hops].cend());
	//			}
	//			if (net.comsumers == complementary.size())
	//			{
	//				int throughtput = 0;  //候选服务点输出总带宽；
	//				for (auto it = candidate_server.cbegin(); it != candidate_server.cbegin() + amount_of_server; ++it)
	//					for (auto node = nettopo[*it].cbegin(); node != nettopo[*it].cend(); ++node)
	//						throughtput += node->second.first;
	//				if (throughtput > net.total_consumption)
	//				{
	//					auto q = [&](const int & j, const int & k) {return count_of_consumer.count(j) < count_of_consumer.count(k); };
	//					stable_sort(order_of_consumer.begin(), order_of_consumer.end(), q);               //对消费节点寻路排序，以当前跳数下到达设施点数为依据
	//					for (auto & node : nodes)
	//						node.set_service_node(false);
	//					for (auto it = candidate_server.cbegin(); it != candidate_server.cbegin() + amount_of_server; ++it)
	//						nodes[*it].set_service_node(true);
	//
	//#ifdef _TEST
	//					for (auto it = candidate_server.cbegin(); it != candidate_server.cbegin() + amount_of_server; ++it)
	//						cout << *it << ends;
	//					cout << endl;
	//					for (auto it = count_of_consumer.cbegin(); it != count_of_consumer.cend(); ++it)
	//						cout << *it << ends;
	//					cout << endl;
	//					for (auto it = order_of_consumer.cbegin(); it != order_of_consumer.cend(); ++it)
	//						cout << *it << ends;
	//					cout << endl << endl;
	//#endif
	//					return set<int>(candidate_server.cbegin(), candidate_server.cbegin() + amount_of_server);
	//				}
	//
	//				}
	//			if (size_of_complementary < complementary.size())
	//			{
	//				size_of_complementary = complementary.size();
	//				continue;
	//			}
	//			if (size_of_complementary == complementary.size())
	//			{
	//				int source_bandwidth = 0, dest_bandwidth = 0;
	//				for (auto it = nettopo[*source].cbegin(); it != nettopo[*source].cend(); ++it)
	//				{
	//					source_bandwidth += it->second.first;
	//				}
	//				for (auto it = nettopo[*dest].cbegin(); it != nettopo[*dest].cend(); ++it)
	//				{
	//					dest_bandwidth += it->second.first;
	//				}
	//				if (source_bandwidth >= dest_bandwidth && dest >= candidate_server.cbegin() + amount_of_server)
	//					continue;
	//			}
	//			swap(*source, *dest);
	//			}
	//		}
	//
	//	auto q = [&](const int & j, const int & k) {return count_of_consumer.count(j) < count_of_consumer.count(k); };
	//	sort(order_of_consumer.begin(), order_of_consumer.end(), q);
	//	for (auto & node : nodes)
	//		node.set_service_node(false);
	//	for (auto it = candidate_server.cbegin(); it != candidate_server.cbegin() + amount_of_server; ++it)
	//		nodes[*it].set_service_node(true);
	//	return set<int>(candidate_server.cbegin(), candidate_server.cbegin() + amount_of_server);
}

int solution::get_hops_tables(int mhops)
{
	hops_tables.resize(net.netnodes);                                                     //表示所有节点在一系列跳数下到达的消费节点
	vector<set<int>>::size_type max_hops = 0;                                                                     //最大跳数
	for (int i = 0; i != net.netnodes; ++i)
	{
		auto hops_table = make_shared<vector<set<int>>>();                               //存放某一结点相应跳数到达的消费节点
		hops_table->push_back(set<int>());
		vector<set<int>> node_of_path;                                                   //从某一节点出发所经节点
		node_of_path.push_back(set<int>{i});

		for (int j = 0; j != mhops + 1; ++j)
		{
			node_of_path.push_back(set<int>());
			for (auto it = node_of_path[j].cbegin(); it != node_of_path[j].cend(); ++it)
			{


				if (-1 != nodes[*it].consume_number())                                   //判断当前节点是否为消费节点
					(*hops_table)[j].insert(nodes[*it].consume_number());
				//for (auto p = nettopo[*it].cbegin(); p != nettopo[*it].cend(); ++p)      //加入当前节点所有下一跳节点
				//	node_of_path[j + 1].insert(p->first);
				node_of_path[j + 1].insert(links[*it].cbegin(), links[*it].cend());
			}
			if (net.comsumers == (*hops_table)[j].size())
				break;
			hops_table->push_back(set<int>((*hops_table)[j].cbegin(), (*hops_table)[j].cend()));

		}
		hops_tables[i] = hops_table;
		max_hops = (*hops_table).size() - 1 > max_hops ? (*hops_table).size() - 1 : max_hops;

	}
#ifdef _TEST
	int k = 0;
	ofstream os("hops_of_tables.txt");
	os << "所有节点跳数与所达的消费节点关系：" << endl << endl;
	for (auto hops_table : hops_tables)
	{
		os << k << "号节点:" << endl;
		int j = 0;
		for (auto & hop_table : *hops_table)
		{
			os << j << "跳：" << "\t";
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
	return MAX_HOPS = max_hops % 2 == 1 ? (max_hops + 1) / 2 : max_hops / 2;

}

void solution::routing()
{

}
