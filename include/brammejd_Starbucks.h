#include <Starbucks.h>
#include <Resources.h>

using namespace std;

struct node {
	node* left;
	node* right;
	Entry* entry;
	node();
	node(Entry* e);
};

node::node(Entry* e){
	left = right = NULL;
	entry = e;
}

node::node(){
	left = right = NULL;
	entry = NULL;
}

class brammejd_Starbucks : public Starbucks
{
public:
	void build(Entry* c, int n);
	Entry* getNearest(double x, double y);
	node* insert(Entry* entry, node* node);
	Entry* sentinelEntry;

private:
	node* sentinelNode;
	Entry* e;
	double x;
	double y;
};

void brammejd_Starbucks::build(Entry* c, int n)
{

	sentinelEntry = new Entry;
	sentinelEntry->identifier = "Sentinel";
	sentinelEntry->x = 0.5;
	sentinelEntry->y = NULL;
			
	sentinelNode = new node(c);




	for(int i = 0; i < n; i++){

		insert(&c[i], sentinelNode);

	}
}

node* brammejd_Starbucks::insert(Entry* entry, node* node)
{
	if(node == NULL) {
		return new node(entry);
	}else if(node->entry->x == entry->x && node->entry->y == entry->y){
		return node;
	}else if(entry->x < node->entry->x){
		return insert(entry, node->left);
	} else {
		return insert(entry, node->right);
	}
	return node;
}

Entry* brammejd_Starbucks::getNearest(double x, double y)
{
	return NULL;
}

