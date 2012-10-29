/*
 *Jason Brammer CSE274
 *This program uses the speed of a binary search tree to locate x values in a given range quickly and then
 *"brute forces" that smaller data set until it finds the closest point.
*/
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

class HW04App : public AppBasic {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
	void readFromFile();
	void convertVectorToArray();
	vector<Entry> list;
	brammejd_Starbucks starbucks;
	Entry* entryArray;
};

void HW04App::setup()
{
	readFromFile();
	convertVectorToArray();
	starbucks.build(entryArray, list.size());
	Entry* closestSB = starbucks.getNearest(0.24, 0.308);
	console() << closestSB->identifier << " " << closestSB->x << " " << closestSB->y << std::endl;
}

/*
 *converts the vector that was created into an Array for requirement purposes
 */
void HW04App::convertVectorToArray(){
	entryArray = new Entry[list.size()];
	
	for(int i = 0; i < list.size(); i++){
		entryArray[i] = list[i];
		//console() << entryArray[i].identifier << ", " << entryArray[i].x << ", " << entryArray[i].y << std::endl;
	}
}

void HW04App::readFromFile(){
	//creates an fstream to read from the file of locations
	fstream in("Starbucks_2006.csv");

	string line;

	if(!in.is_open()){
		console() << "failed to open file" << std::endl;
		return;
	} 

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

	in.close();
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
