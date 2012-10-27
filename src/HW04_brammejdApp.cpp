#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <Starbucks.h>
#include <brammejd_Starbucks.h>

using namespace ci;
using namespace ci::app;
using namespace std;

struct GridItem{
	double startX;
	double stopX;
	double startY;
	double stopY;
	bool empty;
	double boxSize;
};

class HW04App : public AppBasic {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
	void populateGrid();
	double boxCalc(GridItem* item);
	vector<Entry> list;
	vector<GridItem> Gridlist;
	
};

void HW04App::setup()
{

	fstream in("Starbucks_2006.csv");

	//vector<Entry> entryList;
	string line = "not changed";

	int counter = 0;

	if(!in.is_open()){
		console() << "failed to open file" << std::endl;
		return;
	} 
	/*
	Entry *sentinelEntry = new Entry;
	sentinelEntry->x = 0.5;
	sentinelEntry->y = NULL;
	sentinelEntry->identifier = "Sentinel";
	list.push_back(*sentinelEntry);
	*/
	while(!in.eof()){
		Entry *newEntry = new Entry;

		getline(in, line, ',');			//reads in the location information as a string
		newEntry->identifier = line;	//puts that location into a new Entry

		double x1;
		in >> x1;			//this will read in until it hits the , as a double
		in.get();			//this clears out the , that is left over
		newEntry->x = x1;	//puts the x value into the Entry

		double y1;			//rinse/repeat of above except for y value
		in >> y1;			
		in.get();
		newEntry->y = y1;
		
		list.push_back(*newEntry);	//puts our Entry into the Vector of Entries.
	}

	Entry* entryArray = new Entry[list.size()];

	for(int i = 0; i < list.size(); i++){
		//this just tests looping ability of the vector and for data correctness
		console() << list[i].identifier << " " << list[i].x << " " << list[i].y << std::endl;
		entryArray[i] = list[i];
	}
	console() << list.size() << std::endl;
	brammejd_Starbucks* starbucks = new brammejd_Starbucks();
	starbucks->build(entryArray, list.size());
	
}

void HW04App::populateGrid(){
	double stX = 0.0;
	double stY = 0.0;
	double endX = 0.1;
	double endY = 0.1;
	int counter = 0;

	for(int i=0; i < 100; i++){
		GridItem* newGrid = new GridItem;
		newGrid->startX = stX;
		newGrid->stopX = endX;
		newGrid->startY = stY;
		newGrid->stopY = endY;

		if(counter < 10){
			stX = stX + 0.1;
			endX = endX + 0.1;
		} else {
			stX = 0.0;
			endX = 0.1;
			stY = stY + 0.1;
			endY = endY + 0.1;
		}

		Gridlist.push_back(*newGrid);
	}
}

//void Starbucks::build(Entry* entry, int list.size()0:

double HW04App::boxCalc(GridItem* item){
	return 0.0;
}

void HW04App::mouseDown( MouseEvent event )
{
}

void HW04App::update()
{
}

void HW04App::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP_BASIC( HW04App, RendererGl )
