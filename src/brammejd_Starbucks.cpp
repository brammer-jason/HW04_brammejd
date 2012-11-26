#pragma once
#include "brammejd_Starbucks.h"
#include <math.h>

node::node(){
	left = right = NULL;
	entry = NULL;
	color = new Color8u(0,0,255);
}

node::node(Entry* e){
	left = right = NULL;
	entry = e;
	color = new Color8u(0,0,255);
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
	Entry* nearestStarbucks = NULL;

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
		double distance = calcDistance(sentinel->entry->x, x, sentinel->entry->y, y);
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
	int scaledY = (1 - n->entry->y) * AppHeight;

	setPixel(scaledX, scaledY, n->color, dataArray);
}

void brammejd_Starbucks::setPixel(int x, int y, Color8u* c, uint8_t* dataArray)
{
	//Sets a single pixel on the screen to a specified color
	//setPixel is a dependancy for addLine and addTriangle
	int offset = 3*(x + y*TEXTURESIZE);

	dataArray[offset] = c->r;
	dataArray[offset + 1] = c->b;
	dataArray[offset + 2] = c->g;
	
}

/**
 *Adds a line between two given points (A.3)
 *used http://www.codekeep.net/snippets/e39b2d9e-0843-4405-8e31-44e212ca1c45.aspx as a template
 *@x1 first point x coordinate
 *@y1 first point y coordinate
 *@x2 second point x coordinate
 *@y2 second point y coordinate
 *@c color of the line to be drawn
 */
void brammejd_Starbucks::drawLine(int x1, int y1, int x2, int y2, Color8u* c, uint8_t* dataArray){
	//I changed many of the variables to make them more consistant with the naming conventions
	//I used in this code
	
	int F, x, y;

    if (x1 > x2) {  
		// Swap points if first point is on the right of second point
		// This alleviates the need for two algorithms, one that writes left to right and one that writes right to left
        swap(x1, x2);
        swap(y1, y2);
    }

    // Handle trivial cases separately for algorithm speed up.
    // Trivial case 1: m = +/-INF (Vertical line), no special handling required
    if (x1 == x2) {
        if (y1 > y2){  
			// Swap y-coordinates if the first point is above the second point
            swap(y1, y2);
        }

        x = x1;
        y = y1;
        while (y <= y2) {
            setPixel(x, y, c, dataArray);
            y++;
        }
        return;
    }
    // Trivial case 2: m = 0 (Horizontal line), no special handling required
    else if (y1 == y2) {
        x = x1;
        y = y1;

        while (x <= x2) {
            setPixel(x, y, c, dataArray);
            x++;
        }
        return;
    }

    int dy            = y2 - y1;  // y-increment from p1 to p2
    int dx            = x2 - x1;  // x-increment from p1 to p2
    int dy2           = (dy << 1);  // dy << 1 == 2*dy
    int dx2           = (dx << 1);
    int dy2_minus_dx2 = dy2 - dx2;  // precompute constant for speed up
    int dy2_plus_dx2  = dy2 + dx2;

    if (dy >= 0){    // What to do if slope >= 0
        if (dy <= dx) {
			// Case 1: 0 <= m <= 1 (Original case)
            F = dy2 - dx;    // initial F

            x = x1;
            y = y1;
            while (x <= x2) {
                setPixel(x, y, c, dataArray);
                if (F <= 0) {
                    F += dy2;
                }
                else {
                    y++;
                    F += dy2_minus_dx2;
                }
                x++;
            }
        } else {
			// Case 2: 1 < m < INF (Mirror about y=x line replace all dy by dx and dx by dy)
            F = dx2 - dy;    // initial F

            y = y1;
            x = x1;
            while (y <= y2) {
                setPixel(x, y, c, dataArray);
                if (F <= 0) {
                    F += dx2;
                } else {
                    x++;
                    F -= dy2_minus_dx2;
                }
                y++;
            }
        }
    } else {    // What to do if slope < 0
    
        // Case 3: -1 <= m < 0 (Mirror about x-axis, replace all dy by -dy)
        if (dx >= -dy){
            F = -dy2 - dx;    // initial F

            x = x1;
            y = y1;
            while (x <= x2){
                setPixel(x, y, c, dataArray);
                if (F <= 0){
                    F -= dy2;
                } else {                
                    y--;
                    F -= dy2_plus_dx2;
                }
                x++;
            }
        // Case 4: -INF < m < -1 (Mirror about x-axis and mirror about y=x line, replace all dx by -dy and dy by dx)
        } else { 
            F = dx2 + dy;    // initial F

            y = y1;
            x = x1;
            while (y >= y2) {
                setPixel(x, y, c, dataArray);
                if (F <= 0) {
                    F += dx2;
                } else {
                    x++;
                    F += dy2_plus_dx2;
                }
                y--;
            }
        }
    }
}