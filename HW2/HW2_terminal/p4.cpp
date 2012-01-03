//============================================================================
// Name        : p4.cpp
// Author      : Richard Boyle
// uni		   : rmb2189
// Description : Compares images
//============================================================================


#include <iostream>
#include <fstream>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>

#include "vision_utilities.h"
using namespace std;

#define ROUNDTHRESH .1
#define AREATHRESH 250
#define EMINTHRESH 490000

double E(double a, double b, double c,double theta){
	return a*sin(theta)*sin(theta) - b*sin(theta)*cos(theta)+c*cos(theta)*cos(theta);
}


int dbSearch(const char *file, double area, double roundness, double eMin){
	string line;
	ifstream db;
	db.open(file);
	while(db.good()){	
    	double tempLabel = 0;
	    double tempX = 0;
    	double tempY = 0;
	    double tempArea = 0;
    	double tempTheta = 0;
	    double tempEMin = 0;
    	double tempRound = 0;
    	getline (db,line);
    	istringstream lineRead (line);

    	lineRead >> tempLabel >>tempX>>tempY>>tempArea>>tempTheta>>tempEMin>>tempRound;
    	
		if(abs(tempArea - area)<AREATHRESH &&  abs(tempRound - roundness) < ROUNDTHRESH
				&& abs(tempEMin -eMin)<EMINTHRESH){
			db.close();
    		return 1;
    	}
	}


    db.close();
	return -1;
}



int main(int argc, char *argv[]) {

if(argc!=4){
	cout<<"usage: "<< argv[0] <<" <input labeled image><input database><output image>"<<endl;
	return -1;
}	

	const char *name = argv[1];
	//binary image
	Image *im = (Image *) malloc(sizeof(Image));
	readImage(im,name);
	int nCols = getNCols(im);
	int nRows = getNRows(im);
	//open database
	const char *dbName = argv[2];

	
	//for each object in input image search through db
	//for each object number of objects is described by number of colors
	int colorNum = getColors(im);
	for(int color = 1; color <= colorNum; color++){

		double area = 0;
		double x = 0;
		double y = 0;
		double a1 = 0;
		double b1 = 0;
		double c1= 0;
		//find area
		for(int i = 0; i < nRows; i++){
			for(int j = 0; j<nCols;j++){
				int currPix = getPixel(im,i,j);
				if(currPix == color){
					area++;
					y = y + i;
					x  = x + j;
					a1 = a1 + i*i;
					b1 = b1+i*j;
					c1 = j*j + c1;
				}
			}
		}
		b1 = 2*b1;
		//x position
		x = x/area;
		//y position
		y = y/area;
		//set dot
	

		double a = 0;
		double b = 0;
		double c = 0;
		
		for(int i = 0; i<nRows;i++){
			for(int j = 0; j<nCols;j++){
			int currPix = getPixel(im,i,j);
				if(currPix == color){
				a += (i - y)*(i - y);
				c += (j - x)*(j - x);
				b += (i - y)*(j - x);
				}
			}
		}
		
		
		double theta1 = atan2(b,(a-c))/2;
		double theta2 = theta1 + PI/2;
	
		double secondD = (a-c)*cos(2*theta1) + b*sin(2*theta1);
		
		
		double eMin = 0;
		double eMax = 0;
		double thetaMin = 0;
		double thetaMax = 0;
		if(secondD > 0){
			thetaMin = theta1;
			thetaMax = theta2;
			eMin = E(a,b,c,theta1);
			eMax = E(a,b,c,theta2);

		}
		else
		{
			thetaMin = theta2;
			thetaMax = theta1;
			eMin = E(a,b,c,theta2);
			eMax = E(a,b,c,theta1);

		}


		//roundness
		double roundness = eMin/eMax;
		
		if(dbSearch(dbName,area,roundness,eMin)!=-1)
		{
		//if in database
		//draw line and set center dot
		setPixel(im,y,x,0);
		line(im,y-50*cos(thetaMin),x-50*sin(thetaMin),y+50*cos(thetaMin),x+50*sin(thetaMin),0);	
		}
	}
	
	
	
	
	
	
	//write final image
	writeImage(im,argv[3]);
	return 0;
}
