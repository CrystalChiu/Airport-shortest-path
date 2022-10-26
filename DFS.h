#pragma once
#include "Decorator.h"
#include "Graph.h"
#include <iostream>

using namespace std;

// Created by T. Vo for CSCI 230
// Based on C++ code fragment of Goodrich book 

// Make sure that Vertex and Edge is-a Decorator

class DFS {										// generic DFS
protected: 										// member data
	Graph *graph;								// the graph
	Vertex *start;								// start vertex
	Object *yes;
  Object *no;							// decorator values
  int numVisited = 0;

public:											// member functions
	DFS(Graph *g);								// constructor
	void initialize();							// initialize a new DFS
	void dfsTraversal(Vertex *v);				// recursive DFS utility
												// overridden functions
	virtual void startVisit(Vertex *v) {}		// arrived at v
												// discovery edge e
	virtual void traverseDiscovery(Edge *e, Vertex *from) {}
	// back edge e
	virtual void traverseBack(Edge *e, Vertex *from) {}
	virtual void finishVisit(Vertex *v) {}			// finished with v

	virtual bool isDone() const { return false; }	// finished?

  int getNumVisited();
													  
	void visit(Vertex *v) { v->set("visited", yes); }
	void visit(Edge *e) { e->set("visited", yes); }
	void unvisit(Vertex *v) { v->set("visited", no); }
	void unvisit(Edge *e) { e->set("visited", no); }
	bool isVisited(Vertex *v) { return v->get("visited") == yes; }
	bool isVisited(Edge *e) { return e->get("visited") == yes; }
};

DFS::DFS(Graph *g) : graph(g), yes(new Object), no(new Object) {}

void DFS::initialize() {
	list<Vertex *> verts = graph->getVertices();
	for (auto pv = verts.begin(); pv != verts.end(); ++pv)
		unvisit(*pv);							// mark vertices unvisited

	list<Edge *> edges = graph->getEdges();
	for (auto pe = edges.begin(); pe != edges.end(); ++pe)
		unvisit(*pe);							// mark edges unvisited
}

int DFS::getNumVisited()
{
  int temp = numVisited;
  numVisited = 0;
  
  return temp;
}

void DFS::dfsTraversal(Vertex *v) 
{
  //initialize();
	startVisit(v);  
  visit(v);					// visit v and mark visited
  cout << v->getElement() << " -> ";
  ++numVisited;
  
	vector<Edge *> incident = graph->outgoingEdges(v);
  
	auto pe = incident.begin();

  // visit v's incident edges
	while (!isDone() && pe != incident.end()) 
  {	
		Edge *e = *pe++;
    
		if (!isVisited(e)) // discovery edge?
    {					
			visit(e);							// mark it visited
      
			Vertex *w = graph->opposite(v, e);	// get opposing vertex
      
			if (!isVisited(w)) // unexplored?
      {				
				traverseDiscovery(e, v);		// let's discover it
				if (!isDone()) 
          dfsTraversal(w);	// continue traversal
			}
			else traverseBack(e, v);			// process back edge
		}
	}
  
	if (!isDone()) finishVisit(v);				// finished with v
}






