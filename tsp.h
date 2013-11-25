//==================================================================
// File			: tsp.h
// Author		: rsagalyn
// Date			: Aug 18, 2013
// Description	:
//==================================================================
#ifndef MWM_H_
#define MWM_H_

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <pthread.h>
#include <queue>
#include <stack>
#include <string>
#include <stdio.h>
#include <vector>

#include "twoOpt.h"

using namespace std;

// Toggle printing debugging info to console
#define DEBUG 0

// Number of threads to use to fill N x N cost matrix
#define THREADS 1

// Calcualte lowest index controlled by thread id
#define START_AT(id,p,n) ((id)*(n)/(p))

// Calculate highest index controlled by thread id
#define END_AT(id,p,n) (START_AT((id)+1,p,n)-1)



class TSP
{
private:

	// x and y coords of a node
	struct City
	{
		int x;
		int y;
	};

	// Filename supplied on command line to read from
	string inFname;

	// Program-generated filename to output to
	string outFname;

	// List of odd nodes
	vector<int>odds;

	// Smaller cost matrix used to store distances between odd nodes
	// Used to find minimum matching on odd nodes
	int **cost;


	// Initialization function
	void getNodeCount();

	// Find odd vertices in graph
	void findOdds();

	// Prim helper function
	int minKey(int key[], bool mstSet[]);


protected:


public:
	// Number of nodes
	int n;

	// euler circuit
	vector<int>circuit;

	// Store cities and coords read in from file
	vector<City>cities;

	// Full n x n cost matrix of distances between each city
	int **graph;

	// Current shortest path length
	int pathLength;


	// Adjacency list
	// Array of n dynamic arrays, each holding a list of nodes it's index is attached to
	vector<int> *adjlist;


	int start_idx[THREADS];

	int end_idx[THREADS];

	// n x 2 array to store length of TSP path starting at each node
	// col 0 => starting index   col 1 => path length from that node
	int **path_vals;


	// Constructor
	TSP(string in, string out);

	// Destructor
	~TSP();

	// Calculate distance
	int get_distance(struct City c1, struct City c2);


	// Initialization functions
	void readCities();
	void fillMatrix_threads();

	// Find MST using Prim's algorithm
	void findMST_old();


	// Find perfect matching
	void perfect_matching();

	// Find best node to start euler at
	// Doesn't create tour, just checks
	int find_best_path(int);

	// Create tour starting at specified node
	void create_tour(int);

	// Private functions implemented by create_tour() and find_best_path()
	void euler (int pos, vector<int> &);
	//void euler(int);
	void make_hamilton(vector<int> &, int&);

	// Calls twoOpt function
	void make_shorter();


	// Debugging functions
	void printCities();
	void printAdjList();
	void printResult();
	void printEuler();
	void printPath();

	// Get node count
	int get_size() {return n;};
};

#endif /* MWM_H_ */
