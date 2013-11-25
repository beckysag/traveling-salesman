#include "tsp.h"

struct thread_data {
	int tid;
	TSP *tsp;
};
struct thread_data *data;

TSP::TSP(string in, string out){
	/////////////////////////////////////////////////////
	// Constructor
	/////////////////////////////////////////////////////
	inFname = in; outFname = out;

	// set n to number of lines read from input file
	getNodeCount();

	// Allocate memory
	graph = new int*[n];
	for (int i = 0; i < n; i++) {
		graph[i] = new int[n];
		for (int j = 0; j < n; j++) graph[i][j] = 0;
	}

	cost = new int*[n];
	for (int i = 0; i < n; i++) {
		cost[i] = new int[n];
	}

	path_vals = new int*[n];
	for (int i = 0; i < n; i++) {
		path_vals[i] = new int[n];
	}

	// Adjacency lsit
	adjlist = new vector<int> [n];
};

TSP::~TSP(){
	/////////////////////////////////////////////////////
	// Destructor
	/////////////////////////////////////////////////////

	for (int i = 0; i < n; i++) {
		delete [] graph[i];
		delete [] cost[i];
		delete [] path_vals[i];
	}
	delete [] path_vals;
	delete [] graph;
	delete [] cost;
	delete [] adjlist;
}

void TSP::getNodeCount(){
	int count = 0;
	ifstream inStream;
	inStream.open(inFname.c_str(), ios::in);

	if (!inStream) {
	  cerr << "Can't open input file " << inFname << endl;
	  exit(1);
	}
	std::string unused;
	while ( std::getline(inStream, unused) )
	   ++count;
	n = count;
	inStream.close();
};

void TSP::readCities(){
	/////////////////////////////////////////////////////
	ifstream inStream;
	inStream.open(inFname.c_str(), ios::in);
	if (!inStream) {
	  cerr << "Can't open input file " << inFname << endl;
	  exit(1);
	}
	int c, x, y;
	int i = 0;
	while (!inStream.eof() ) {
		inStream >> c >> x >> y;
		// Push back new city to vector
		struct City c = {x, y};
		cities.push_back(c);
		i++;
	}
	inStream.close();
};

int TSP::get_distance(struct TSP::City c1, struct TSP::City c2) {
	/////////////////////////////////////////////////////
	// Calculate distance between c1 and c2
	/////////////////////////////////////////////////////
	int dx = pow((float)(c1.x - c2.x), 2);
	int dy = pow((float)(c1.y - c2.y), 2);
	return (floor((float) (sqrt(dx + dy)) + 0.5));
};

void *F(void* args){
	struct thread_data *my_data = (struct thread_data *) args;
	int tid = my_data->tid;
	TSP *tsp = my_data->tsp;
	int **graph = tsp->graph;
	int start, end;
	//start = START_AT(tid, THREADS, tsp->n);
	//end = END_AT(tid, THREADS, tsp->n);

	start = tsp->start_idx[tid];
	end = tsp->end_idx[tid];
	//cout << "thread " << setw(4) << left << tid << setw(8) << left << " start: " << setw(5) << left << start;
	//cout << setw(6) << left << " end: " << setw(5) << left << end << " load: " << end- start + 1 << endl;

	//clock_t t = clock();
	// fill matrix with distances from every city to every other city
	for (int i = start; i <= end; i++) {
		for (int j = i; j < tsp->n; j++) {
			// Don't delete this line  it's supposed to be there.
			graph[i][j] = graph[j][i] =  tsp->get_distance(tsp->cities[i], tsp->cities[j]);
		}
	}
	//t = clock() - t;
	//t = clock();
	//cout << "thread " << tid << " time: " << 1000*(((float)clock())/CLOCKS_PER_SEC) << " s"<< endl;
	pthread_exit(NULL);
}

void TSP::fillMatrix_threads(){
	/////////////////////////////////////////////////////
	/////////////////////////////////////////////////////
	int amount = (n / THREADS) * 0.2;
	int x = (n / THREADS) - amount;		// min amount given to threads
	int rem = n - (x * THREADS);
	int half = THREADS/2 + 1;

	int pos = 0;
	for (int i = 0; i < half; i++) {
		start_idx[i] = pos;
		pos += (x - 1);
		end_idx[i] = pos;
		pos++;
	}
	int remainingThreads = THREADS - half + 1;
	int extraToEach = rem / remainingThreads;
	// Divide remainer among second half of threads
	for (int i = half; i < THREADS; i++) {
		start_idx[i] = pos;
		pos += (x + extraToEach - 1);
		end_idx[i] = pos;
		pos++;
	}
	end_idx[THREADS-1] = n - 1;

	int rc; void *status;
	data = new struct thread_data[n];

	// allocate space for n thread ids
	pthread_t *thread = new pthread_t[n];
	pthread_attr_t attr;

	// Initialize and set thread detached attribute
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	for (long t = 0; t < THREADS; t++) {
		//printf("Creating thread %ld\n", t);
		data[t].tid = t;
		data[t].tsp = this;
		rc = pthread_create(&thread[t], &attr, F, (void*)&data[t]);
		if (rc) {
			printf("ERROR; return code from pthread_create() is %d\n", rc);
			exit(-1);
		}
	}

	// Free attribute and wait for the other threads
	pthread_attr_destroy(&attr);
	for (long t = 0; t < THREADS; t++) {
		rc = pthread_join(thread[t], &status);
		if (rc) {
			printf("ERROR; return code from pthread_join() is %d\n", rc);
			exit(-1);
		}
		 //printf("Completed join with thread %ld having a status of %ld\n",t,(long)status);
	}
	delete [] data;
}

void TSP::findMST_old() {
	/////////////////////////////////////////////////////
	// In each iteration, we choose a minimum-weight
	// edge (u, v), connecting a vertex v in the set A to
	// the vertex u outside of set A
	/////////////////////////////////////////////////////
	int key[n];   // Key values used to pick minimum weight edge in cut
	bool in_mst[n];  // To represent set of vertices not yet included in MST
	int parent[n];

	// For each vertex v in V
	for (int v = 0; v < n; v++) {
		// Initialize all keys to infinity
		key[v] = std::numeric_limits<int>::max();

		// Mark as not being in mst yet
		in_mst[v] = false;
	}

	// Node 0 is the root node so give it the lowest distance (key)
	key[0] = 0;
	parent[0] = -1; // First node is always root of MST

	for (int i = 0; i < n - 1; i++) {
		// Find closest remaining (not in tree) vertex
		// TO DO : This would be better represented by heap/pqueue
		int v = minKey(key, in_mst);

		// Add vertex v to the MST
		in_mst[v] = true;

		// Look at each vertex u adjacent to v that's not yet in mst
		for (int u = 0; u < n; u++) {
			if (graph[v][u] && in_mst[u] == false && graph[v][u] < key[u]) {
				// Update parent index of u
				parent[u] = v;

				// Update the key only if dist is smaller than key[u]
				key[u] = graph[v][u];
			}
		}
	}

	// map relations from parent array onto matrix
	for (int v1 = 0; v1 < n; v1++) {
		// there is an edge between v1 and parent[v1]
		int v2 = parent[v1];
		if (v2 != -1) {
			adjlist[v1].push_back(v2);
			adjlist[v2].push_back(v1);
		}
	}
};

// findMST helper function
int TSP::minKey(int key[], bool mstSet[]) {
	// Initialize min value
	int min = std::numeric_limits<int>::max();
	int min_index;
	for (int v = 0; v < n; v++)
		if (mstSet[v] == false && key[v] < min) {
			min = key[v];
			min_index = v;
		}
	return min_index;
};

void TSP::findOdds() {
	/////////////////////////////////////////////////////
	// Find nodes with odd degrees in T to get subgraph O
	/////////////////////////////////////////////////////

	// store odds in new vector for now
	for (int r = 0; r < n; r++) {
		//cities[r].isOdd = ((adjlist[r].size() % 2) == 0) ? 0 : 1;
		if ((adjlist[r].size() % 2) != 0 ) {
			odds.push_back(r);
		}
	}
}

void TSP::perfect_matching() {
	/////////////////////////////////////////////////////
	// find a perfect matching M in the subgraph O using greedy algorithm
	// but not minimum
	/////////////////////////////////////////////////////
	int closest, length; //int d;
	std::vector<int>::iterator tmp, first;

	// Find nodes with odd degrees in T to get subgraph O
	findOdds();

	// for each odd node
	while (!odds.empty()) {
		first = odds.begin();
		vector<int>::iterator it = odds.begin() + 1;
		vector<int>::iterator end = odds.end();
		length = std::numeric_limits<int>::max();
		for (; it != end; ++it) {
			// if this node is closer than the current closest, update closest and length
			if (graph[*first][*it] < length) {
				length = graph[*first][*it];
				closest = *it;
				tmp = it;
			}
		}	// two nodes are matched, end of list reached
		adjlist[*first].push_back(closest);
		adjlist[closest].push_back(*first);
		odds.erase(tmp);
		odds.erase(first);
	}
}


// Take reference to a path vector
// so can either modify actual euler path or a copy of it
void TSP::euler (int pos, vector<int> &path) {
	/////////////////////////////////////////////////////////
	// Based on this algorithm:
	//	http://www.graph-magics.com/articles/euler.php
	// we know graph has 0 odd vertices, so start at any vertex
	// O(V+E) complexity
	/////////////////////////////////////////////////////////

	// make copy of original adjlist to use/modify
	vector<int> *temp = new vector<int> [n];
	for (int i = 0; i < n; i++) {
		temp[i].resize(adjlist[i].size());
		temp[i] = adjlist[i];
	}

	path.clear();

	// Repeat until the current vertex has no more neighbors and the stack is empty.
	stack<int> stk;
	while (!stk.empty() || temp[pos].size() > 0 ) {
		// If current vertex has no neighbors -
		if (temp[pos].size() == 0) {
			// add it to circuit,
			path.push_back(pos);
			// remove the last vertex from the stack and set it as the current one.
			int last = stk.top();
			stk.pop();
			pos = last;
		}
		// Otherwise (in case it has neighbors)
		else {
			// add the vertex to the stack,
			stk.push(pos);
			// take any of its neighbors,
			int neighbor = temp[pos].back();
			// remove the edge between selected neighbor and that vertex,
			temp[pos].pop_back();
	        for (unsigned int i = 0; i < temp[neighbor].size(); i++)
	            if (temp[neighbor][i] == pos) { // find position of neighbor in list
	        	    temp[neighbor].erase (temp[neighbor].begin() + i); // remove it
	                break;
	            }
			// and set that neighbor as the current vertex.
	        pos = neighbor;
		}
	}
	path.push_back(pos);
}


void TSP::make_hamilton(vector<int> &path, int &path_dist) {
	// remove visited nodes from Euler tour
	bool visited[n]; // boolean value for each node if it has been visited yet
	memset(visited, 0, n * sizeof(bool));

	path_dist = 0;

	int root = path.front();
	vector<int>::iterator curr = path.begin();
	vector<int>::iterator next = path.begin()+1;
	visited[root] = true;

	// loop until the end of the circuit list is reached
	while ( next != path.end() ) {
		// if we haven't been to the next city yet, go there
		if (!visited[*next]) {
			path_dist += graph[*curr][*next];
			curr = next;
			visited[*curr] = true;
			next = curr + 1;
		}else {
			next = path.erase(next); // remove it
		}
	}

	// add the distance back to the root
	path_dist += graph[*curr][*next];
}





void TSP::create_tour(int pos){
	// call euler with actual circuit vector
	euler(pos, circuit);

	// make it hamiltonian
	// pass actual vars
	make_hamilton(circuit, pathLength);
}


// Does euler and hamilton but doesn't modify any variables
// Just finds path length from the node specified and returns it
int TSP::find_best_path (int pos) {

	// create new vector to pass to euler function
	vector<int>path;
	euler(pos, path);

	// make it hamiltonian, pass copy of vars
	int length;
	make_hamilton(path, length);

	// Optimize
	twoOpt(graph, path, length, n);
	twoOpt(graph, path, length, n);
	twoOpt(graph, path, length, n);
	twoOpt(graph, path, length, n);
	twoOpt(graph, path, length, n);

	return length;
}


void TSP::make_shorter(){
	// Modify circuit & pathLength
	twoOpt(graph, circuit, pathLength, n);
}




//================================ PRINT FUNCTIONS ================================//

void TSP::printResult(){
	ofstream outputStream;
	outputStream.open(outFname.c_str(), ios::out);
	outputStream << pathLength << endl;
	for (vector<int>::iterator it = circuit.begin(); it != circuit.end(); ++it) {
	//for (vector<int>::iterator it = circuit.begin(); it != circuit.end()-1; ++it) {
		outputStream << *it << endl;
	}
	//outputStream << *(circuit.end()-1);
	outputStream.close();
};

void TSP::printPath(){
	cout << endl;
	for (vector<int>::iterator it = circuit.begin(); it != circuit.end()-1; ++it) {
		cout << *it << " to " << *(it+1) << " ";
		cout << graph[*it][*(it+1)] << endl;
	}
	cout << *(circuit.end()-1) << " to " << circuit.front();

	cout << "\nLength: " << pathLength << endl << endl;
};

void TSP::printEuler() {
	for (vector<int>::iterator it = circuit.begin(); it != circuit.end(); ++it)
		cout << *it << endl;
}

void TSP::printAdjList() {
	for (int i = 0; i < n; i++) {
		cout << i << ": "; //print which vertex's edge list follows
		for (int j = 0; j < (int)adjlist[i].size(); j++) {
			cout << adjlist[i][j] << " "; //print each item in edge list
		}
		cout << endl;
	}
};

void TSP::printCities(){
	cout << endl;
	int i = 0;
	for (vector<City>::iterator it = cities.begin(); it != cities.end(); ++it)
		cout << i++ << ":  " << it->x << " " << it->y << endl;
}


