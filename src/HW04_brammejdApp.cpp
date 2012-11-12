/*/*
 *Jason Brammer CSE274
 *This program uses the speed of a binary search tree to locate x values in a given range quickly and then
 *"brute forces" that smaller data set until it finds the closest point.
*/

#pragma once
#include <brammejd_HW04App.h>
#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/ImageIo.h"
#include "Resources.h"
#include "cinder/gl/Texture.h"

using namespace ci;
using namespace ci::app;
using namespace std;

void HW04App::prepareSettings(Settings* settings){
	(*settings).setWindowSize(AppWidth, AppHeight);
	(*settings).setResizable(false);
}

void HW04App::setup(){
	//use Census_2000.csv by default
	oldDensity = true;

	//drawMap set to false by default
	drawMap = false;

	//zoomed set to false by default
	zoomed = false;

	//sets our offsets to the base value and our zoom factor to regular factor of 1
	xOffset = 0;
	yOffset = 0;
	zoom = 1;

	//loads our overlay map to compare our points to the map
	map = loadImage("map.jpg");
	
	//base surface
	mySurface = new Surface(TEXTURESIZE, TEXTURESIZE, false);
	dataArray = mySurface->getData();

	//zoomed surface that is used to represent zoomed portions of mySurface
	zoomSurface = new Surface(TEXTURESIZE, TEXTURESIZE, false);
	zoomPixels = zoomSurface->getData();

	//drawNodes is set to true so that we draw at least once
	drawNodes = true;

	//default background color is set to black
	BGC = Color8u(0,0,0);

	readFromFile();
	convertVectorToArray();
	starbucks.build(entryArray, list.size());
	
	//Entry* closestSBTest = starbucks.getNearest(0.24, 0.308); //used for testing!!!
	//console() << closestSBTest->identifier << " " << closestSBTest->x << " " << closestSBTest->y << std::endl; //used for testing!!
}

void HW04App::update()
{
	//draws our screen according to zoom factor
	curZoom();

	//draws our data
	if(drawNodes){
		resetBackground(dataArray, BGC);
		drawDensity(dataArray);
		starbucks.drawStarbucks(dataArray);
		drawNodes = false;
	}
}

void HW04App::draw()
{
	gl::draw(*zoomSurface);

	if(drawMap){
		gl::draw(map);
	}
}

/*
 *this draws our screen to whatever the current zoom scale is
 */
void HW04App::curZoom(){
	for(int g = 0; g < TEXTURESIZE; g++){
		for(int i = 0; i < TEXTURESIZE; i++){
			int x = i;
			int y = g;
			int index = 3*(x + y*TEXTURESIZE);
			int index2 = 3*(((x/zoom)+xOffset) + (((y/zoom)+yOffset)*TEXTURESIZE));
			
			//This sets our zoom surface to the corresponding pixels of the dataArray
			//after the zoom factor has been applied
			zoomPixels[index] = dataArray[index2];
			zoomPixels[index+1] = dataArray[index2+1];
			zoomPixels[index+2] = dataArray[index2+2];
			
		}
	}
}

/*
 *reads in a census file based on whether or not we want the older density vs. the new one
 *the first 4 reads chew up non useful data so that we can get to the last three fields
 *This is an attempt at Goal H. I wanted to represent the overall population density in all areas
 *This would be useful to see if there is an untapped population that has a high enough density
 *to make a profit
 */
void HW04App::drawDensity(uint8_t* dataArray){
	string whichCensus;
	if(oldDensity){
		whichCensus = "Census_2000.csv";
	} else {
		whichCensus = "Census_2010.csv";
	}
	
	fstream in(whichCensus);

	if(!in.is_open()){
		console() << "failed to open file" << std::endl;
		return;
	} 

	while(!in.eof()){
		census *newCensus = new census;

		int garbage1;

		//The next for assignments to garbage1 are clearing unwanted data
		in >> garbage1;
		in.get();

		in >> garbage1;
		in.get();
		 
		in >> garbage1;
		in.get();

		in >> garbage1;
		in.get();

		//reads in the population information
		int population;
		in >> population;
		in.get();
		newCensus->pop = population;

		//reads in the x coordinate for that city
		double x;
		in >> x;
		in.get();
		newCensus->x = x;

		//reads in the y coordinate for that city
		double y;
		in >> y;
		in.get();
		newCensus->y = y;

		censusList.push_back(*newCensus);	//puts our census data into the Vector of census.
	}

	in.close();

	//Iter is a cinder class that is useful for iterating through the pixels in a surface
	//The idea here is to increment the red value of a pixel if it matches a city that is in the censusList
	//The effect is that the more a pixel contains a city the more red the pixel becomes
	Surface::Iter iter = mySurface->getIter();

	//Perhaps I'm not using the iter class correctly because this doesn't seem to be doing anything
	//but the intended effect can be seen when examining the logic behind my loops
	for(int i = 0; i < censusList.size(); i++){
		while(iter.line()){
			while(iter.pixel()){
				//checks the current pixel vs the x,y coordinates of the censusList
				//if they match the red value interates by 0.1f
				//One would imagine that this would show some red as it goes through the list but it doesn't
				if (censusList[i].x*AppWidth == iter.getPos().x && censusList[i].y*AppHeight == iter.getPos().y){
					iter.r() = iter.r() + 0.1f;
				}
			}
		}
	}
}

/*
 * resets the background to the designated Background color BGColor
 */
void HW04App::resetBackground(uint8_t* dataArray, Color8u BGColor){
	for(int i = 0; i < AppHeight; i++){
		for(int j = 0; j < AppWidth; j++){
			int offset = 3*(j + i*TEXTURESIZE);
			dataArray[offset] = BGColor.r;
			dataArray[offset+1] = BGColor.g;
			dataArray[offset+2] = BGColor.b;
	
		}
	}
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
	if(zoom == 1){

		Color8u* curColor = new Color8u(255,0,0); //draws all our modifications in red

		//draws the point that was clicked
		starbucks.setPixel(event.getX(), event.getY(), curColor, dataArray);

		//converts our clicked x,y into values that make sense with our starbucks data
		double clickX = (((double)event.getX() / (double)AppWidth));
		double clickY = (1 - ((double)event.getY() / (double)AppHeight));

		console() << clickX << endl; //used for debugging
		console() << clickY << endl; //used for debugging

		//returns the closest starbucks relative to our clicked x and y
		closestSB = starbucks.getNearest(clickX, clickY);

		//converts closest starbucks point into a point that makes sense to the window's scale
		int pointX = closestSB->x * AppWidth;

		//does the same as above with the x but also inverts it due to the difference between
		//the computer drawn origin and the logical origin of the data
		int pointY = (1 - closestSB->y) * AppHeight;

	
		starbucks.drawLine(event.getX(), event.getY(), pointX, pointY, curColor, dataArray);
		console() << closestSB->identifier << " " << closestSB->x << " " << closestSB->y << std::endl;
	} else {
		console() << "Cannot select points while zoomed" << endl;
	}
}

void HW04App::keyDown( KeyEvent event){
	
	//this will toggle the map overlay
	if(event.getCode() == KeyEvent::KEY_m){
		if(drawMap){
			drawMap = false;
		} else {
			drawMap = true;
		}
	}

	//this will zoom in if the period is pressed
	if(event.getCode() == KeyEvent::KEY_PERIOD){
		console() << "Zoom In" << endl;
		zoom *= 2;
	}
	//this will zoom in in the comma is pressed
	if(event.getCode() == KeyEvent::KEY_COMMA){
		console() << "Zoom Out" << endl;
		if(zoom != 1){
			zoom /= 2;
			if(xOffset > (TEXTURESIZE - (TEXTURESIZE/zoom))){
				xOffset = (TEXTURESIZE - (TEXTURESIZE/zoom));
			}
			if(yOffset > (TEXTURESIZE - (TEXTURESIZE/zoom))){
				yOffset = (TEXTURESIZE - (TEXTURESIZE/zoom));
			}
		}
	}
	//Scroll screen up
	if(event.getCode() == KeyEvent::KEY_UP){
		console() << "Scroll up" << endl;
		if(yOffset > 0){
			yOffset -= 20;
		}
	}
	//Scroll screen down
	if(event.getCode() == KeyEvent::KEY_DOWN){
		console() << "Scroll down" << endl;
		if(yOffset < (TEXTURESIZE - (TEXTURESIZE/zoom))){
			yOffset += 20;
		}
	}
	//Scroll screen right
	if(event.getCode() == KeyEvent::KEY_RIGHT){
		console() << "Scroll right" << endl;
		if(xOffset < (TEXTURESIZE - (TEXTURESIZE/zoom))){
			xOffset += 20;
		}
	}
	//Scroll screen left
	if(event.getCode() == KeyEvent::KEY_LEFT){
		console() << "Scroll left" << endl;
		if(xOffset > 0){
			xOffset -= 20;
		}
	}

	//backspace forces a redraw
	if(event.getCode() == KeyEvent::KEY_BACKSPACE){
		drawNodes = true;
	}

	//The F1 key toggles whether we draw the 2000 census or the 2010 census
	if(event.getCode() == KeyEvent::KEY_F1){
		if(oldDensity){
			oldDensity = false;
		} else {
			oldDensity = true;
		}
	}
}


CINDER_APP_BASIC( HW04App, RendererGl )
