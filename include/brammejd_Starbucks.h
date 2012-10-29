#pragma once
#include <cinder/app/App.h>
#include <Starbucks.h>
#include <Resources.h>
#include <vector>


using namespace std;

struct node {
	node* left;
	node* right;
	Entry* entry;

	node();
	node(Entry* e);
};

class brammejd_Starbucks : public Starbucks {
public:
	/*
	 *builds the binary tree using the passed in array of entries c and c's size
	*/
	void build(Entry* c, int n);

	/*
	 *returns the nearest Starbucks location relative to the one entered
	*/
	Entry* getNearest(double x, double y);

private:
	/*
	 *the core algorithm for searching
	 *this algorithm uses a 0.1 by 0.1 box as a starting box and increments this box if no match is found by .05
	 *the idea is to increment the search box parameter while using the speed of binary search to quickly find all 
	 *points that fit inside that box.
	*/
	Entry* searchTree(double boxSize, double x, double y, node* sentinel);

	/*
	 *returns the distance between two points
	*/
	double calcDistance(double x1, double x2, double y1, double y2);

	/*
	 *inserts a new node into the binary search tree (sorted on x values!!)
	*/
	node* insert(Entry* entry, node* newNode);

	node* sentinelNode; //the first node in the binary search tree.
	int numEntries; 
	Entry* copyEntries; //copy of entries passed to build method
	Entry* closestEntry; //tracks the closest entry found
	double distance; //distance to be calculated
	double bestDistance; //keeps track of the best distance foudn so far
};