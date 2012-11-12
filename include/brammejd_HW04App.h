/*
 *Jason Brammer, CSE 278, HW04 Phase 3 goals
 *
 *Goal D: curZoom and all related functions satisfy
 *Goal A: The overlay and the drawing of the starbucks satisfy
 *Goal B: The program draws a line to the "closest Starbucks", and apparently my searching algorithm isn't as accurate as I thought
 *Goal H: I attempted to create a full population density grid of the Census data that could be switched back and forth
		  but I think a lack of understand in how to use the Cinder Iter function resulted in a failure to draw.

  *Since a controls pane was not a requirement for this assignment I did not design one. The controls are as follows:
  "m" = shows/hides US Map
  "backspace" = redraws the data, was to be more useful for the density maps
  "," zooms out
  "." zooms in, Note: while zoomed in points may not be selected!
  directional arrows scroll accordingly
  "F1" will swap between census 2000 and 2010 data
 */


#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <Starbucks.h>
#include <brammejd_Starbucks.h>
#include "cinder/gl/Texture.h"

using namespace ci;
using namespace ci::app;
using namespace std;

struct census{
	int pop;
	double x;
	double y;
};

class HW04App : public AppBasic {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
	void prepareSettings(Settings* settings);
	void resetBackground(uint8_t* dataArray, Color8u BGColor);
	
	
	void curZoom();

	void readFromFile();
	void convertVectorToArray();
	vector<Entry> list;
	vector<census> censusList;
	brammejd_Starbucks starbucks;
	Entry* entryArray;
	void keyDown( KeyEvent event );
	void drawDensity(uint8_t* dataArray);


	int zoom;
	int yOffset;
	int xOffset;

	gl::Texture map;

	Color8u BGC;
	
	Entry* closestSB;

	bool drawNodes;
	bool oldDensity;
	bool zoomed;
	bool drawMap;

	Surface* mySurface;
	uint8_t* dataArray;

	Surface* zoomSurface;
	uint8_t* zoomPixels;

};