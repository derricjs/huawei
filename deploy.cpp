#include "deploy.h"
#include <stdio.h>
#include <string>
#include <iostream>
#include <vector>
#include "network.h"
#include "node.h"
using namespace std;
//你要完成的功能总入口
void deploy_server(char * topo[MAX_EDGE_NUM], int line_num, char * filename)
{

	network net;
	map<int, set<int>> nettopo;
	map<pair<int, int>, pair<int, int>> link_status;
	vector<node> nodes;
	net = netbrief(topo, nettopo, link_status, nodes); //生成网络概况(包括网络节点、消费节点、链路条数、服务器成本),网络拓扑结构，链路情况。

	/******************************************输出测试******************************************************/
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
		cout << node << endl<<endl;
	/*********************************************************************************************************/

	int hops_to_consumer = 5;

	// 需要输出的内容
	char * topo_file = (char *)"17\n\n0 8 0 20\n21 8 0 20\n9 11 1 13\n21 22 2 20\n23 22 2 8\n1 3 3 11\n24 3 3 17\n27 3 3 26\n24 3 3 10\n18 17 4 11\n1 19 5 26\n1 16 6 15\n15 13 7 13\n4 5 8 18\n2 25 9 15\n0 7 10 10\n23 24 11 23";

	// 直接调用输出文件的方法输出到指定文件中(ps请注意格式的正确性，如果有解，第一行只有一个数据；第二行为空；第三行开始才是具体的数据，数据之间用一个空格分隔开)
	write_result(topo_file, filename);

}