#include "Manager.h"

Manager::Manager() {
	graph = nullptr;
	fout.open("log.txt", ios::app);		//open log.txt. If there is a file created, continue to write it.
	load = 0;
}

Manager::~Manager() {
	if(fout.is_open())
		fout.close();
}

void Manager::run(const char* command_txt) {
	ifstream fin;
	fin.open(command_txt);
	//if cannot open command_txt, print error message
	if(!fin) {
		fout << "[ERROR] command file open error!" <<endl;
		return;
	}
	char cmd[256]={ 0 };
	while(!fin.eof()) {
		fin.getline(cmd, 256);

		bool result;
		char* ptr = strtok(cmd, " ");
		if(ptr == nullptr) continue;
		if(ptr[0] == '/' || ptr[1] == '/') continue;
		if(strcmp(ptr, "LOAD") == 0) {
			//If Graph is exist, delete & make new Graph
			ptr = strtok(NULL, " ");
			if(load) {
				//Clear and recreate the graph if it had already existed before.
				delete graph;
				load = 0;
			}
			try {
				result = LOAD(ptr);
				if(!result) throw 100;
				fout << "=========== LOAD ==========" << endl;
				fout << "Success" << endl;
				fout << "===========================" << endl << endl;
				load = 1;
			}
			catch(int errorcode) {
				printErrorCode(errorcode);
			}
		}
		else if(strcmp(ptr, "PRINT") == 0) {
			try {
				if(!load) throw 200;	//if Graph does not Exist, Print Errorcode
				fout << "========= PRINT =========" << endl;
				result = PRINT();
				if(!result) throw 200;
				fout << "=========================" << endl << endl;
			}
			catch(int errorcode) {
				printErrorCode(errorcode);
			}			
		}
		else if(strcmp(ptr, "BFS") == 0) {
			ptr = strtok(NULL, " ");
			try {
				if(ptr == NULL) throw 300;	//No Factor
				if(!load) throw 300;		//If graph does not exist
				int vertex = atoi(ptr);
				if(vertex >= graph->getSize()) throw 300;	//If the entered vertex does not exist in the graph.
				fout << "========= BFS ===========" << endl;
				result = mBFS(vertex);
				if(!result) throw 300;
				fout << "=========================" << endl << endl;
			}
			catch(int errorcode) {
				printErrorCode(errorcode);
			}
			
		}
		else if(strcmp(ptr, "DFS") == 0) {
			ptr = strtok(NULL, " ");
			try {
				if(ptr == NULL) throw 400;	//No Factor
				if(!load) throw 400;		//If graph does not exist
				int vertex = atoi(ptr);
				if(vertex >= graph->getSize()) throw 400;	//If the entered vertex does not exist in the graph.
				fout << "========= DFS ==========" << endl;
				result = mDFS(vertex);
				if(!result) throw 400;
				fout << "========================" << endl << endl;
			}
			catch(int errorcode) {
				printErrorCode(errorcode);
			}
		}
		else if(strcmp(ptr, "DFS_R") == 0) {
			ptr = strtok(NULL, " ");
			try {
				if(ptr == NULL) throw 500;	//No Factor
				if(!load) throw 500;		//If graph does not exist
				int vertex = atoi(ptr);
				if(vertex >= graph->getSize()) throw 500;	//If the entered vertex does not exist in the graph.
				fout << "========== DFS_R ==========" << endl;
				result = mDFS_R(vertex);
				if(!result) throw 500;
				fout << "===========================" << endl << endl;
			}
			catch(int errorcode) {
				printErrorCode(errorcode);
			}
		}
		else if(strcmp(ptr, "KRUSKAL") == 0) {
			try {
				if(!load) throw 600;	//If graph does not exist
				fout << "========= Kruskal =========" << endl;
				result = mKRUSKAL();
				if(!result) throw 600;
				fout << "===========================" << endl << endl;
			}
			catch(int errorcode) {
				printErrorCode(errorcode);
			}
		}
		else if(strcmp(ptr, "DIJKSTRA") == 0) {
			ptr = strtok(NULL, " ");
			try {
				if(!load) throw 700;	//If graph does not exist
				if(ptr == NULL) throw 700;	//If none of the factors exist, output an error code.
				int Vertex = atoi(ptr);
				fout << "========= Dijkstra =========" << endl;
				result = mDIJKSTRA(Vertex);		//If negative weight exist, result is false.
				if(!result) throw 700;
				fout << "============================" << endl << endl;
			}
			catch(int errorcode) {
				printErrorCode(errorcode);
			}
		}
		else if(strcmp(ptr, "BELLMANFORD") == 0) {
			ptr = strtok(NULL, " ");
			try {
				if(!load) throw 800;	//If graph does not exist
				if(ptr == NULL) throw 800;	//If none of the factors exist, output an error code.
				int start = atoi(ptr);
				ptr = strtok(NULL, " ");
				if(ptr == NULL) throw 800;
				int end = atoi(ptr);
				fout << "======= Bellman-Ford =======" << endl;
				result = mBELLMANFORD(start, end);
				if(!result) throw 800;	//if Negative cycle exists, result is false.
				fout << "============================" << endl << endl;
			}
			catch(int errorcode) {
				printErrorCode(errorcode);
			}
		}
		else if(strcmp(ptr, "FLOYD") == 0) {
			try {
				if(!load) throw 900;	//If graph does not exist
				fout << "========== Floyd ========" << endl;
				result = mFLOYD();
				if(!result) throw 900;		//if Negative cycle exists, result is false.
				fout << "=========================" << endl << endl;
			}
			catch(int errorcode) {
				printErrorCode(errorcode);
			}
		}
		else if(strcmp(ptr, "EXIT") == 0) {
			fin.close();
			delete graph;
			load = 0;
			return;
		}
	}
}

bool Manager::LOAD(char* filename) {
	ifstream fload;
	bool type;
	int size = 0;
	//Decide whether to make it List or Matrix according to the file name.
	fload.open(filename);	
	char buf[64];
	memset(buf, -1, 64*sizeof(char));
	if(!fload) return false;		//can't open file, return false

	fload.getline(buf, 64);
	if(buf[0] == NULL) return false;		//If nothing exists in the file
	//type 1->Matrix
	//type 0->List
	if(buf[0] == 'M') type = 1;	
	else if(buf[0] == 'L') type = 0;

	fload.getline(buf, 5);
	char* ptr = strtok(buf, " ");
	size = atoi(ptr);

	//LOAD ListGraph
	//Read Vertex & Edge
	int StartVertex = 0;
	if(type == 0) {
		graph = new ListGraph(type, size);
		//Store values in adjacency list.
		while(!fload.eof()) {
			memset(buf, -1, 64 * sizeof(char));
			fload.getline(buf, 64);
			char* ptr = strtok(buf, " ");
			int temp = atoi(ptr);
			ptr = strtok(NULL, " ");
			if(ptr == NULL) {
				StartVertex = temp;
			}
			else {
				int toVertex = temp;
				int weight = atoi(ptr);
				graph->insertEdge(StartVertex, toVertex, weight);
			}
		}
	}
	else {		//LOAD MatrixGraph
		graph = new MatrixGraph(type, size);
		char buffer[256];
		int from=0;
		//Store values in adjacency matrix.
		while(!fload.eof()) {
			fload.getline(buffer, 256);
			int toVertex = 0;
			char* ptr = strtok(buffer, " ");
			while(ptr != NULL) {
				int weight = atoi(ptr);
				if(weight != 0) {
					graph->insertEdge(from, toVertex, weight);
				}
				++toVertex;
				ptr = strtok(NULL, " ");
			}
			from++;
		}
	}
	fload.close();
	return true;
}

//Print function.
bool Manager::PRINT() {
	if(graph->printGraph(fout))
		return true;
	return false;
}

//BFS function
bool Manager::mBFS(int vertex) {
	if(BFS(graph, vertex, fout)) 
		return true;
	return false;
}

//DFS function using stack
bool Manager::mDFS(int vertex) {
	if(DFS(graph, vertex, fout)) 
		return true;
	return false;
}

//DFS function using Recursive function
bool Manager::mDFS_R(int vertex) {
	int size = graph->getSize();
	vector<bool> visited(size);
	fout << "startvertex : " << vertex << endl;
	if(DFS_R(graph, visited, vertex, fout)) {		
		fout << endl;
		return true;
	}
	return false;
}

//Dijkstra function
bool Manager::mDIJKSTRA(int vertex) {
	if(Dijkstra(graph, vertex, fout))
		return true;
	return false;
}

//kruskal function
bool Manager::mKRUSKAL() {
	if(Kruskal(graph, fout))
		return true;
	return false;
}

//Bellman-Ford function
bool Manager::mBELLMANFORD(int s_vertex, int e_vertex) {
	if(Bellmanford(graph, s_vertex, e_vertex, fout))
		return true;
	return false;
}

//Floyd function
bool Manager::mFLOYD() {
	if(FLOYD(graph, fout))
		return true;
	return false;
}

//Print Errorcode.
void Manager::printErrorCode(int n) {
	fout << "======== ERROR ========" << endl;
	fout << n << endl;
	fout << "=======================" << endl << endl;
}
