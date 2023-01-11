#ifndef _LISTGRAPH_H_
#define _LISTGRAPH_H_

#include "Graph.h"

using namespace std;

class ListGraph : public Graph {
private:
	map < int, int >* m_List;
	ofstream fout;

public:	
	ListGraph(bool type, int size);
	~ListGraph();
	
	void getIncomingEdges(int to, map<int, int>* m);
	void getAdjacentEdges(int vertex, map<int, int>* m);
	void getDirAdjacentEdges(int vertex, map<int, int>* m);
	void insertEdge(int from, int to, int weight);
	bool printGraph(ofstream &fout);
};

#endif