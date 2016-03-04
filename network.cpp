
#include "network.h"

int network::addNode(Graph_Node node)
{
  Pres_Graph.push_back(node);
  return 0;
}


int network::netSize()
{
  return Pres_Graph.size();
}

// get the index of node with a given name
int network::get_index(vector<Graph_Node>::iterator listIt)
{
  listIt->index;
}
// get the node at nth index
vector<Graph_Node>::iterator network::get_nth_node(int n)
{
  &(Pres_Graph[n]);
}
//get the iterator of a node with a given name
vector<Graph_Node>::iterator network::search_node(string val_name)
{
  vector<Graph_Node>::iterator listIt;
  for(listIt=Pres_Graph.begin();listIt!=Pres_Graph.end();listIt++)
  {
    if(listIt->get_name().compare(val_name)==0)
    return listIt;
  }
  return listIt;
}
