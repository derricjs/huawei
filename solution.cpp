#include "solution.h"
using namespace std;
solution::solution(char * topo[MAX_EDGE_NUM])
{
	ftime(&rawtime);
	s = rawtime.time;

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
		links[stoi(source)].push_back(stoi(end));
		links[stoi(end)].push_back(stoi(source));
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

vector<int> solution::search_dev_node(const int current_hops)
{
	//int amount_of_server = ceil((1 - net.comsumers)*current_hops / static_cast<double>(MAX_HOPS)) + net.comsumers;  //服务器数量
	int amount_of_server = net.comsumers / 2;
	vector<int> candidate_server(net.netnodes);        //候选服务节点
	vector<int> servers;
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
	order_of_consumer.resize(net.comsumers); //寻路时遍历消费节点的顺序；
	int j = 0;
	for (auto & ele : order_of_consumer)
		ele = j++;
	auto h = [&](const int & j, const int & k) {return consumers[j].second < consumers[k].second; };
	sort(order_of_consumer.begin(), order_of_consumer.end(), h);         //消费节点寻路排序，以消耗量为依据
	int k = 0;
	for (auto it = candidate_server.cbegin(); it != candidate_server.cend(); ++it)
	{
		++k;
		complementary.insert((*hops_tables[*it])[current_hops].cbegin(), (*hops_tables[*it])[current_hops].cend());
		if (size_of_complementary < complementary.size() || net.comsumers == size_of_complementary)
		{
			servers.push_back(*it);
			count_of_consumer.insert((*hops_tables[*it])[current_hops].cbegin(), (*hops_tables[*it])[current_hops].cend());
			size_of_complementary = complementary.size();
			nodes[*it].set_service_node(true);

			for (auto node = nettopo[*it].cbegin(); node != nettopo[*it].cend(); ++node)
				throughtput += node->second.first;

			if (amount_of_server == servers.size() || (net.comsumers == size_of_complementary && throughtput > 3 * net.total_consumption))
			{
				auto q = [&](const int & j, const int & k) {return count_of_consumer.count(j) < count_of_consumer.count(k); };
				stable_sort(order_of_consumer.begin(), order_of_consumer.end(), q);               //对消费节点寻路排序，以当前跳数下到达设施点数为依据
				return servers;
			}
		}
	}
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

vector<vector<int>> solution::routing(vector<int>& servers)
{
	vector<vector<int>> routes;
	vector<int> route;
	set<int> number_of_server_used;
	int max_hops_of_routing = 7;//最大允许路由跳数

	for (int j = 0; j != 1000; ++j)
	{
		set_nodes_level(servers);
		for (auto & consumer : order_of_consumer)
		{
			if (0 == consumers[consumer].second)
				continue;
			route.clear();
			route.insert(route.begin(), { 0,consumer, consumers[consumer].first });
			int min_bandwidth = 1000; //路由上的最小带宽
			if (0 == nodes[consumers[consumer].first].show_level())
			{
				route[0] = consumers[consumer].second;
				consumers[consumer].second = 0;
				routes.push_back(route);
				continue;
			}
				
			for (int i = 2; i != max_hops_of_routing + 2; ++i)
			{
				auto f = [&](const int j, const int k) {return (nodes[j].show_level() + nettopo[route[i]][j].second) < (nodes[k].show_level() + nettopo[route[i]][j].second); };
				sort(links[route[i]].begin(), links[route[i]].end(), f);
				if (nodes[route[i]].show_level() < nodes[links[route[i]][0]].show_level())
					break;
				route.push_back(links[route[i]][0]);
				if (min_bandwidth > nettopo[route[i]][route[i + 1]].first)
					min_bandwidth = nettopo[route[i]][route[i + 1]].first;
				if (0 == nodes[route[i + 1]].show_level())
				{

					route[0] = min_bandwidth < consumers[consumer].second ? min_bandwidth : consumers[consumer].second;
					consumers[consumer].second -= route[0];
					for (auto it = route.begin() + 2; it != route.end() - 1; ++it)
					{
						nettopo[*it][*(it + 1)].first -= route[0];
						if (0 == nettopo[*it][*(it + 1)].first)
							links[*it].erase(links[*it].begin());
					}
					routes.push_back(route);

					break;
				}
			}

		}
		static int Q = 0, num = 0;
		int q = 0;
		for (auto& consumer : consumers)
		{
			if (0 == consumer.second.second)
				++q;
		}
		(Q == q) ? ++num : num = 0;
		Q = q;
		ftime(&rawtime);
		if (4 == num || rawtime.time - s > 60)
		{
			//vector<int> direct_mode;
			for (auto& consumer : consumers)
			{
				if (consumer.second.second != 0)
				{

					for (auto it = routes.begin(); it != routes.end(); ++it)
					{

						for (auto p = it->begin() + 2; p != it->end(); ++p)
						{
							if (*p == consumer.second.first)
							{
								it->erase(++p, it->end());
								break;
							}
						}
					}
					routes.push_back(vector<int>{ consumer.second.second, consumer.first, consumer.second.first});
				}

			}
			return routes;
		}
		for (auto& route : routes)
		{
			number_of_server_used.insert(route.back());
		}
		cout << q << ends << number_of_server_used.size() << endl;
	}
	return routes;
}

void solution::set_nodes_level(vector<int>& servers)
{
	vector<set<int>> node_of_path;
	for (auto & node : nodes)
		node.set_level(100);
	for (auto & server : servers)
	{
		nodes[server].set_level(0);
		node_of_path.push_back(set<int>{server});
		for (int i = 0; i != 5; ++i)
		{
			node_of_path.push_back(set<int>());
			for (auto it = node_of_path[i].cbegin(); it != node_of_path[i].cend(); ++it)
			{
				for (auto p = nettopo[*it].cbegin(); p != nettopo[*it].cend(); ++p)
				{
					if (0 != nettopo[p->first][*it].first)
					{
						int new_level = nodes[*it].show_level() + p->second.second;
						if (new_level < nodes[p->first].show_level())
						{
							nodes[p->first].set_level(new_level);
							node_of_path[i + 1].insert(p->first);
						}
					}
				}
			}
		}
	}
}
