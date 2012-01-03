//============================================================================
// Name        : p3.cpp
// Author      : Richard Boyle
// uni		   : rmb2189
// Description : Outputs image database and adds graphical features
//============================================================================

#include <iostream>
#include <fstream>
#include <cmath>
#include "vision_utilities.h"
using namespace std;


//E function
double E(double a, double b, double c,double theta){
	return a*sin(theta)*sin(theta) - b*sin(theta)*cos(theta)+c*cos(theta)*cos(theta);
}


int main(int argc, char *argv[]) {
	
	//check for arguments to be correct
	if(argc!=4){
		cout<<"usage: "<< argv[0] <<" <input labeled image><output database><output image>"<<endl;
		return -1;
	}	
	
	//create output file stream
	ofstream db;

	//open database
	db.open(argv[2]);


	//open image
	const char *name = argv[1];
	//binary image
	Image *im = (Image *) malloc(sizeof(Image));
	readImage(im,name);
	int nCols = getNCols(im);
	int nRows = getNRows(im);

	//for each object number of objects is described by number of colors
	int colorNum = getColors(im);
	for(int color = 1; color <= colorNum; color++){
		//object label
		db<<color <<" ";

		//find position
		double area = 0;
		int x = 0;
		int y = 0;
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
				}
			}
		}
		//x position
		x = x/area;
		//y position
		y = y/area;
		
		
		//set center dot based on x and y position and area
		setPixel(im,y,x,0);
		
		//output to db
		db<<x<<" "<<y<<" ";
		db<<area<<" ";
		
		
		//find a,b,c
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
		
		//find theta1 and theta2
		double theta1 = atan2(b,(a-c))/2;
		double theta2 = theta1 + PI/2;
		double secondD = (a-c)*cos(2*theta1) + b*sin(2*theta1);
		
		
		//determine eMin and eMax based on secondD
		double eMin = 0;
		double eMax = 0;
		double thetaMin = 0;
		double thetaMax = 0;
		if(secondD > 0){
			thetaMin = theta1;
			thetaMax = theta2;
			eMin = E(a,b,c,theta1);
			eMax = E(a,b,c,theta2);
			db<<theta1<<" ";
		}
		else
		{
			thetaMin = theta2;
			thetaMax = theta1;
			eMin = E(a,b,c,theta2);
			eMax = E(a,b,c,theta1);
			db<<theta2<<" ";
		}

		db<<eMin<<" ";

			
		//roundness
		double roundness = eMin/eMax;
		db<<roundness;
		if(color!=colorNum)
			db<<endl;
			
	
		//draw line based on center point and orientation
		line(im,y-50*cos(thetaMin),x-50*sin(thetaMin),y+50*cos(thetaMin),x+50*sin(thetaMin),0);	
	}

	//end for

	//close file
	db.close();
	
	//write final image
	writeImage(im,argv[3]);
	return 0;
}
