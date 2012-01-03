/*
Richard Boyle
rmb2189
p1.cpp
Finds the locations of the edge points in the image using the 3x3 sobel
*/

#include <iostream>
#include <cmath>
#include "vision_utilities.h"
using namespace std;




int main(int argc, char *argv[]) {

	//check for arguments
	if(argc!=3){
		cout<<"usage: "<< argv[0] <<" <input image><output image>\n";
		return -1;
	}	
	
	//name of image
	const char *inputName = argv[1];


	//read in image
	Image *im = (Image *) malloc(sizeof(Image));
	readImage(im,inputName);
	int nCols = getNCols(im);
	int nRows = getNRows(im);

	//hard code sobel
	int sobelX [3][3]={{-1,0,1},{-2,0,2},{-1,0,1}};
	int sobelY [3][3]={{1,2,1},{0,0,0},{-1,-2,-1}};
	

	//read image into array
	int imA [nRows][nCols];
	for(int i = 0; i<nRows; i++){
		for(int j = 0; j<nCols; j++){
			imA[i][j] = getPixel(im,i,j);
		}
	}


	int gX, gY;
	int sum;
	int kcenterX = 1;
	int kcenterY = 1;
	for(int i = 0; i< nRows; i++){
		for(int j = 0; j<nCols; j++){
			gX = 0;
			gY = 0;		
			sum = 0;
			
			gX = ((getPixel(im,i+1,j+1) - getPixel(im,i,j+1))+
				(getPixel(im,i+1,j)-getPixel(im,i,j)));
			gY = ((getPixel(im,i+1,j+1) - getPixel(im,i+1,j))+
				(getPixel(im,i,j+1)-getPixel(im,i,j)));
			
			//gradient magnitude
			int S = sqrt((gX*gX) + (gY * gY));
			setPixel(im,i,j,S);
			
		}
	}

	const char *outputName = argv[2];
	
	//write image to new file name
	writeImage(im,outputName);
	free(im);
	return 0;
}
