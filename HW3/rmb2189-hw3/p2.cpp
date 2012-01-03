/*
Richard Boyle
rmb2189
p2.cpp
Thresholds the image to find the strong edges and implements the hough transform
*/


#include <iostream>
#include <cmath>
#include "vision_utilities.h"
using namespace std;




int main(int argc, char *argv[]){
	//check for arguments
	if(argc!=5){
		cout<<"usage: "<< argv[0] <<" <input image><edge threshold>";
		cout<<"<output thesholded image><output hough image>\n";
		return -1;
	}	
	
	//get arguments
	const char *inputName = argv[1];
	int edgeThreshold = atoi(argv[2]);
	const char *outputThreshName = argv[3];
	const char *outputHoughName = argv[4];


	//read image		
	Image *im = (Image *) malloc(sizeof(Image));
	readImage(im,inputName);

	//size of image
	int nCols = getNCols(im);
	int nRows = getNRows(im);


	//THRESHOLD IMAGE
	for(int i = 0; i<nRows; i++){
		for(int j = 0; j<nCols; j++){
			int tempPix = getPixel(im,i,j);
			if(tempPix >= edgeThreshold)
				setPixel(im,i,j,1);
			else
				setPixel(im,i,j,0);
		}
	}
	//write thresholded image
	setColors(im,1);
	writeImage(im,outputThreshName);

	//HOUGH TRANSFORM
	//What are the ranges of possible θ values and possible ρ values (be careful here)? 
	//You can use these constraints to limit the size of the accumulator array. 

	// = 0;
	
	//theta range is -Pi/2 -> Pi/2
	//p range is 0 -> Image Diagonal (255?)
	//theta crap
	int ntheta = 800;
	int pMax = sqrt(nCols*nCols + nRows*nRows);
	//create accumulator array
	int acum [pMax][ntheta];
	//set A(m,c) = 0 for all (m,c)
	for(int i = 0; i<pMax;i++){
		for(int j = 0; j< ntheta; j++){
			acum[i][j] = 0;
		}
	}

	//iterate through thresholded image and vote on lines
	for(int i = 0; i<nRows;i++){
		for(int j = 0; j<nCols; j++){
			if(getPixel(im,i,j)){
				for(int thetaIndex = 0; thetaIndex<ntheta;++thetaIndex){
				double theta = (M_PI/ntheta)*thetaIndex - M_PI/2;
				double p = i*cos(theta)-j*sin(theta);
				int pIndex = (.5)*(p+pMax);
				acum[pIndex][thetaIndex]++;
			}
			}
		}
	}



	//write acumulator array to hough image
	Image *houghIm = (Image *) malloc(sizeof(Image));
	setSize(houghIm,  pMax,ntheta);
	setColors(houghIm,255);
	for(int i = 0; i< ntheta; i++){
		for(int j = 0; j<pMax;j++){
				if(acum[j][i]>255)
				setPixel(houghIm,j,i,255);
				else	
				setPixel(houghIm,j,i,acum[j][i]);
		}
	}


	//write hough image to new file name
	writeImage(houghIm,outputHoughName);
	free(houghIm);
	free(im);
	return 0;
}
