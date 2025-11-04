#ifndef HEAP_H
#define HEAP_H


#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <iostream>
#include <cassert>
#include <map>
#include "graph.hpp"
using namespace std;

class Heap
{
	public:
	  	std::vector<Node*> store;
        std::map<Node*,int> Ind;
        std::map<Node*,int> &M;
	  	unsigned sz = 0;
	  	int swap_count = 0;
	  	Heap(std::map<Node*,int> &M1):M(M1){
        };
	  	int  size();       
	  	void reset();      
	  	void removeLast(); 
	  	void append(Node* k);
	  	int parent(int);
	  	int left(int);
	  	int right(int);
        void swap(int i, int j);
	  	Node* min();          
	  	void insert(Node* k); 
	  	void heapify(int i);
	  	void deleteMin();   
	  	void buildHeap();    
        void decreasepriority(Node* ch);
};
inline int Heap::size() 
{
	return sz;
}

inline void Heap::reset() 
{
  	store.clear();
  	sz = 0;
}

inline void Heap::removeLast() 
{
  	sz = sz - 1;
}

inline void Heap::append(Node* k) 
{
  	if( sz == store.size() )
    	{
    		store.push_back(k); 
    	}
  	else
  	{
    		store[sz] = k;
    	}
  	Ind[k]=sz;
  	sz = sz + 1;
}




inline int Heap::parent(int i) 
{
	return (i-1) >> 1;
}
inline int Heap::left(int i) 
{
  	return (i << 1) + 1;
}
inline int Heap::right(int i) 
{
  	return (i << 1) + 2;
}
inline Node* Heap::min() 
{
  	return store[0];
}
inline void Heap::swap(int i, int j) 
{
	std::swap(store[i],store[j]);
    Ind[store[j]]=j;
    Ind[store[i]]=i;
	swap_count += 1;
}
inline void Heap::insert(Node* v) 
{
  	append(v);
}
inline void Heap::heapify(int i) 
{
  	if( i >= size() )
  	{
  		return;
  	}
  	auto l = left(i);
  	auto r = right(i);
  	auto smallest = i;
  	if( l < size() ) 
  	{
  		smallest = ( M[store[l]] < M[store[i]] ) ? l : i;
  	}
  	if( r < size() ) 
  	{
  	  	smallest = ( M[store[r]] < M[store[smallest]] ) ? r : smallest;
  	}
  	if( i == smallest )
  	{
  		return;
  	} 
  	swap(i,smallest);
  	heapify(smallest);
}
inline void Heap::deleteMin() 
{
    Node* remove=store.back();
  	swap( 0, size()-1 );
  	removeLast();
    Ind.erase(remove);
  	if(size()>0)heapify(0);
}
inline void Heap::buildHeap() 
{
  	for( int i = size() -1; i >= 0; i-- ) 
  	{
    		heapify(i);
  	}
}
inline void Heap::decreasepriority(Node* ch){
    auto i=Ind[ch];
    while(i>0 && M[store[parent(i)]]>M[store[i]]){
        swap(i,parent(i));
        i=parent(i);
    }
}

#endif 

