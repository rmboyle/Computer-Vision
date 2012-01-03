/*
Richard Boyle
rmb2189
p3.cpp
Finds the mask of the object
*/

#include <iostream>
#include <cmath>
#include <fstream>
#include "vision_utilities.h"

using namespace std;
Image imageStore[5];
int main(int argc, char *argv[]) {
	//check for arguments
	if(argc!=7){
		cout<<"usage: "<< argv[0] <<" <image 1><image 2>";
		cout<<"<image 3><image 4><image 5><output mask>"<<endl;
		return -1;
	}	

	//create output mask
	Image *outputMask = (Image *) malloc(sizeof(Image));
	setSize(outputMask, 480,480);
	setColors(outputMask,1);
	for(int i = 0; i<480;i++){
		for(int j = 0;j<480;j++){
			setPixel(outputMask,i,j,0);
		}
	}
	for(int i = 0; i < 5; i++){
		readImage(&imageStore[i],argv[i+1]);
	}
	
	int nCols = getNCols(&imageStore[0]);
	int nRows = getNRows(&imageStore[0]);
	
	for(int x = 0; x<nCols;x++){
		for(int y = 0; y<nRows; y++){
			int sourceCount=0;
			for(int source = 0; source<5;source++){
				if(getPixel(&imageStore[source],y,x)!=0){
					sourceCount++;
					if(sourceCount==3)
						setPixel(outputMask,y,x,1);
				}
			}
		}
		
	}
	writeImage(outputMask,argv[6]);
	free(outputMask);
}
