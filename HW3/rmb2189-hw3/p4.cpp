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

int main(int argc, char *argv[]) 


{

	if(argc!=6){
		cout<<"usage: "<< argv[0] <<"<input original><input hough image>";
		cout<<"<input thresholded edge image>";
		cout<<"<hough threshold>";
		cout<<"<cropped line-detected output image>"<<endl;
		return -1;
	}
	//get arguments
	const char *originalIm = argv[1];
	const char *houghIm = argv[2];
	const char *threshIm = argv[3];
	int houghThresh = atoi(argv[4]);
	const char *outputIm = argv[5];

	//read in images
	Image *imOriginal = (Image *) malloc(sizeof(Image));
	readImage(imOriginal,originalIm);

	//size of image
	int OnCols = getNCols(imOriginal);
	int OnRows = getNRows(imOriginal);

	Image *imHough = (Image *) malloc(sizeof(Image));
	readImage(imHough,houghIm);

	int HnCols = getNCols(imHough);
	int HnRows = getNRows(imHough);
	int diagonal = sqrt((OnCols*OnCols)+(OnRows*OnRows));


	Image *imThresh = (Image *) malloc(sizeof(Image));
	readImage(imThresh,threshIm);
	//scan through the hough transform if a pixel is greater the hough threshold
	//find x, y values for it.
	for(int i = 0; i< HnRows;i++){
		for(int j = 0; j<HnCols;j++){
			if(getPixel(imHough,i,j)>=houghThresh){
				double p = ((2*diagonal)/HnRows)*i-diagonal;
				double theta = (M_PI/HnCols)*j-M_PI/2;
				double x = -p*sin(theta);
				double y = p*cos(theta);
				double xdiag = x-diagonal*cos(theta);
				double ydiag = y-diagonal*sin(theta);
				

				//moves it until it is with in range of the image
				while (xdiag<0 || ydiag<0 || xdiag>=OnCols || ydiag>=OnRows) {
					xdiag += cos(theta);
					ydiag += sin(theta);
				}
				while (xdiag>=0 && ydiag>=0 && xdiag<OnCols && ydiag<OnRows) {
					if (getPixel(imThresh, ydiag, xdiag))
						setPixel(imOriginal, ydiag, xdiag, 255);
					//increment
					xdiag += cos(theta);
					ydiag += sin(theta);
				}
			}
		}
	}
	writeImage(imOriginal,outputIm);
	free(imOriginal);
	free(imHough);
	free(imThresh);
}
