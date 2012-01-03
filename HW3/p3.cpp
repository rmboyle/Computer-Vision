/*
Richard Boyle
rmb2189
p3.cpp
Finds the strong lines of the image by scanning through the acummulator array and find the high
votes based on a threshold. 
*/


#include <iostream>
#include <cmath>
#include "vision_utilities.h"
using namespace std;

int main(int argc, char *argv[]) {
	if(argc!=5){
		cout<<"usage: "<< argv[0] <<"<input original><input hough image>";
		cout<<"<hough threshold>";
		cout<<"<line-detected output image>\n";
		return -1;
	}
	//get arguments
	const char *originalIm = argv[1];
	const char *houghIm = argv[2];
	int houghThresh = atoi(argv[3]);
	const char *outputIm = argv[4];

	//read in images
	Image *imOriginal = (Image *) malloc(sizeof(Image));
	readImage(imOriginal,originalIm);
	Image *imHough = (Image *) malloc(sizeof(Image));
	readImage(imHough,houghIm);

	//size of image
	int OnCols = getNCols(imOriginal);
	int OnRows = getNRows(imOriginal);
	int HnCols = getNCols(imHough);
	int HnRows = getNRows(imHough);
	int diagonal = sqrt((OnCols*OnCols)+(OnRows*OnRows));
	

	//look for high votes in accumulator if greater than threshold draw a line
	for(int i = 0; i<HnRows; i++){
		for(int j = 0;j<HnCols;j++){
			if(getPixel(imHough,i,j)>=houghThresh){
				double p = 2*diagonal/HnRows*i - diagonal;
				double theta = M_PI/HnCols*j - M_PI/2;
				double x = p*cos(theta+M_PI/2);
				double y = p*sin(theta+M_PI/2);
				line(imOriginal,y-diagonal*sin(theta),
					x-diagonal*cos(theta),y+diagonal*sin(theta),x+diagonal*cos(theta),255);
			}
		}
	}
	writeImage(imOriginal,outputIm);
	free(imOriginal);
	free(imHough);
	return 0;
}