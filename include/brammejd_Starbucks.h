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

struct GridItem{
	double startX;
	double stopX;
	double startY;
	double stopY;
	bool empty;
	double boxSize;
};

class brammejd_Starbucks : public Starbucks {
public:
	void build(Entry* c, int n);
	Entry* getNearest(double x, double y);

private:
	Entry* searchTree(double boxSize, double x, double y, node* sentinelNode);
	double calcDistance(double x1, double x2, double y1, double y2);
	node* insert(Entry* entry, node* node);

	node* sentinelNode;
	int numEntries;
	Entry* copyEntries;
	Entry* closestEntry;
	double bestDistance;
};