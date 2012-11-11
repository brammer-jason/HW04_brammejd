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
	void prepareSettings(Settings* settings);

	void readFromFile();
	void convertVectorToArray();
	vector<Entry> list;
	brammejd_Starbucks starbucks;
	Entry* entryArray;

	Color8u BGColor;
	Color8u SBColor;

	bool drawNodes;

  private:
	Surface* mySurface;
	uint8_t* dataArray;

};