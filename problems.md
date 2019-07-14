The plan was to pre-compute whether there is an edge between two vertices
by evaluating the objective function for each pair of vertices

However the objective function runs in f = O(m) for maxcut. Besides that, the number of vertices can get quite large.
We need n^2 * sizeof(double) space for the data, which can come out to 270 GB in some cases - not feasible

Calculating the values on-the-fly is also not reasonable, since we need O(n * f) for _each individual flipped bit_ (since we dont know beforehand whether two bits interact)
We can improve this by omitting all vertices that have no connection, giving us  O( |max N(v)| * f), where N(v) is something small (probably ~50, most of the times much less than that)

Possible improvements:
	Speed up maxcut evaluation
		instead of going through all edges, iterate through bits and their adjacency lists (in AND out)
		-> speed up bc sparse graphs
	Use caching, but use "adj matrix" style instead of matrix. (good point to discuss in paper)


