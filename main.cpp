#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <cfloat>
#include <fstream>
#include "AdjacencyListGraph.h"
#include "Graph.h"
#include "DFS.h"
#include "Decorator.h"
#include <bits/stdc++.h>

using namespace std;

// ------ Option 0 Functions ------
void insertFromFile(vector<string> &strInput, string filename)
{
    ifstream infile;
    string line;
    infile.open(filename);

    if(!infile)
    {
      cout << "Unable to open file.\n";
    }
  
    while(getline(infile, line))
    {
      strInput.push_back(line);
    }

    infile.close();
}


void processFile(vector<string> &strInput, AdjacencyListGraph& graph)
{
  string firstAirport;
  string secondAirport;
  float cost;

  vector<string>::iterator itr;
  vector<string>::iterator it;
  vector<string> VecOfAiports;
  vector<Vertex*> VecOfVertex;


//Process first aiport (start)
	for(int i = 0; i < strInput.size(); ++i)
	{
    firstAirport = strInput[i].substr(0,3);
    itr = std::find (VecOfAiports.begin(), VecOfAiports.end(), firstAirport);
    if(itr == VecOfAiports.end())
    {
        VecOfAiports.push_back(firstAirport);
        Vertex* insertNewVertex = graph.insertVertex(firstAirport);
        VecOfVertex.push_back(insertNewVertex);
    }
  }

//Process second aiport (destination)
	for(int i = 0; i < strInput.size(); ++i)
  {
    secondAirport = strInput[i].substr(6,3);
    it = std::find (VecOfAiports.begin(), VecOfAiports.end(), secondAirport);
    if(it == VecOfAiports.end()) 
    {
      VecOfAiports.push_back(secondAirport);
      Vertex* insertNewVertex = graph.insertVertex(secondAirport);
      VecOfVertex.push_back(insertNewVertex);
    }
  }
  
  int Index_of_firstAirport;
  int Index_of_secondAirport;
  
	for(int i = 0; i < strInput.size(); ++i)
  {
    firstAirport = strInput[i].substr(0,3);
		secondAirport = strInput[i].substr(6,3);
		for(int j = 0; j < VecOfAiports.size(); j++)
		{
			if(VecOfAiports[j] == firstAirport)
				Index_of_firstAirport = j;
			if(VecOfAiports[j] == secondAirport)
				Index_of_secondAirport = j;
		}
    //Process cost
    cost = stoi(strInput[i].substr(13));
    graph.insertEdge(VecOfVertex[Index_of_firstAirport], VecOfVertex[Index_of_secondAirport], cost); 
  }
    

}      

void displayGraph(AdjacencyListGraph G)
{
  cout << endl;
  G.print();
}

// ------ Option 1 Functions ------
void printPath(Vertex* start, Vertex* end)
{
  Vertex* nextPtr = end;
  string path = "PATH: ";
  stack<string> airports;

  //push everything onto a stack to reverse
  while(nextPtr != nullptr)
  {
    airports.push(nextPtr->getElement());
    nextPtr = nextPtr->prev;
  }

  //build path (string)
  while(!airports.empty())
  {
    path += airports.top();
    
    if(airports.top() != end->getElement())
      path += " -> ";

    airports.pop();
  }

  cout << path << endl;
}

struct compare
{
  constexpr bool operator()(pair<double, Vertex*> p1, pair<double, Vertex*> p2)
  {
     return(p1.first > p2.first);
  }
};

void cheapestFlight(AdjacencyListGraph G, Vertex *start, Vertex *dest)
{
  priority_queue<pair<double, Vertex*>, vector<pair<double, Vertex*>>, compare> pq1;
  priority_queue<pair<double, Vertex*>, vector<pair<double, Vertex*>>, compare> pq2;
  
  //every vertex is INF except start
  for(Vertex* v : G.getVertices())
  {
    if(v == start)
    {
      v->minDist = 0;
      v->prev = nullptr;
    }
    else
      v->minDist = DBL_MAX;

    pair<double, Vertex*> p = make_pair(v->minDist, v);
    pq1.push(p);
  }

  while(!pq1.empty())
  {
    //current distance and vertex
    pair<double, Vertex*> curr = pq1.top();
    pq1.pop();
    
    double dist = curr.first;
    Vertex *v = curr.second;
    
    for(Edge* e : G.outgoingEdges(v))
    {
      //adjV is the vertex opposite to current vertex and each adj edge
      Vertex* adjV = G.opposite(v, e);
      double newDist = dist + e->getElement();

      //update the pq with new values
      if(newDist > dist)
      { 
        //add everything in pq1 to pq2 except for this new pair   
        while(!pq1.empty())
        {
          pair<double, Vertex*> top = pq1.top();
          
          if(top.second != adjV)
            pq2.push(top);
          else
          {
            top.first = newDist;
            top.second->minDist = newDist;
            top.second->prev = v;
            pq2.push(top);
          }

          pq1.pop();
        }

        //add it back to pq1 
        while(!pq2.empty())
        {
          pq1.push(pq2.top());
          pq2.pop();
        }
      }

    }
  }
  
  //output distances here
  cout << "\nThe minimum cost from " << start->getElement() << " to " << dest->getElement() << " is $" << dest->minDist << endl;
  printPath(start, dest);
} 

// ------ Option 2 Function ------
void cheapestRoundTrip(AdjacencyListGraph G, Vertex *start, Vertex *dest)
{
  double totalDist = 0;

  cheapestFlight(G, start, dest);
  totalDist += dest->minDist;

  cheapestFlight(G, dest, start);
  totalDist += start->minDist;

  cout << "\nCheapest round trip from " << start->getElement()
      << " to " << dest->getElement() << ": $" << totalDist << "\n";
}

// ------ Option 3 Function ------
void visitAll(AdjacencyListGraph G, Vertex *v)
{
  int numVisits;
  
  cout << endl;
  DFS search(&G);
  search.dfsTraversal(v);
  cout << "END" << endl;
  
  numVisits = search.getNumVisited();
  if(numVisits != G.numVertices())
    cout << "Unable to visit all airports" << endl;
  else
    cout << "Successfully to visited all airports" << endl;
}

Vertex* findVertex(AdjacencyListGraph G, string element)
{
  for(Vertex* v : G.getVertices())
  {
    if(v->getElement() == element)
      return v;
  }
}

// // ------ Option 4 Functions ------

int getVertexIndex(AdjacencyListGraph& G, Vertex *find)
{
  int index = 0;
  for(auto v : G.getVertices())
  {
    if(v == find)
      return index;

    ++index;
  }
  return index;
}

void calcMinEdges(vector<bool>& visited, AdjacencyListGraph& G, Vertex *start, Vertex *dest, int& leastNumberOfEdges, int currentNumberOfEdges)
{

  int startIndex = getVertexIndex(G, start);
  DFS dfsObj(&G);

  visited[startIndex] = true;
  
  vector<string>::iterator itr;
  
  if(start == dest)
  {
    if(leastNumberOfEdges > currentNumberOfEdges)
      leastNumberOfEdges = currentNumberOfEdges;
  }
  else
  { 

    for(Vertex* v : G.getVertices())
    {
      dfsObj.startVisit(v);  
      dfsObj.visit(v);					// visit v and mark visited
      bool isVisited = dfsObj.isVisited(v);
//*i.isVisited()
      if(!isVisited)
        currentNumberOfEdges++;

      calcMinEdges(visited, G, start, dest, leastNumberOfEdges, currentNumberOfEdges);
    }
  }

  visited[startIndex] = false;
  currentNumberOfEdges--;
      
}

void fewestStop(AdjacencyListGraph G, Vertex *start, Vertex *dest)
{
  DFS dfsObj(&G);
  vector<bool> visited;
  int leastNumberOfEdges = INT16_MAX;
  int currentNumberOfEdges = 0;

  calcMinEdges(visited, G, start, dest, leastNumberOfEdges, currentNumberOfEdges);

    cout << leastNumberOfEdges;
}

int main()
{
  AdjacencyListGraph graph;
  list<Vertex*> vertices = graph.getVertices();
  Vertex* start;
  Vertex * dest;
  bool stop = false;
  string ans; //for user input responses
  vector<string> myVec;

  //set up graph right off the bat
  insertFromFile(myVec, "Flights.txt");
  processFile(myVec, graph);

    char input;
    do {
        cout << endl;
        cout << "0. Display all flights" << endl;
        cout << "1. Find a cheapest flight from one airport to another airport" << endl;
        cout << "2. Find a cheapest roundtrip from one airport to another airport" << endl;
        cout << "3. Find an order to visit all airports starting from an airport" << endl;
        cout << "4. Find flight with the fewest stops from one airport to another." << endl;
        cout << "Q. Exit" << endl;

        cout << "\nYour response: ";
        cin >> input;
        switch (input) {
        case '0':
            displayGraph(graph);
            break;
        case '1':
            cout << "What airport would you like to start at?: ";
            cin >> ans;
            start = findVertex(graph, ans);
            cout << "\nWhat airport would you like to end at?: ";
            cin >> ans;
            dest = findVertex(graph, ans);
            
            cheapestFlight(graph, start, dest);
            break;
        case '2':
            cout << "What airport would you like to start at?: ";
            cin >> ans;
            start = findVertex(graph, ans);
            cout << "\nWhat airport would you like to end at?: ";
            cin >> ans;
            dest = findVertex(graph, ans);
    
            // cheapestFlight(graph, start, dest);
            // cheapestFlight(graph, dest, start);
            cheapestRoundTrip(graph, start, dest);
            break;
        case '3':
            cout << "What airport would you like to start at?: ";
            cin >> ans;
            start = findVertex(graph, ans);
  
            visitAll(graph, start);
        case '4':
          cout << "What airport would you like to start at?: ";
          cin >> ans;
          start = findVertex(graph, ans);
          cout << "\nWhat airport would you like to end at?: ";
          cin >> ans;
          dest = findVertex(graph, ans);
          fewestStop(graph, start, dest);
          break;
        }
    } while (input != 'Q');
 
  return 0;
}

