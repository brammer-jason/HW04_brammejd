#pragma once

#define TEXTURESIZE 1024
#define AppWidth 1024
#define AppHeight 768

#include <cinder/app/App.h>
#include <Starbucks.h>
#include <Resources.h>
#include <vector>
#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/Color.h"

using namespace ci;
using namespace ci::app;
using namespace std;

struct node {
	node* left;
	node* right;
	Entry* entry;
	Color8u* color;

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

	/*
	 *calls the private function drawSBNodes
	 */
	void drawStarbucks(uint8_t* dataArray);

	/*
	 *sets and individual pixel to the pass in color
	 */
	void setPixel(int x, int y, Color8u* c, uint8_t* dataArray);

	/*
	 *draws a line between two points
	 */
	void drawLine(int x1, int y1, int x2, int y2, Color8u* c, uint8_t* dataArray);

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

	/*
	 *called from drawStarbucks() and does all the heavy lifting for drawing
	 */
	void drawSBNodes(uint8_t* dataArray, node* n);



	node* sentinelNode; //the first node in the binary search tree.
	int numEntries; 
	Entry* copyEntries; //copy of entries passed to build method
	Entry* closestEntry; //tracks the closest entry found
	//double distance; //distance to be calculated
	double bestDistance; //keeps track of the best distance foudn so far
};