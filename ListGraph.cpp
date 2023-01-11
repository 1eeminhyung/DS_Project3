#include "ListGraph.h"

ListGraph::ListGraph(bool type, int size) : Graph(type, size)
{
	m_Type = type;
	m_List = new map<int, int>[size];
}

ListGraph::~ListGraph() {
	delete[] m_List;
}

//Implemented by dividing into directed and undirected graphs.
void ListGraph::getAdjacentEdges(int vertex, map<int, int>* m) {
	//Implemented undirected graph
	for(auto iter = m_List[vertex].begin(); iter != m_List[vertex].end(); iter++) {
		m->insert(make_pair(iter->first, iter->second));
	}
	//Also include the edge coming into the vertex. If the edges connecting the two vertex overlap, use an edge with a smaller weight.
	for(int i=0; i<m_Size; i++) {
		if(i == vertex) continue;
		for(auto iter = m_List[i].begin(); iter != m_List[i].end(); iter++) {
			if(iter->first == vertex) {
				m->insert(make_pair(i, iter->second));
			}
		}
	}
	return;
}

void ListGraph::getDirAdjacentEdges(int vertex, map<int, int>* m) {
	for(auto iter = m_List[vertex].begin(); iter != m_List[vertex].end(); iter++) {
		m->insert(make_pair(iter->first, iter->second));
	}
	return;
}

void ListGraph::getIncomingEdges(int to, map<int, int>* m) {
	for(int i=0; i<m_Size; i++) {
		for(auto iter = m_List[i].begin(); iter != m_List[i].end(); iter++) {
			if(iter->first == to) {
				m->insert({i, iter->second});
			}
		}
	}
	return;
}

void ListGraph::insertEdge(int from, int to, int weight) {
	m_List[from].insert({to, weight});		//insert Map
	return;
}

bool ListGraph::printGraph(ofstream& fout) {
	fout << "Graph is ListGraph!" << endl;

	for(int i=0; i<m_Size; i++) {
		fout << "[" << i << "]";
		for(auto it_=m_List[i].begin(); it_!=m_List[i].end() && fout<<" -> "; it_++) {
			fout << "(" << it_->first << "," << it_->second << ")";
		}
		fout << endl;
	}
	return 1;
}

