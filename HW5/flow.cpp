/*
Richard Boyle
rmb2189
flow.cpp
Finds the optical flow of two specified images using template matching
*/




#include <iostream>
#include <cstdlib>
#include <fstream>
#include "vision_utilities.h"
using namespace std;

#define tempSize 7
#define windowSize 40

void getTemplate(Image *im, int row, int col, int temp[tempSize][tempSize]){
	temp[3][3]=getPixel(im,row,col);
	int nCols = getNCols(im);
	int nRows = getNRows(im);
	for(int i = 0; i<tempSize; i++){
		for(int j = 0; j<tempSize; j++){
			int newRow = row-((tempSize/2)-i);
			int newCol = col-((tempSize/2)-j);
			if(newRow<0 ||newRow>=nRows || newCol<0||newCol>=nCols)
				temp[i][j] = 0;
			else
				temp[i][j] = getPixel(im,newRow,newCol);
		}
	}
}
void getSearchWindow(Image *im, int row, int col, int search[windowSize][windowSize]){
	int nCols = getNCols(im);
	int nRows = getNRows(im);
	for(int i = 0; i<windowSize; i++){
		for(int j = 0; j<windowSize; j++){
			int newRow = row-((windowSize/2)-i);
			int newCol = col-((windowSize/2)-j);
			if(newRow<0 ||newRow>=nRows || newCol<0||newCol>=nCols)
				search[i][j] = 0;
			else
				search[i][j] = getPixel(im,newRow,newCol);
		}
	}
}

void templateSearch(int temp[tempSize][tempSize], int search[windowSize][windowSize], int newLoc[1][2]){
	int bestX=0;
	int bestY=0;
	float SSD=0;
	float e1=0;
	float e2 = 0;

	float minSSD = 10000000;
	for(int i = 0; i<=windowSize-tempSize;i++){
		for(int j = 0; j<=windowSize-tempSize;j++){
			
			SSD=0;
			e1 = 0;
			e2 = 0;
			for(int m=0; m<tempSize; m++){
				for (int n = 0; n<tempSize; n++){

					int currTemp = temp[m][n];
					int currPixel = search[i+m][j+n];
					SSD+=pow(abs(currTemp - currPixel),2);
					e1+=pow(abs(currTemp),2);
					e2+=pow(abs(currPixel),2);
				}
			}
			SSD /=sqrt(e1*e2);
			if (minSSD > SSD)
			{
				minSSD = SSD;
				bestY = i;
				bestX = j;
			}
		}
		
	}
	newLoc[0][0] = bestX;
	newLoc[0][1] = bestY;
}

int main(int argc, char *argv[]) {
	//check for arguments
	if(argc!=5){
		cout<<"usage: "<< argv[0] <<" <input image 1><input image2><grid><output image>\n";
		return -1;
	}

	//read in images
	Image *im1 = (Image *) malloc(sizeof(Image));
	readImage(im1,argv[1]);
	int nCols = getNCols(im1);
	int nRows = getNRows(im1);

	Image *im2 = (Image *) malloc(sizeof(Image));
	readImage(im2,argv[2]);

	
	int gridN = atoi(argv[3]);
	int im1template[tempSize][tempSize];
	int newLoc[1][2];
	
	Image *output = (Image*)malloc(sizeof(Image));
	setSize(output,nRows,nCols);
	setColors(output,255);
	for(int i = 0; i<nRows;i++){
		for(int j = 0; j<nCols;j++){
			setPixel(output,i,j,0);
		}
	}
	//apply template matching to two images
	int im2searchwindow[windowSize][windowSize];
	for(int i = 0; i <nRows;i++){
		for(int j = 0; j<nCols;j++){
			if(i % gridN == 0 && j % gridN==0 && i<nRows-tempSize && j<nCols-tempSize){
				getTemplate(im1,i,j,im1template);
				getSearchWindow(im2,i,j,im2searchwindow);
				templateSearch(im1template,im2searchwindow,newLoc);
				line(output,i,j,newLoc[0][1]+i-20,newLoc[0][0]+j-20,255);
			}
			
		}
	}
	//write output image with flow
	writeImage(output,argv[4]);
	free(output);
	free(im1);
	free(im2);


}
