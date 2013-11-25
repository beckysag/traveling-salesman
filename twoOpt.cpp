//==================================================================
// File			: twoOpt.cpp
// Author		: rsagalyn
// Date			: Aug 25, 2013
// Description	:
//==================================================================

#include "twoOpt.h"

// Input: edge 1's v, edge 2's u
// Remove edge 1 and edge 2, reconnect using new path
void reverse(vector<int> &path, int start, int end, int n)
{
	while (end - start > 0)
	{
		// Start, end is relative value to the start,
		// the index is start|slut % size
		int temp = path[start % n];
		path[start % n] = path[end % n];
		path[end % n] = temp;
		start++;
		end--;
	}
}


int is_path_shorter(int **graph, int v1, int v2, int v3, int v4, int &total_dist)
{
	if ((graph[v1][v3] + graph[v2][v4]) < (graph[v1][v2] + graph[v3][v4]))
	{
		total_dist -= (graph[v1][v2] + graph[v3][v4] - graph[v1][v3]
				- graph[v2][v4]);
		return 1;
	}
	return 0;
}


// Non-looping version of two-opt heuristic
int twoOpt(int **graph, vector<int> &path, int &pathLength, int n)
{
	int counter = 0;
	int term_cond = 5;
	int old_distance = pathLength;
	//int size = path.size();
	int v1, v2, u1, u2;

	// Iterate over each edge
	for (int i = 0; i < n; i++)
	{
		// first edge
		u1 = i;
		v1 = (i+1) % n; // wrap around to first node if u1 is last node

		// Skip adjacent edges, start with node one past v1
		for (int j = i + 2; (j + 1) % n != i; j++)
		{
			// mod by length to go back to beginning
			u2 = j % n;
			v2 = (j+1) % n;

			// Check if new edges would shorten the path length
			// If so, decreases pathLength
			if (is_path_shorter(graph, path[u1], path[v1], path[u2],
					path[v2], pathLength))
			{

				// Swap u1--v1 and u2--v2
				reverse(path, i + 1, j, n); // v1, u2

				if (pathLength - old_distance < 5 && counter == term_cond)
					break;

				// reset i
				if (pathLength - old_distance > term_cond )
					i = 0;

				old_distance = pathLength;
				counter++;
			}
		}
	}
	return pathLength;
}



int get_path_length(int **graph, vector<int> &path, int size){
	int length = 0;
	for (int i = 0; i < size-1; i++)
	{
		length += graph[path[i]][path[i+1]];
	}
	length += graph[path[size-1]] [path[0]]; // back home
	return length;
}


