#include "GraphMethod.h"


bool BFS(Graph* graph, int vertex, ofstream& fout) {
    int size = graph->getSize();
    if(size == 0) return false;     //Returns false if the graph does not exist.

    bool* visited = new bool[size];
    memset(visited, 0, size*sizeof(bool));
    visited[vertex] = true;
    fout << "start vertex : " << vertex << endl;    //Print Start vetertx, and visit processing
    queue<int> q;
    map<int, int> Adj;                      //Map that stores adjacent nodes and weights for each Vertex.
    q.push(vertex);
    int i=0;
    while(!q.empty()) {
        int tmp = q.front();
        if(i+1 == size) fout << tmp;        //pop of queue and print
        else fout << tmp << " -> ";
        i++;   
        q.pop();
        Adj.clear();            //clear Map
        graph->getAdjacentEdges(tmp, &Adj);
        for(const auto& elem : Adj) {
            int x = elem.first;         //Push into the queue for non-visited nodes among adjacent nodes and visit processing.
            if(!visited[x]) {
                q.push(x);
                visited[x] = true;
            }
        }
    }
    fout << endl;
    Adj.clear();
    delete[] visited;
    return true;
}

bool DFS(Graph* graph, int vertex, ofstream& fout) {
    int size = graph->getSize();
    if(size == 0) return false;       //if graph does not exist, return false

    bool* visited = new bool[size];
    memset(visited, 0, size * sizeof(bool));
    visited[vertex] = true;
    fout << "startvertex : " << vertex << endl;     //Print StartVertex and visit processing
    fout << vertex;
    stack<int> s;
    map<int, int> Adj;      //Map that stores adjacent nodes and weights for each Vertex.
    s.push(vertex);

    while(!s.empty()) {
        int tmp = s.top();              //pop of queue and print
        s.pop();
        Adj.clear();
        graph->getAdjacentEdges(tmp, &Adj);     //get Adjacent Edges
        for(const auto elem : Adj) {
            int next = elem.first;
            if(!visited[next]) {        //If not visit yet
                fout << " -> " << next;     //Process the visit and insert tmp and next
                visited[next] = true;
                s.push(tmp);
                s.push(next);
                break;                  //break, because operate DFS function
            }
        }
    }
    fout << endl;
    delete[] visited;
    return true;
}

bool DFS_R(Graph* graph, vector<bool>& visit, int vertex, ofstream& fout) {
    int size = graph->getSize();
    if(size == 0) return false;     //No graph
    visit[vertex] = true;
    fout << vertex;
    map<int, int> Adj;      //Map that stores adjacent nodes and weights for each Vertex.
    graph->getAdjacentEdges(vertex, &Adj);
    for(auto iter = Adj.begin(); iter != Adj.end(); iter++) {
        int next = iter->first;
        if(!visit[next]) {
            fout << " -> ";
            DFS_R(graph, visit, next, fout);       //Recursive
        }
    }
    Adj.clear();
    return true;
}

int SimpleFind(int i, int* parent) {            //Using Simple Find.
    if(parent[i] == i) return i;                //If parents are that node, return it
    else return parent[i] = SimpleFind(parent[i], parent);  //Recursive
}

void SimpleUnion(int i, int j, int* parent) {     //Using simple Union.
    parent[j] = i;      //Update j's parent to i
    return;
}

void insertionSort(vector<pair<int, pair<int, int>>>& v, const int left, const int right) {
    int i,j;
    for(i=left+1; i<=right; i++) {
        auto key = v[i];
        for(j=i-1; j>=left; j--) {
            if(v[j].first > key.first) v[j+1] = v[j];   //Push the value one by one and find a place to insert.
            else break;
        }
        v[j+1] = key;           //Insert
    }
}

void QuickSort(vector<pair<int, pair<int, int>>>& v, int left, int right) {
    /* If the segment size is 6 or less, insert alignment is performed, 
    and if it is 7 or more, perform QuickSort using pivot.  */
    if(left < right) {
        if(right - left + 1 <= 6) {
            insertionSort(v, left, right);
        }
        else {
            int i = left;
            int j = right + 1;
            int pivot = v[left].first;              //Set pivot leftMostValue.
            do {
                do i++; while(v[i].first < pivot);  //Repeat until an index is larger than the pivot.
                do j--; while(v[j].first > pivot);  //Repeat until an index is smaller than the pivot.
                if(i < j) swap(v[i], v[j]);         //Swap if i<j
            } while(i < j);
            swap(v[left], v[j]);                    //Swap v[j] with Pivot
            QuickSort(v, left, j-1);                //Recursive
            QuickSort(v, j+1, right);
        }
    }
    return;
}

bool Kruskal(Graph* graph, ofstream& fout) {
    int m_Size = graph->getSize();      //Get graph Size.
    int cost = 0;                       //Cost required to obtain a minimum spanning tree using the Kruskal algorithm.

    /*Obtain weights for all edges. first value is weight, and
    The second value with the pair value is the value stored
     in order of the coming vertex and the thin vertex.*/
    vector<pair<int, pair<int, int>>> edgeSet;      
    map<int, int> Adj;      //Map that stores adjacent nodes and weights for each Vertex.
    for(int i=0; i<m_Size; i++) {
        Adj.clear();
        graph->getDirAdjacentEdges(i, &Adj);
        for(const auto& elem : Adj) {
            edgeSet.push_back({elem.second, {i, elem.first}});
        }
    }
    //Using quickSort & InsertionSort
    QuickSort(edgeSet, 0, edgeSet.size() - 1);

    int numofEdge = 0;
    map<int, int> m[m_Size];
    int parent[m_Size];
    for(int i=0; i<m_Size; i++) parent[i] = i;      //Initialization
    for(int i=0; i<edgeSet.size();) {
        //MST contains less than n-1 edges && EgdeSet not Empty
        if(edgeSet.empty() || numofEdge > m_Size-1) break;  
        //p1 is edgeSet[i].second.first's parent
        //p2 is edgeSet[i].second.second's parent
        int p1 = SimpleFind(edgeSet[i].second.first, parent);
        int p2 = SimpleFind(edgeSet[i].second.second, parent);

        if(p1 != p2) {      //Have different parent.
            m[edgeSet[i].second.first].insert({edgeSet[i].second.second, edgeSet[i].first});    //Path from the starting point
            m[edgeSet[i].second.second].insert({edgeSet[i].second.first, edgeSet[i].first});
            SimpleUnion(p1, p2, parent);
            cost += edgeSet[i].first;
            auto iter = edgeSet.begin() + i;
            edgeSet.erase(iter);            //Discard Edge.
            numofEdge++;
        }
        else i++;
    }
    if(numofEdge < m_Size-1) return false;  //graph has no spanning tree, return false 

    for(int i=0; i<m_Size; i++) {
        fout << "[" << i << "]" << " ";
        for(auto iter = m[i].begin(); iter != m[i].end(); iter++) {
            fout << iter->first << "(" << iter->second << ")" << " ";
        }
        fout << endl;
    }
    fout << "cost: " << cost << endl;
    Adj.clear();
    edgeSet.clear();
    return true;
}

int Choose(int d[], vector<bool>& s, int n) {    //visited, num of vertex
    int min = MAX;                              //The function returns the value with the lowest distance among the candidates.
    int idx = -1;
    for(int i=0; i<n; i++) {                    //s[i] != visited
        if((d[i] < min) && !s[i]) {
            min = d[i];
            idx = i;
        }
    }
    return idx;
}

bool Dijkstra(Graph* graph, int vertex, ofstream& fout) {
    int m_Size = graph->getSize();

    int dist[m_Size];
    int prev[m_Size];
    vector<bool> set(m_Size);

    //initialize
    for(int i=0; i<m_Size; i++) {
        dist[i] = MAX;
        prev[i] = -1;
        set[i] = false;
    }

    map<int, int> Adj;      //Map that stores adjacent nodes and weights for each Vertex.
    for(int i=0; i<m_Size; i++) {
        Adj.clear();
        graph->getAdjacentEdges(i, &Adj);
        for(auto iter = Adj.begin(); iter != Adj.end(); iter++) {
            if(iter->second < 0) return false;      //Print error if negative weight exists.
        }
    }

    Adj.clear();        //Clear Adj
    graph->getDirAdjacentEdges(vertex, &Adj);
    //Update prev and dist for the first vertex.
    for(auto iter = Adj.begin(); iter != Adj.end(); iter++) {
        dist[iter->first] = iter->second;
        prev[iter->first] = vertex;
    }
    //visited processed the start vertex.
    set[vertex] = true;
    for(int i=0; i<m_Size-1; i++) {
        int u = Choose(dist, set, m_Size);
        //If there's nothing adjacent, break
        if(u == -1) break;

        //visited processed u vertex.
        set[u] = true;
        Adj.clear();
        graph->getDirAdjacentEdges(u, &Adj);
        for(auto iter = Adj.begin(); iter != Adj.end(); iter++) {
            int newdist = dist[u] + iter->second;
            //dist[w] = min(dist[u] + length[u][w], dist[w])
            if(newdist < dist[iter->first]) {
                dist[iter->first] = newdist;
                prev[iter->first] = u;
            }
        }
    }

    //Print the path using dist and prev. using Stack.
    stack<int> tmp;
    fout << "startvertex: " << vertex << endl;
    for(int i=0; i<m_Size; i++) {
        if(i == vertex) continue;
        fout << "[" << i << "]";
        if(dist[i] == MAX) {
            fout <<  " x" << endl;
            continue;
        }
        tmp.push(i);
        for(int j=prev[i]; j != vertex; ) {
            tmp.push(j);
            j = prev[j];
        }
        fout << " " << vertex;
        while(!tmp.empty()) {
            fout << "->" << tmp.top();
            tmp.pop();
        }
        fout << " (" << dist[i] << ")" << endl;
    }
    return true;
}

bool Bellmanford(Graph* graph, int s_vertex, int e_vertex, ofstream& fout) {
    int m_Size = graph->getSize();
    int dist[m_Size];
    int prev[m_Size];

    map<int, int> Adj;      //Map that stores adjacent nodes and weights for each Vertex.
    //Initialization
    for(int i=0; i<m_Size; i++) {
        dist[i] = MAX;
        prev[i] = -1;
    }

    dist[s_vertex] = 0;
    graph->getDirAdjacentEdges(s_vertex, &Adj);
    //Caculation when k=1
    for(auto iter = Adj.begin(); iter != Adj.end(); iter++) {
        dist[iter->first] = iter->second;                       //Treat non-directly connected paths as MAX
        prev[iter->first] = s_vertex;                           //Update Prev array
    }

    //Update dist and prev for route past edge 2 to v-1 times
    //d(v,k) = min{d(v,k-1), min{d(w,k-1) + length of edge (w,v)}}

    map<int, int> income;   //Obtain the incoming edge for the entered vertex and save it on the map as {from, weight}.
    for(int k=2; k <= m_Size-1; k++) {
        for(int j=0; j<m_Size; j++) {
            income.clear();
            graph->getIncomingEdges(j, &income);
            //v has at least one incoming edge && v != s
            if(!income.empty()) {
                for(auto iter = income.begin(); iter != income.end(); iter++) {
                    if(dist[iter->first] == MAX) continue;
                    if(dist[j] > dist[iter->first] + iter->second) {
                        dist[j] = dist[iter->first] + iter->second;
                        prev[j] = iter->first;
                    }
                }
            }
        }
    }

    for(int i=0; i<m_Size; i++) {
        income.clear();
        graph->getIncomingEdges(i, &income);
        if(!income.empty()) {
            for(auto iter = income.begin(); iter != income.end(); iter++) {
                //iter->first : From, iter->second : Weight
                if(dist[iter->first] == MAX) continue;
                if(dist[i] > dist[iter->first] + iter->second) {
                    //A negative cycle exists, return false
                    return false;
                }
            }
        }
    }

    stack<int> tmp;
    int p = e_vertex, j = 0;
    tmp.push(e_vertex);
    p = prev[p];
    if(p == -1) {
        fout << "x" << endl;    //print 'x' if path is unreachable
        return true;            //return.
    }
    while(p != -1) {            //print all paths along the prev.
        tmp.push(p);            //Using Stack.
        p = prev[p];
        if(p == s_vertex) break;
    }
    fout << s_vertex;
    while(!tmp.empty()) {
        fout << " -> " << tmp.top();
        tmp.pop();
    }
    fout << endl << "cost: " << dist[e_vertex] << endl;
    income.clear();
    Adj.clear();
    return true;
}

bool FLOYD(Graph* graph, ofstream& fout) {
    int m_Size = graph->getSize();
    int Mat[m_Size][m_Size];    //Construct Adjacency matrix 
    //Initialization
    for(int i=0; i<m_Size; i++) {
        for(int j=0; j<m_Size; j++) {
            if(i==j) {
                Mat[i][j] = 0;
                continue;
            }
            Mat[i][j] = MAX;        //Treat non-directly connected paths as MAX
        }
    }
    map<int, int> Adj;      ////Map that stores adjacent nodes and weights for each Vertex.
    //construct m_Size*m_Size Matrix
    for(int i=0; i<m_Size; i++) {
        Adj.clear();
        graph->getDirAdjacentEdges(i, &Adj);
        for(auto iter = Adj.begin(); iter != Adj.end(); iter++) {
            Mat[i][iter->first] = iter->second;
        }
    }

    for(int k=0; k<m_Size; k++) {
        for(int i=0; i<m_Size; i++) {
            for(int j=0; j<m_Size; j++) {
                if(Mat[i][k] == MAX || Mat[k][j] == MAX) continue; 
                Mat[i][j] = min(Mat[i][j], Mat[i][k] + Mat[k][j]);
            }
        }
    }
    //If Graph contains negative weight, Print Errorcode
    for(int i=0; i<m_Size; i++) {
        if(Mat[i][i] < 0) {
            return false;
        }
    }

    //Print final adjacency matrix
    fout << '\t'; 
	for(int i=0; i<m_Size; i++) {
		fout << "[" << i << "]" << '\t';
	}
	fout << endl;

	for(int i=0; i<m_Size; i++) {
		fout << "[" << i << "]";
		for(int j=0; j<m_Size && fout<<'\t'; j++) {
            if(Mat[i][j] == MAX) fout << "x";
			else {
                fout << Mat[i][j];
            }
		}
		fout << endl;
	}
    Adj.clear();
    return true;
}