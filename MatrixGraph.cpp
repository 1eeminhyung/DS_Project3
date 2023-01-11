#include "MatrixGraph.h"

MatrixGraph::MatrixGraph(bool type, int size) : Graph(type, size) {
	m_Type = type;
	m_Mat = new int*[size];
	for(int i=0; i<size; i++)
	{
		m_Mat[i] = new int[size];
		memset(m_Mat[i], 0, sizeof(int)*size);
	}
}

MatrixGraph::~MatrixGraph() {
	for(int i=0; i<getSize(); i++)
	{
		delete[] m_Mat[i];
	}
	delete[] m_Mat;
}

//Implemented by dividing into directed and undirected graphs.
void MatrixGraph::getAdjacentEdges(int vertex, map<int, int>* m) {
	//Implemented undirected graph
	for(int i=0; i<m_Size; i++) {
		if(m_Mat[vertex][i] != 0) m->insert(make_pair(i, m_Mat[vertex][i]));
	}
	for(int i=0; i<m_Size; i++) {
		if(m_Mat[i][vertex] != 0) m->insert(make_pair(i, m_Mat[vertex][i]));
	}
	return;
}

void MatrixGraph::getDirAdjacentEdges(int vertex, map<int, int>* m) {
	//Implemented directed graph
	for(int i=0; i<m_Size; i++) {
		if(m_Mat[vertex][i] != 0) m->insert(make_pair(i, m_Mat[vertex][i]));
	}
	return;
}

void MatrixGraph::getIncomingEdges(int to, map<int, int>* m) {
	//Obtained incoming edge entering the corresponding vertex.
	for(int i=0; i<m_Size; i++) {
		if(m_Mat[i][to] != 0) m->insert({i, m_Mat[i][to]});
	}
	return;
}

void MatrixGraph::insertEdge(int from, int to, int weight) {
	//Insert Edge.
	m_Mat[from][to] = weight;
	return;
}

bool MatrixGraph::printGraph(ofstream& fout) {
	if( m_Size < 0 )
		return 0;

	fout << "Graph is MatrixGraph!" << endl;
	fout << '\t';
	for(int i=0; i<m_Size; i++) {
		fout << "[" << i << "]" << '\t';
	}
	fout << endl;

	for(int i=0; i<m_Size; i++) {
		fout << "[" << i << "]";
		for(int j=0; j<m_Size && fout<<'\t'; j++) {
			fout << m_Mat[i][j];
		}
		fout << endl;
	}
	return 1;
}
