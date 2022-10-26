#pragma once
#include <string>
#include <map>
#include "Graph.h"

using namespace std;

// Created by T. Vo for CSCI 230
// Based on C++ code fragment of Goodrich book 

class Object // generic object
{					
  public:
  	virtual int      intValue()    const  throw(bad_cast);
  	virtual string   stringValue() const  throw(bad_cast);
};


class String : public Object 
{
  private:
  	string value;

  public:
  	String(string v = "") : value(v) { }
  	string getValue() const
  	{
  		return value;
  	}
};


class Integer : public Object 
{
  private:
  	int value;

  public:

  	Integer(int v = 0) : value(v) { }

  	int getValue() const
  	{
  		return value;
  	}
};

int Object::intValue() const throw(bad_cast) {		// cast to Integer

	const Integer* p = dynamic_cast<const Integer*>(this);
  
	if (p == NULL) throw bad_cast(); // ("Illegal attempt to cast to Integer");
  
	return p->getValue();
}

string Object::stringValue() const throw(bad_cast) {		
  // cast to String
	const String* p = dynamic_cast<const String*>(this);
  
	if (p == NULL) throw bad_cast(); // ("Illegal attempt to cast to Srring");
  
	return p->getValue();
}


class Decorator 
{
  // member data
  private:									
  	std::map<string, Object*> map1;			// the map

  public:

    //returns pointer to object
  	Object *get(const string& a)			// get value of attribute
  	{
  		return map1[a];
  	}

  	void set(const string& a, Object* d)	// set value
  	{
  		map1[a] = d;
  	}
};

