/*
Richard Boyle
rmb2189
p2.cpp
Computes the directions and intensities of the light sources
*/

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <sstream>
#include "vision_utilities.h"
using namespace std;




int main(int argc, char *argv[]) {

	//check for arguments
	if(argc!=8){
		cout<<"usage: "<< argv[0] <<" <input parameters file><image 1><image 2>";
		cout<<"<image 3><image 4><image 5><output directions file>"<<endl;
		return -1;
	}	
	
	
  	//gets data from parameter file from p1.cpp
  	ifstream paramFile(argv[1]);
	int xCenter;
	paramFile>>xCenter;
	int yCenter;
	paramFile>>yCenter;
	int r;
	paramFile>>r;
	paramFile.close();

	//creates a directions file
	ofstream directionsFile;
	directionsFile.open(argv[7]);
	for(int i = 0; i < 5; i++){
		//name of image
		const char *inputName = argv[i+2];
		//read in image
		Image *im = (Image *) malloc(sizeof(Image));
		readImage(im,inputName);
		int nCols = getNCols(im);
		int nRows = getNRows(im);


		//for each point on the sphere find the brightest point
		float xMax = 0; 
		float yMax = 0;
		float maxVal = 0;
		for(int m = 0; m<nRows;m++){
			for(int n = 0; n<nCols;n++){
				int currPix = getPixel(im,m,n);
				if(currPix > maxVal){
					maxVal = currPix;
					xMax = n;
					yMax = m;
				}			
			}
		}

		//adjust the x,y values to center of coordinate system xcenter,ycenter
		xMax =xMax - xCenter;
		yMax =yMax - yCenter;

		//compute z based on x,y and r
		float z = sqrt(pow(r,2) - pow(xMax,2) - pow(yMax,2));

		//scale the coordinates to maxVal
		float scale = (float)maxVal/r;
		//ouput to directions file
		directionsFile<<xMax*scale<<" "<<yMax*scale<<" "<<z*scale<<endl;
		free(im);
	}

	directionsFile.close();
	return 0;

}
