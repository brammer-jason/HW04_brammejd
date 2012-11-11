#pragma once
#include "brammejd_Starbucks.h"
#include <math.h>

node::node(){
	left = right = NULL;
	entry = NULL;
	c->b = 0;
	c->g = 0;
	c->r = 0;
}

node::node(Entry* e){
	left = right = NULL;
	entry = e;
	c->b = 0;
	c->g = 0;
	c->r = 0;
}

/*
 *inserts an entry into the binary tree
 */
node* brammejd_Starbucks::insert(Entry* entry, node* newNode)
{
	if(newNode == NULL){ 
		return new node(entry); //if no node exists here we add a new one
	}

	//parameters given for not storing duplicates is taken care of here
	if((abs(newNode->entry->x - entry->x) <= 0.00001) && (abs(newNode->entry->y - entry->y) <= 0.00001)){
		return newNode;
	}

	//this is where we sort on x, if x is greater than the node's x we go right
	//and right if x is less than the node's x
	if(entry->x < newNode->entry->x){
		newNode->left = insert(entry, newNode->left);
	} else {
		newNode-> right = insert(entry, newNode->right);
	}
	return newNode;
}

/*
 *build piggybacks on insert by passing in each entry one at a time and by using the sentinelNode
 *to give it a root to build upon
 *I am not confident that I have made a deep copy of the Entry array c as requested
 *I am going to investigate further until deadline
 */
void brammejd_Starbucks::build(Entry* c, int n)
{
	numEntries = n;
	copyEntries = new Entry[n];

	for(int i = 0; i < n; i++){
		
		double x1 = c[i].x;
		double y1 = c[i].y;
		string id = c[i].identifier;

		copyEntries[i].identifier = id;
		copyEntries[i].x = x1;
		copyEntries[i].y = y1;
	}

	c = copyEntries;

	sentinelNode = new node(c);

	for(int i = 0; i < n; i++){
		insert(&copyEntries[i], sentinelNode);
	}
}

/*
 *returns the nearest Starbucks in the data set to the given x,y coordinates.
 */
Entry* brammejd_Starbucks::getNearest(double x, double y)
{
	//this effectively makes our box .1 by .1 since we look left and right of x
	//this should be tweaked based on what the data looks like
	//the farther away the points are the bigger this number should be
	double boxSize = 0.05;

	//we set this to the worst possible distance we could possibly get
	//to accommodate any data set
	bestDistance = sqrt(2.0); 

	//reset our closestEntry to Null each time getNearest is called so it can be queried over and over
	closestEntry = NULL;

	//this counter is simply for debugging and looking at how many times the box must expand
	//if no point is found
	int counter = 0;

	//a local variable to be returned
	Entry* nearestStarbucks = new Entry();
	nearestStarbucks = NULL;

	bool done = false;

	//this loop is critical as it will increase the size of the box if there was no starbucks found
	//at all and then try again with a bigger box
	//if the boxSize exceeds one there are no points in the data set
	while(!nearestStarbucks && !done){
		nearestStarbucks = searchTree(boxSize, x, y, sentinelNode); //traverses the tree 
		boxSize += 0.05;	//iterates the box size for next loop through if nothing was found
		counter++; //counts how many times the box had to grow

		//if our boxSize exceeds 1.0 we are done
		if(boxSize > 1.0){
			done = true;
		}
	}

	return nearestStarbucks;
}

/*
 *searchTree only looks at points that fall inside the x range given by x and the boxSize
 *the goal here is to skip all of the tree that does not apply so that we still end up with a fast solution
 *even though once we reach a certain box size we may calculate the distance to more than one point
 *but we will have skipped a sufficient amount and the speed result will be desirable
 */
Entry* brammejd_Starbucks::searchTree(double boxSize, double x, double y, node* sentinel){

	if(sentinel == NULL){
		return NULL;
	}

	//this tests the bounds of the given point with the current node
	//if the point lies within the box relative to the x,y coordinates then its 
	//distance is calculated here and tested against the best known point
	//variables are updated accordingly if it is a better point
	if(sentinel->entry->x <= x + boxSize && sentinel->entry->x >= x - boxSize &&
	   sentinel->entry->y <= y + boxSize && sentinel->entry->y >= y - boxSize){
		distance = calcDistance(sentinel->entry->x, x, sentinel->entry->y, y);
		if (distance < bestDistance){
			bestDistance = distance;
			closestEntry = sentinel->entry;
		}
	}

	//if left exists we check to see if we need to look there
	if(sentinel->left){
		//if the left node qualifies in the range of x we head that direction
		//otherwise we move on
		if(sentinel->left->entry->x <= x + boxSize && sentinel->left->entry->x >= x - boxSize){
			closestEntry = searchTree(boxSize, x, y, sentinel->left);
		}
	}
	//if left exists we check to see if we need to look there
	if(sentinel->right){
		//if the right node qualifies in the range of x we head that direction
		//otherwise we move on
		if(sentinel->right->entry->x <= x + boxSize && sentinel->right->entry->x >= x - boxSize){
			closestEntry = searchTree(boxSize, x, y, sentinel->right);
		}
	}

	return closestEntry;

}

/*
 *distance between two points calculated here
 */
double brammejd_Starbucks::calcDistance(double x1, double x2, double y1, double y2){
	return sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
}

void brammejd_Starbucks::drawStarbucks(uint8_t* dataArray){
	drawSBNodes(dataArray, sentinelNode);
}

void brammejd_Starbucks::drawSBNodes(uint8_t* dataArray, node* n){
	if(n == NULL) return;

	drawSBNodes(dataArray, n->left);
	drawSBNodes(dataArray, n->right);

	int scaledX = n->entry->x * AppWidth;
	int scaledY = n->entry->y * AppHeight;

	int offset = 3*(scaledX + scaledY*TEXTURESIZE);

	dataArray[offset] = n->c->r;
	dataArray[offset + 1] = n->c->b;
	dataArray[offset + 2] = n->c->g;

}