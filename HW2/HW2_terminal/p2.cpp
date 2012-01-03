//============================================================================
// Name        : p2.cpp
// Author      : Richard Boyle
// uni		   : rmb2189
// Description : Labels image
//============================================================================

#include <iostream>
#include "vision_utilities.h"
using namespace std;



#define MAXX 1000 


int unionA[MAXX],unionrank[MAXX];
int tempPix[MAXX][MAXX];

int colorLib[1000];

int colorCount = 0;


void setTempPixel(int i, int j, int label){
	tempPix[i][j]=label;
}
int getTempPixel(int i,int j){
	return tempPix[i][j];
}


void make_set(int x) {
	unionA[x] = x;
	unionrank[x] = 0;
}

void link(int x,int y) {
	if (unionrank[x] > unionrank[y])
		unionA[y] = x;
	else {
		unionA[x] = y;
		if (unionrank[x] == unionrank[y])
			unionrank[y] = unionrank[y] + 1;
	}
}

int find_set(int x) {
	if (x != unionA[x])
		unionA[x] = find_set(unionA[x]);
	return unionA[x];
}

void union_set(int x,int y) {
	link(find_set(x),find_set(y));
}


int find_pixel(int set){
	if(colorLib[set]!=0){
		return colorLib[set];
	}
	else{

		colorCount++;
		colorLib[set] = colorCount;

		return colorCount;
	}
}


void setPixels(Image *im){
	int nCols = getNCols(im);
	int nRows = getNRows(im);
	for(int i = 0;i<nRows;i++){
		for(int j = 0; j<nCols;j++){
			if(tempPix[i][j]!=0){
			int color = find_pixel(find_set(tempPix[i][j]));
			//cout<<color<<endl;
			setPixel(im,i,j,color);
			}
			else
				setPixel(im,i,j,0);
		}
	}
	setColors(im,colorCount);
}


int main(int argc, char *argv[]) {
	
	if(argc!=3){
		cout<<"usage: "<< argv[0] <<" <input binary image><output labeled image>"<<endl;
		return -1;
	}	
	

	const char *name = argv[1];
	
	Image *im = (Image *) malloc(sizeof(Image));
	readImage(im,name);
	int nCols = getNCols(im);
	int nRows = getNRows(im);
	for(int i = 0; i<nRows; i++){
		for(int j = 0; j<nCols; j++){
			setTempPixel(i,j,getPixel(im,i,j));
		}
	}
	
	//apply connected component alg
	int currLabel = 0;
	int finalLabel = 0;
	for(int i = 0;i<nRows;i++){
		for(int j = 0; j<nCols;j++){
			//if not background
			int currPix = getTempPixel(i,j);
			int nPix;
			int wPix;
			int nwPix;
			if(i!=0)
				nPix = getTempPixel(i-1,j);
			else
				nPix = -1;
			if(j!=0)
				wPix = getTempPixel(i,j-1);
			else
				wPix = -1;
			if(i!=0 && j!=0)
				nwPix = getTempPixel(i-1,j-1);
			else
				nwPix = -1;
			if(currPix!=0){

				if(nPix==-1 && nwPix ==-1 && wPix ==-1){
				//	setPixel(im,i,j,++currLabel);
					setTempPixel(i,j,++currLabel);
				//	cout<<"setting temp: " <<currLabel<<endl;
					make_set(currLabel);
				}
				else if(nPix==0 && wPix==0 && nwPix ==0){
				//	setPixel(im,i,j,++currLabel);
					setTempPixel(i,j,++currLabel);
				//	cout<<"setting temp: " <<currLabel<<endl;
					make_set(currLabel);
				}
				else if(nwPix !=0 && nwPix!=-1){
					setTempPixel(i,j,nwPix);

					}
				else if(nwPix==0 && nPix==0){
					setTempPixel(i,j,wPix);

				}
				else if(nwPix==0 && wPix==0)

				{
					setTempPixel(i,j,nPix);
					}
				else if(nwPix==0){
					setTempPixel(i,j,nPix);
					union_set(nPix,wPix);
				}
			}
			else
				setTempPixel(i,j,0);
		}

	}


	setPixels(im);
	writeImage(im,argv[2]);

	return 0;
}



