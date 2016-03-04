#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <algorithm>
#include <math.h>

using namespace std;
// Our graph consists of a list of nodes where each node is represented as follows:
class Graph_Node{

public:
 string Node_Name;  // Variable name
 int index;
 vector<int> Children; // Children of a particular node - these are index of nodes in graph.
 vector<int> Parents; // Parents of a particular node- note these are names of parents
 int nvalues;  // Number of categories a variable represented by this node can take
 vector<string> values; // Categories of possible values
 vector<float> CPT; // conditional probability table as a 1-d array . Look for BIF format to understand its meaning

 // Constructor- a node is initialised with its name and its categories
Graph_Node(string name,int n,vector<string> vals);

  public:
    string get_name();
    vector<int> get_children();
    vector<int> get_Parents();
    vector<float> get_CPT();
    int get_nvalues();
    vector<string> get_values();
    void set_CPT(vector<float> new_CPT);

    void set_Parents(vector<int> Parent_Nodes);

   // add another node in a graph as a child of this node
    int add_child(int new_child_index );
};
