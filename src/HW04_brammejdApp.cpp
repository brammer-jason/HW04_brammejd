#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include <fstream>
#include <sstream>
#include <vector>

using namespace ci;
using namespace ci::app;
using namespace std;


class Entry{
public:
	string location;
	double x;
	double y;
};

class HW04App : public AppBasic {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
	double string_to_double(const string& s);
};

void HW04App::setup()
{

	fstream in("Starbucks_2006.csv");
	vector<Entry> list;
	//vector<Entry> entryList;
	string line = "not changed";

	int counter = 0;

	if(!in.is_open()){
		console() << "failed to open file" << std::endl;
		return;
	} 

	while(!in.eof()){
		Entry *newEntry = new Entry;

		getline(in, line, ',');			//reads in the location information as a string
		newEntry->location = line;		//puts that location into a new Entry

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

	for(int i = 0; i < list.size(); i++){
		//this just tests looping ability of the vector and for data correctness
		console() << list[i].location << " " << list[i].x << " " << list[i].y << std::endl;
	}


	
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
