#include "Graph_Node.h"
//Defining the Graph_Node functions
Graph_Node::Graph_Node(string name,int n,vector<string> vals)
{
  Node_Name=name;
  nvalues=n;
  values=vals;
}

string Graph_Node::get_name()
{
 return Node_Name;
}
vector<int> Graph_Node::get_children()
{
 return Children;
}
vector<int> Graph_Node::get_Parents()
{
 return Parents;
}
vector<float> Graph_Node::get_CPT()
{
 return CPT;
}
int Graph_Node::get_nvalues()
{
 return nvalues;
}
vector<string> Graph_Node::get_values()
{
 return values;
}
void Graph_Node::set_CPT(vector<float> new_CPT)
{
 CPT.clear();
 CPT=new_CPT;
}
void Graph_Node::set_Parents(vector<int> Parent_Nodes)
{
 Parents.clear();
 Parents=Parent_Nodes;
}
// add another node in a graph as a child of this node
int Graph_Node::add_child(int new_child_index )
{
 for(int i=0;i<Children.size();i++)
 {
   if(Children[i]==new_child_index)
   return 0;
 }
 Children.push_back(new_child_index);
 return 1;
}
