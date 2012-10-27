#include <Starbucks.h>
#include <vector>


using namespace ci;
using namespace ci::app;
using namespace std;

struct node {
	node* left;
	node* right;
	Entry* entry;
};

class brammejd_Starbucks : public Starbucks
{
public:
	void build(Entry* c, int n);
	Entry* getNearest(double x, double y);
	void insert(Entry* entry, node* node);
	node* sentinelNode;
};

void brammejd_Starbucks::build(Entry* c, int n)
{
	sentinelNode->entry->x = 0.5;
	sentinelNode->entry->identifier = "sentinel";
	sentinelNode->entry->y = NULL;
	
	
	for(int i = 0; i < n; i++){
		Entry* entryToAdd = new Entry(c[i]*);
		insert(c[i]*, sentinelNode*);

	}
}

void brammejd_Starbucks::insert(Entry* entry, node* node)
{
	if(node == NULL) {
		node->entry = entry;
		return;
	}else if(node->entry->x == entry->x && node->entry->y == entry->y){
		return;
	}else if(entry->x < node->entry->x){
		insert(entry, node->left);
	} else {
		insert(entry, node->right);
	}
}

Entry* brammejd_Starbucks::getNearest(double x, double y)
{
	return NULL;
}

