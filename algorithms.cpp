#include "algorithms.h"

#include <algorithm>

using namespace std;

// A minimal implementation of the Floyd-Warshall algorithm.
// O(V^3).
// Takes an adjacency matrix as input and uses it 
// as the dynamic programming table directly.
void floyd_warshall(vector<vector<int>> &adj_mat) {
	int V = adj_mat.size();
	// All pairs shortest path.
	for (int k = 0; k < V; ++k)
		for (int i = 0; i < V; ++i)
			for (int j = 0; j < V; ++j)
				adj_mat[i][j] = min(adj_mat[i][j], adj_mat[i][k] + adj_mat[k][j]);
}

// Computes the APSP table and returns the sum of all shortest paths.
int sum_shortest(vector<vector<int>> adj_mat) {
	int V = adj_mat.size();
	// APSP.
	floyd_warshall(adj_mat);
	// Sum up all shortest paths.
	int sum = 0;
	for (int i = 0; i < V; ++i)
		for (int j = 0; j < V; ++j) {
			sum += adj_mat[i][j];
			sum = min(sum, INF);
		}
	return sum;
}

#include <iostream>

// Implements the naive algorithm for finding a single edge
// from a set of edges "el" that, when added to a graph
// defined by "adj_mat", minimizes the average shortest path.
int brute_force(vector<vector<int>> adj_mat, const vector<Edge>& el) {
	int best = INF;
	// Try taking every edge.
	for (const Edge& e : el) {
		// Update graph and recompute and re-sum shortest paths.
		int old_c = adj_mat[e.i][e.j];
		adj_mat[e.i][e.j] = e.cost;
		best = min(best, sum_shortest(adj_mat));
		cout << e.i << " " << e.j << " " << e.cost << " " << sum_shortest(adj_mat) << endl;
		adj_mat[e.i][e.j] = old_c;

	}
	cout << best << endl;
	return best;
}

// The less naive algorithm for finding a single edge
// from a set of edges "el" that, when added to a graph
// defined by "adj_mat", minimizes the average shortest path
int ward_datta(vector<vector<int>> adj_mat, const vector<Edge>& el) {
	int V = adj_mat.size();
	// Compute all shortest paths.
	floyd_warshall(adj_mat);
 	int best = INF;
	// Try every new edge.
	for (const Edge& e : el) {
		int sum = 0;
		// Sum up all shortest paths.
		// Does a single edge relaxation to see if the shortest path changed.
		for (int i = 0; i < V; ++i)
			for (int j = 0; j < V; ++j) {
				sum += min(adj_mat[i][j], adj_mat[i][e.i] + e.cost + adj_mat[e.j][j]);
				sum = min(sum, INF);
			}
		best = min(best, sum);
	}
	return best;
}