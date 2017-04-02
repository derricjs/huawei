#include "deploy.h"
#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include "solution.h"
#include "node.h"

using namespace std;
//你要完成的功能总入口
void deploy_server(char * topo[MAX_EDGE_NUM], int line_num, char * filename)
{

	solution first_edit(topo);//生成解决方案类，包括网络概况(包括网络节点、消费节点、链路条数、服务器成本),网络拓扑结构，链路情况。

	/******************************************输出测试******************************************************/
#ifdef _TEST
	ofstream out("network.txt");
	first_edit.print(out);
#endif
	/*********************************************************************************************************/
	int MAX_HOPS = first_edit.get_hops_tables();
	vector<int> servers3(first_edit.search_dev_node(2));
	vector<vector<int>> routes = first_edit.routing(servers3);

	// 需要输出的内容
	ostringstream os;
	os << routes.size() << endl << endl;
	for (auto &route : routes)
	{
		for (auto it = route.crbegin(); it != route.crend(); ++it)
		{
			os << *it;
			it != --route.crend() ? (os << " ") : (os << endl);
		}


	}
	string result = os.str();

	//char * topo_file = ;//(char *)"17\n\n0 8 0 20\n21 8 0 20\n9 11 1 13\n21 22 2 20\n23 22 2 8\n1 3 3 11\n24 3 3 17\n27 3 3 26\n24 3 3 10\n18 17 4 11\n1 19 5 26\n1 16 6 15\n15 13 7 13\n4 5 8 18\n2 25 9 15\n0 7 10 10\n23 24 11 23";

	// 直接调用输出文件的方法输出到指定文件中(ps请注意格式的正确性，如果有解，第一行只有一个数据；第二行为空；第三行开始才是具体的数据，数据之间用一个空格分隔开)
	write_result(result.c_str(), filename);

}