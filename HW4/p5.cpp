/*
Richard Boyle
rmb2189
p5.cpp
Creates a depth map of image
*/


#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <sstream>
#include "vision_utilities.h"

using namespace std;

#define seedNum 5

int main(int argc, char *argv[]) {
	if(argc!=5){
		cout<<"usage: "<< argv[0] <<" <input gradient file><seed point file><binary mask>";
		cout<<"<output depth map>"<<endl;
		return -1;
	}
	//read in mask
	Image *mask = (Image*)malloc(sizeof(Image));
	readImage(mask,argv[3]);
	int nCols = getNCols(mask);
	int nRows = getNRows(mask);

	//create depth map file
	Image *depthMap = (Image*)malloc(sizeof(Image));
	setSize(depthMap,nCols,nRows);
	setColors(depthMap,255);

	//read in gradients
	Gradient gradients[nCols][nRows];
	ifstream gradFile(argv[1], ios::in | ios::binary);
	if(!gradFile){
		cerr<<"Cannot open"<<argv[1]<<endl;
		exit(1);
	}
	gradFile.read((char*)gradients,nCols*nRows*sizeof(Gradient));
	gradFile.close();
	
	//read in seeds and store in array
	int seedsXY[seedNum][2];
  	ifstream seedFile;
  	seedFile.open(argv[2]);

  	for(int i = 0; i<seedNum; i++){
      	seedFile>>seedsXY[i][0];
      	seedFile>>seedsXY[i][1];
    }
	

	//depth array that will do it for each seed point
	float depth[seedNum][nRows][nCols];
	for(int i = 0; i<nRows; i++){
		for(int j =0; j<nCols;j++){
			for(int seed = 0; seed<seedNum;seed++){
				depth[seed][i][j] = 0;
			}
		}
	}
	
	for(int seed = 0; seed<seedNum; seed++){
		//initialize reference depth
		int xSeed = seedsXY[seed][0];
		int ySeed = seedsXY[seed][1];
		depth[seed][ySeed][xSeed] = 0;

		


		//compute cross
		//north
		for(int i = ySeed; i>=0;i--){
			depth[seed][i][xSeed] = (depth[seed][i+1][xSeed]-gradients[i][xSeed].q);

		}
		//south
		for(int i = ySeed; i<nRows;i++){
			depth[seed][i][xSeed] = (depth[seed][i-1][xSeed]-gradients[i][xSeed].q);

		}
		//west
		for(int j = xSeed; j>=0;j--){
			depth[seed][ySeed][j] = (depth[seed][ySeed][j+1]-gradients[ySeed][j].p);

		}
		//east
		for(int j = xSeed; j<nCols;j++){
			depth[seed][ySeed][j] = (depth[seed][ySeed][j-1]-gradients[ySeed][j].p);

		}

		
		//quadrant 1
		for(int i = ySeed; i>=0;i--){
			for(int j = xSeed;j<nCols;j++){
				if(getPixel(mask,i,j)){
					depth[seed][i][j]=0.5*(depth[seed][i][j-1]+gradients[i][j].p) + 0.5*(depth[seed][i+1][j]-gradients[i+1][j].q);

				}
				else
					depth[seed][i][j] = 0;
			}
		}
		//quadrant 2
		for(int j = xSeed; j<nCols;j++){
			for(int i = ySeed;i<nRows;i++){
				if(getPixel(mask,i,j)){
					depth[seed][i][j]=0.5*(depth[seed][i][j-1]+gradients[i][j].p) + 0.5*(depth[seed][i-1][j]+gradients[i][j].q);

				}
				else
					depth[seed][i][j] = 0;
			}
		}
		//quadrant 3
		for(int j = xSeed; j>=0;j--){
			for(int i = ySeed;i<nRows;i++){
				if(getPixel(mask,i,j)){
					depth[seed][i][j]=0.5*(depth[seed][i][j+1]-gradients[i][j+1].p) + 0.5*(depth[seed][i-1][j]+gradients[i][j].q);

				}
				else
					depth[seed][i][j] = 0;
			}
		}
		//quadrant 4
		for(int i = ySeed; i>=0;i--){
			for(int j = xSeed; j>=0;j--){
				if(getPixel(mask,i,j)){
					depth[seed][i][j]=0.5*(depth[seed][i][j+1]-gradients[i][j+1].p) + 0.5*(depth[seed][i+1][j]-gradients[i+1][j].q);

				}
				else
					depth[seed][i][j] = 0;
			}
		}
	}
	
	double max = 0;
	
	//avg the depth of all the seeds

	double depthAvg[nRows][nCols];
	for(int i = 0; i<nRows;i++){
		for(int j=0; j<nCols;j++){
			for(int seed = 0; seed<seedNum;seed++){
				depthAvg[i][j]+=depth[seed][i][j];

			}
			depthAvg[i][j]/=seedNum;
			if(depthAvg[i][j] > max)
				max = depthAvg[i][j];
			
		}
	}
	//write to depth map
	for(int i = 0; i<nRows;i++){
		for(int j=0; j<nCols;j++){
			setPixel(depthMap,i,j,255*depthAvg[i][j]/max);
		}
	}
	
	writeImage(depthMap,argv[4]);
	free(depthMap);
	seedFile.close();
}
