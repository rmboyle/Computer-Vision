//============================================================================
// Name        : p1.cpp
// Author      : Richard Boyle
// uni		   : rmb2189
// Description : Converts image to binary based on a threshold value.
//============================================================================

#include <iostream>

#include "vision_utilities.c"
using namespace std;

int main(int argc, char *argv[]) {
	if(argc!=4){
		cout<<"usage: "<< argv[0] <<" <input binaray image><threshold>";
		cout<<"<output binary image>\n";
		return -1;
	}	

	const char *name = argv[1];

	Image *im = (Image *) malloc(sizeof(Image));
	readImage(im,name);
	int nCols = getNCols(im);
	int nRows = getNRows(im);
	int thresh = atoi(argv[2]);

	for(int i = 0;i<nRows;i++){
		for(int j = 0; j<nCols;j++){
			if(getPixel(im,i,j)>thresh)
				setPixel(im,i,j,1);
			else
				setPixel(im,i,j,0);

		}
	}
	setColors(im,1);
	const char *newIm = argv[3];
	writeImage(im,newIm);
	return 0;
}
