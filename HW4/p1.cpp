/*
Richard Boyle
rmb2189
p1.cpp
Locates sphere in image and computes its center and radius.
*/

#include <iostream>
#include <cmath>
#include <fstream>
#include "vision_utilities.h"
using namespace std;




int main(int argc, char *argv[]) {

	//check for arguments
	if(argc!=3){
		cout<<"usage: "<< argv[0] <<" <input image><output parameters file>\n";
		return -1;
	}	
	
	//name of image
	const char *inputName = argv[1];

	//name of output file
	ofstream paramFile;
  	paramFile.open (argv[2]);
  	

	//read in image
	Image *im = (Image *) malloc(sizeof(Image));
	readImage(im,inputName);
	int nCols = getNCols(im);
	int nRows = getNRows(im);
	int area = 0;
	int y = 0;
	int x = 0;
	
	for(int i = 0; i < nRows; i++){
		for(int j = 0; j<nCols;j++){
			int currPix = getPixel(im,i,j);
			if(currPix !=0){
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
	//computer radius
	float r = sqrt(area/M_PI);

	paramFile<<x<<" "<<y<<" "<<r<<endl;
	paramFile.close();

	free(im);
	return 0;

}
