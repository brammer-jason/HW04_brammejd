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
		getline(in, line, ',');
		newEntry->location = line;

		double x1;
		in >> x1;
		in.get();
		newEntry->x = x1;

		double y1;
		in >> y1;
		in.get();
		newEntry->y = y1;
		
		list.push_back(*newEntry);
	}

	for(int i = 0; i < list.size(); i++){
		console() << list[i].location << " " << list[i].x << " " << list[i].y << std::endl;
	}


	
}

double HW04App::string_to_double(const string& s)
{
	istringstream i(s);
	double d;
	if(!(i >> d)) return 0;
	return d;
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
