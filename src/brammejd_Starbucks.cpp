#pragma once
#include "brammejd_Starbucks.h"
#include <math.h>

node::node(){
	left = right = NULL;
	entry = NULL;
}

node::node(Entry* e){
	left = right = NULL;
	entry = e;
}

node* brammejd_Starbucks::insert(Entry* entry, node* newNode)
{
	if(newNode == NULL){ 
		return new node(entry);
	}
	if((abs(newNode->entry->x - entry->x) <= 0.00001) && (abs(newNode->entry->y - entry->y) <= 0.00001)){
		return newNode;
	}

	if(entry->x < newNode->entry->x){
		newNode->left = insert(entry, newNode->left);
	} else {
		newNode-> right = insert(entry, newNode->right);
	}
	return newNode;
}

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

Entry* brammejd_Starbucks::getNearest(double x, double y)
{
	double boxSize = 0.1;
	
	bestDistance = 1.0;
	closestEntry = NULL;

	int counter = 0;
	Entry* nearestStarbucks = new Entry();
	nearestStarbucks = NULL;
	while(!nearestStarbucks){
		nearestStarbucks = searchTree(boxSize, x, y, sentinelNode);
		boxSize += 0.05;	
		counter++;
	}

	return nearestStarbucks;
}

Entry* brammejd_Starbucks::searchTree(double boxSize, double x, double y, node* sentinelNode){

	if(sentinelNode == NULL){
		return NULL;
	}

	if(sentinelNode->entry->x <= x + boxSize && sentinelNode->entry->x >= x - boxSize &&
	   sentinelNode->entry->y <= y + boxSize && sentinelNode->entry->y >= y - boxSize){
		double distance = calcDistance(sentinelNode->entry->x, x, sentinelNode->entry->y, y);
		if (distance < bestDistance){
			bestDistance = distance;
			closestEntry = sentinelNode->entry;
		}
	}

	if(sentinelNode->left){
		if(sentinelNode->left->entry->x <= x + boxSize && sentinelNode->left->entry->x >= x - boxSize){
			return searchTree(boxSize, x, y, sentinelNode->left);
		}
	}
	if(sentinelNode->right){
		if(sentinelNode->right->entry->x <= x + boxSize && sentinelNode->right->entry->x >= x - boxSize){
			return searchTree(boxSize, x, y, sentinelNode->right);
		}
	}

	return closestEntry;

}

double brammejd_Starbucks::calcDistance(double x1, double x2, double y1, double y2){
	return sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
}