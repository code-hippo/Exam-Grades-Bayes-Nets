
#include <random>
#include <time.h>
#include "network.h"
#define vi vector<int>
#define vf vector<float>
#define vs vector<string>
#define vvi vector<vector<int> >
#define vvs vector<vector<string> >
#define pb push_back
#define mp make_pair
#define vvf vector<vector<float> >
// Format checker just assumes you have Alarm.bif and Solved_Alarm.bif (your file) in current directory


using namespace std;

time_t start;
//constant used for laplacian smoothing
float laplace_smoothing_const = 0.001;



// To read the network nodes from the file.
// Used format alarm.bif
network read_network();

//To write the nodes and the predicted CPT tables (computed from Hidden Markov model) to the file.
void write_network();

// Returns the corresponding index of the symbol "s" of a node.
// "Intelligence" takes two value "False","True" then index of False ->0 and index of True ->1
int symbol_num(int index, string node_value);


// Randomly initialises the data.
//If  "Intelligence" is the "?" (or) unknown field in the observation
//And intelligence can take two types of values.
//Therefore randomly (0 or 1) is chosen to initialise instead of "?".
void initialiseData(records &r);

// Given the parentids and corresponding values, returns the index in the cpt array.
int getIndex(vector<int> parent_ids, vector<int> values);

//prepares the help arrays with reduces the code redundancy.
void make_help_arrays();

// Updates the network with the changes CPT table values for each node
//input : updated cpt table
void insertCpt(vvf cpt);

//Obtained from counting the "observations table of records class"
//Laplacian smoothing applied
vvf unnormalisedCPT(records record);

// Calculates normalised CPT values from the given CPT values
vector<vector<float> > normalisedCPT(vvf count_cpt);

//applies the markov model of substituting the "?"
//Takes into account
//        the records of the observations.
//        confidence values taken from CPT tables of the graph node (calculated)
//Outputs
//        the updated observations as per the hidden markov model
void markovmodel_update(records &r);

//Stopping criteria with the time as parameter
//Could be modified to use convergence as the stopping criteria
void expectationMax(records &r);
