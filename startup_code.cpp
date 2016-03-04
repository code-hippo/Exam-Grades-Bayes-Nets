#include "startup_code.h"

using namespace std;



network Alarm;
char* alarmFileName;
char* recordsFileName;

// Format checker just assumes you have Alarm.bif and Solved_Alarm.bif (your file) in current directory
// Can be changed in main()
network read_network()
{
  string line;
  ifstream myfile(alarmFileName);
  string temp;


  string node_name;
  vector<string> node_values;
  vector<int> parentNodes;

  int node_index=0;

  if (myfile.is_open())
  {
    while (! myfile.eof() )
    {
      stringstream ss;
      getline (myfile,line);

      ss.str(line);
      ss>>temp;

      if(temp.compare("variable")==0)
      {

        ss>>node_name;
        getline (myfile,line);

        stringstream ss2;
        ss2.str(line);
        for(int i=0;i<4;i++)
        {
          ss2>>temp;
        }
        node_values.clear();
        while(temp.compare("};")!=0)
        {
          node_values.push_back(temp);
          ss2>>temp;
        }
        Graph_Node new_node(node_name,node_values.size(),node_values);
        new_node.index= node_index;node_index++;
        int pos=Alarm.addNode(new_node);

      }
      else if(temp.compare("probability")==0)
      {
        ss>>temp;
        ss>>temp;

        vector<Graph_Node>::iterator node;
        vector<Graph_Node>::iterator tempNode;

        node=Alarm.search_node(temp);
        int index=node->index;
        ss>>temp;
        parentNodes.clear();
        while(temp.compare(")")!=0)
        {
          tempNode=Alarm.search_node(temp);
          tempNode->add_child(index);
          parentNodes.push_back(tempNode->index);
          ss>>temp;

        }
        node->set_Parents(parentNodes);
        getline (myfile,line);
        stringstream ss2;

        ss2.str(line);
        ss2>> temp;
        ss2>> temp;

        vector<float> curr_CPT;
        string::size_type sz;
        while(temp.compare(";")!=0)
        {
          curr_CPT.push_back(atof(temp.c_str()));
          ss2>>temp;
        }
        node->set_CPT(curr_CPT);
        }
     }
     myfile.close();
   }

   return Alarm;
}

void write_network()
{
  ifstream myfile(alarmFileName);
  ofstream mywritefile("solved_alarm.bif");
  string line;
  string temp;
  int index=0;
  if (myfile.is_open())
  {
    while (! myfile.eof() )
    {
      stringstream ss;
      getline (myfile,line);
      ss.str(line);
      ss>>temp;
      if(temp.compare("variable")==0)
      {
        mywritefile << line<<"\n";
        getline (myfile,line);
        mywritefile << line<<"\n";
        getline (myfile,line);
        mywritefile <<line<<"\n";
        getline (myfile,line);
        mywritefile <<line<<"\n";
        temp="";
      }
      else if(temp.compare("probability")==0)
      {
        mywritefile << line<<"\n";

        getline (myfile,line);
        mywritefile << line.substr(0,7); // " table " = line.substr(0,7)

        for(int i=0;i<Alarm.Pres_Graph[index].CPT.size();i++)
        {
          float probability = (Alarm.Pres_Graph[index].CPT[i]);

          probability=((roundf(probability*10000))/10000);
          if(probability<0.0001)
          {
           probability=0.0001;            // 0.0001 given to account for the bias in the information
          }
          else if(probability>.9999){
             probability=0.9999;           // 0.0001 given to account for the bias in the information
          }
          mywritefile <<probability<<" ";
        }
        index++;
        mywritefile<<";\n";
        getline (myfile,line);
        mywritefile<< line<<"\n";
        temp="";
      }
      else
      {
        temp="";
        if(!myfile.eof()){
        mywritefile<< line<<"\n";
        }
      }
    }
    mywritefile.close();
    myfile.close();
  }
}

// Returns the corresponding index of the symbol "s" of a node.
// "Intelligence" takes two value "False","True" then index of False ->0 and index of True ->1

int symbol_num(int index, string node_value){
  if(node_value=="\"?\"")
  {
    return -1;
  }
  else
  {
    string node_name=(Alarm.Pres_Graph[index]).Node_Name;
    vector<string> node_values=((Alarm.Pres_Graph[index]).values);
    for(int i=0; i<node_values.size();i++)
    {
      if(node_value==node_values[i])
      {
        return i;
      }
    }
    return -2;
  }
}


// Randomly initialises the data.
//If  "Intelligence" is the "?" (or) unknown field in the observation
//And intelligence can take two types of values.
//Therefore randomly (0 or 1) is chosen to initialise instead of "?".
void initialiseData(records &r){
  srand(time(NULL));
  for(int i=0; i<r.observations_array.size();i++){
    vector<int> toInit=r.observations_array[i];
    int index=r.unknown_Index[i];
    if(index!=-1){
      int nvalues=(Alarm.Pres_Graph[index]).get_nvalues();
      int random=rand()%nvalues;
      r.observations_array[i][index]=random;
    }
  }
  return ;
}

// Given the parentids and corresponding values, returns the index in the cpt array.
int getIndex(vector<int> parent_ids, vector<int> values){

    int index=0;
    for(int i=0;i<parent_ids.size()-1;i++){
        index=(index+values[i])*((Alarm.Pres_Graph[parent_ids[i+1]]).nvalues);
    }

    return index+values[parent_ids.size()-1];
}

vvi markovB;
vi CPTmSize;
// array of (id of node followed by its parentids)
vvi Parent_ids;
// array to count the spt table size for each node.
vi CPT_tableSize;

//prepares the help arrays with reduces the code redundancy.
void make_help_arrays(){
  for(int i=0;i<Alarm.Pres_Graph.size();i++){
    vi id;
    id.pb(i);
    for(int j=0; j<Alarm.Pres_Graph[i].Parents.size(); j++){
      id.pb((Alarm.Pres_Graph[i].Parents[j]));
    }
    Parent_ids.pb(id);
    int cptSize=1;
    for(int k=0;k<id.size();k++){
      cptSize*=Alarm.Pres_Graph[id[k]].nvalues;
    }
    CPT_tableSize.pb(cptSize);
  }
}

// Updates the network with the changes CPT table values for each node
//input : updated cpt table
void insertCpt(vvf cpt){

    for(int i=0; i<(Alarm.Pres_Graph).size(); i++){
        (Alarm.Pres_Graph[i]).set_CPT(cpt[i]);
    }
    return ;
}

vvf unnormalisedCPT(records record){
  vvf countvvf;
  for(int i=0;i<Alarm.Pres_Graph.size();i++){
    vi id;
    id = Parent_ids[i];
    vf countCpt(CPT_tableSize[i],laplace_smoothing_const);         // for laplacian smoothing

    for(int j=0; j<record.observations_array.size();j++){
      vi values;
      for(int k=0; k<id.size(); k++){
        values.pb((record.observations_array[j])[id[k]]);
      }
      int index=getIndex(id,values);
      countCpt[index]+=1;
    }
    countvvf.pb(countCpt);
  }
  return countvvf;
}

// Calculates normalised CPT values from the given CPT values
vector<vector<float> > normalisedCPT(vvf count_cpt){
    vector<vector<float> > cptvvf;
    for(int i=0;i<count_cpt.size();i++){
      vi id;
      id=Parent_ids[i];
      int cptSize=CPT_tableSize[i];
      int pi=cptSize/Alarm.Pres_Graph[i].nvalues;
      vector<float> cptvf(CPT_tableSize[i]);
      vf piCount(pi,0);

      for(int j=0; j<pi; j++){
        for(int k=0;k<Alarm.Pres_Graph[i].nvalues;k++){
          piCount[j]+=count_cpt[i][k*pi+j];
        }
      }

      for(int j=0;j<count_cpt[i].size();j++){          //also takes into account the laplacian smoothing
        if(piCount[j%pi]==0.001){
          cptvf[j]=0.0001;
          continue;
        }
        else if(count_cpt[i][j]==piCount[j%pi]){
          cptvf[j]=0.9999;
        }
        cptvf[j]=((float)count_cpt[i][j])/((float)piCount[j%pi]);
      }

      cptvvf.pb(cptvf);
    }
    return cptvvf;
}


//applies the markov model of substituting the "?"
//Takes into account
//        the records of the observations.
//        confidence values taken from CPT tables of the graph node (calculated)
//Outputs
//        the updated observations as per the hidden markov model
void markovmodel_update(records &r){

  for(int i=0;i<r.unknown_Index.size();i++){
    //q_index -> index of the question mark (or) unknown field in the observation
    int q_index=r.unknown_Index[i];
    float p1=0;
    if(q_index>-1){
      vf prob;
      int clocp;
      for(int w=0;w<Alarm.Pres_Graph[q_index].nvalues;w++)
      {
        vi values;
        for(int j=0;j<Parent_ids[q_index].size();j++)
        {
          values.push_back(r.observations_array[i][Parent_ids[q_index][j]]);
        }
        values[0]=w;
        int cloc0=getIndex(Parent_ids[q_index],values);
        if(w==r.observations_array[i][Parent_ids[q_index][0]])
        {
          clocp=cloc0;
        }
        float p1=1;

        p1=(Alarm.Pres_Graph[q_index].CPT[cloc0]);

        for(int j=0;j<Alarm.Pres_Graph[q_index].Children.size();j++)
        {
          vi values2;
          int index2=Alarm.Pres_Graph[q_index].Children[j];
          for(int k=0;k<Parent_ids[index2].size();k++)
          {
            values2.push_back(r.observations_array[i][Parent_ids[index2][k]]);
            if(q_index==Parent_ids[index2][k])
            {
              values2[values2.size()-1]=w;
            }
          }
          int cloc2=getIndex(Parent_ids[index2],values2);
          p1*=Alarm.Pres_Graph[index2].CPT[cloc2];
        }
        prob.pb(p1);
      }
      int p=0;
      float pf=0;
      for(int k=0;k<Alarm.Pres_Graph[q_index].nvalues;k++)
      {
        if(prob[k]>pf)
        {
          p=k;
          pf=prob[k];
        }
      }
      r.observations_array[i][q_index]=p;
    }
  }
}

//Stopping criteria with the time as parameter
//Could be modified to use convergence as the stopping criteria
void expectationMax(records &r)
{
  make_help_arrays();
  initialiseData(r);
  insertCpt(normalisedCPT(unnormalisedCPT(r)));

  while(time(NULL)-start<(20))
  {
    markovmodel_update(r);
    if(time(NULL)-start>(20))
    {
      return ;
    }
    insertCpt(normalisedCPT(unnormalisedCPT(r)));
  }
  return ;
}

int main(int argc,char** argv)
{
  alarmFileName=argv[1];
  recordsFileName=argv[2];

  start=time(NULL);
  records record;
  Alarm=read_network();

  int numVars=Alarm.netSize();
  vector<vector<int> > observations_array;
  vector<int> unknown_Index;
  string dataLine="";
  int q_index = 0;
  vector<int> dataEntry;
  bool pushed=false;
  ifstream myfile(recordsFileName);
  // For debigging purposes : print output redirected for crosschecking the working
  ofstream mywritefile("test.txt");

  if (myfile.is_open())
  {
    while (! myfile.eof() )
    {
      getline (myfile,dataLine);
      stringstream ss(dataLine);
      string temp="";
      ss >> temp;
      while(ss)
      {
        if(temp=="?"){
          unknown_Index.push_back(q_index);
          pushed=true;
        }
        q_index++;
        int sym_num=symbol_num(q_index-1,"\""+temp+"\"");
        dataEntry.push_back(sym_num);
        mywritefile << sym_num << "	 ";
        if(q_index == numVars){
          if(!pushed){ unknown_Index.push_back(-1);}
          observations_array.push_back(dataEntry);
          dataEntry.clear();
          q_index = 0;
          pushed=false;
        }
        ss>>temp;
      }
      mywritefile<<"run"<< observations_array.size()<<"\n";
    }
    myfile.close();
    mywritefile.close();
  }

  record.unknown_Index=unknown_Index;
  record.observations_array=observations_array;
  expectationMax(record);

  write_network();
  return 0;
}
