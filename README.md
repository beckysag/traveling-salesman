# Traveling Salesperson Problem
C++ Implementation of TSP for CS 325 Algorithms at Oregon State University


Overview
-----------

The problem: Given a set of cities and known distances between each pair of cities, find a tour in which each city is visited exactly once and the distance travelled is minimized. 

Implementation: Uses Christofides’ algorithm to construct a tour and the 2-opt algorithm to improve it



Usage
-----------

```
./tsp inputfile
```
where inputfile is a text file in the format:
```   
0    x-coord    y-coord
1    x-coord    y-coord 
.
.
.
n-1  x-coord    y-coord

```
and <i>n</i> is the number of cities

#### Program Output
A new text file with n +1 lines where n is the number of cities. The first line is the length of the tour found. The next n lines contain the city identifiers in the order they are visited by the tour. Each city is listed exactly once in this list. 


Implementation
-----------

### 1. Christofides' Algorithm

Christofides' is a tour construction heuristic that can be applied to graphs with the following property: a complete graph _G_ = ( _V_,_E_,_w_ ) with edge weights that satisfy the triangle inequality _w_(_x,y_) + _w_(_y,z_) ≤ _w_(_x,z_).  

1. Find a minimum spanning tree T of G
2. Let O be the set of vertices with an _odd_ degree in T
3. Find a minimum cost perfect matching M for these vertices
4. Add M to T to obtain multigraph H
5. Find a Eulerian tour of H
6. Convert the Eulerian tour into Hamiltonian path by skipping visited nodes (using shortcuts)

![alt text][fig1]

#### 1.1 Minimum Spanning Tree
I used Prim’s Algorithm to find the minimum spanning tree in G:
```scilab
function MST(G = (V, E)) 
  for v in V do
    key[v] <-- infinity
    parent[v] <-- NULL
    insert v into Q
  end for
  key[0] <-- 0
  while !Q.empty() do
    v <-- Q.removeMin() 
    for u adjacent to v do
      if u ∈ Q and weight(u, v) < key[u] then
        parent[u] <-- v 
      end if
      key[v] <-- weight(u,v)
    end for
  end while
end function
```

The algorithm maintains an array _Q_ with the vertices that are not yet in the tree (initially, _Q_ is empty). It iterates through each vertex _v_ not yet in the tree (using vertex 0 as the intial vertex) and chooses the minimum weight edge (_u_, _v_) where _u_ is already in the tree. Thus, this is the lightest edge crossing the cut (since it connects an edge in the MST with an edge not yet in the MST). Then vertex _v_ is added to the tree. The algorithm continues until _Q_ is empty and thus all vertices have been added to the MST.

![alt text][fig2]

#### 1.2 Vertices With An Odd Degree in MST
The next step is to find vertices with an odd degree in the MST. Since I store the MST in an adjacency list using C++ vectors, this procedure only involves checking the vector size at each index (node) of the list and is O(_n_).


#### 1.3 Weighted Perfect Matching for Odd Vertices

A connected graph has an even number of vertices of an odd degree. We now find a perfect matching among these vertices so that all vertices have an even degree. Ideally, we would find a minimum matching, but instead I used a greedy algorithm to find an approximate minimal matching.
```scilab
function PerfectMatching()
  Input: odds (list of odd vertices), G (adjacency list)
  while !odds.empty do
    v <-- odds.popFront()
    length <-- ∞
    for u ∈ odds do
      if weight(u,v) < length then
        length <-- weight(u,v)
        closest <-- u
      end if
    end for
    G.addEdge(closest,u)
    odds.remove(closest)
  end while
end function
```

![alt text][fig3]

The set of matched vertices is now added to the MST, forming a new multigraph.


#### 1.4 Eulerian Tour
Next we find a euler circuit starting at any arbitrary node in our multigraph. If our node has neighbors, we push our node on a stack, choose a neighbor, remove the edge between them from the graph, and make that neighbor the current vertex. If our vertex has no neighbors left, we add it to our circuit and pop the top vertex from the stack to use as our current vertex. We continue tracing a tour in this manner until the stack is empty and the last vertex has no more neighbors left.

![alt text][fig4]


#### 1.5 Hamiltonian Path

Finally, we turn our Euler circuit into a Hamiltonian path by walking along the Euler tour, checking at every stop whether that node has already been visited. If it has, we skip that node and move on to the next one. Since our graph satisfies the triangle inequality, shortcutting vertices in this manner will not increase the length of our path.

![alt text][fig5]



### 2. Two-Opt

After a tour was constructed using the Christofides heuristic, I applied the 2-opt improvement algorithm to optimize the path. The 2-opt algorithm examines each edge in the tour. For each edge, it looks all non-adjacent edges, and determines whether removing the two edges and reconnecting them would shorten the tour. If it does, the edges are swapped. The search continues until it no longer improves the path.




[fig1]: https://github.com/beckysag/traveling-salesman/raw/master/images/figure01.png "Figure 1"
[fig2]: https://github.com/beckysag/traveling-salesman/raw/master/images/figure02.png "Figure 2"
[fig3]: https://github.com/beckysag/traveling-salesman/raw/master/images/figure03.png "Figure 3"
[fig4]: https://github.com/beckysag/traveling-salesman/raw/master/images/figure04.png "Figure 4"
[fig5]: https://github.com/beckysag/traveling-salesman/raw/master/images/figure05.png "Figure 5"
