#include "Graph_Node.h"
// The whole network represted as a list of nodes
class network{

public:
 vector <Graph_Node> Pres_Graph;
 int addNode(Graph_Node node);

//number of nodes
 int netSize();

// get the index of node with a given name
 int get_index(vector<Graph_Node>::iterator listIt);

// get the node at nth index
 vector<Graph_Node>::iterator get_nth_node(int n);

//get the iterator of a node with a given name
 vector<Graph_Node>::iterator search_node(string val_name);

};


class records{
public:
    vector<int> unknown_Index;
    vector<vector<int> > observations_array;
};
