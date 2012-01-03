/*
Richard Boyle
rmb2189
p4.cpp
Computes the surface normals and albedo of object
*/

#include <iostream>

#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>
#include "vision_utilities.h"


using namespace std;


Image imageStore[5];

//maps x y z value to a valid r g or b value
int map_rgb(float value){
	return 255*(1+value)/2;
}
//determinate function
float det(float m[3][3]){
	float	det;
	det = m[0][0] * ( (m[1][1] * m[2][2]) - (m[2][1] * m[1][2]) );
	det = det - m[0][1] * ( (m[1][0] * m[2][2]) - (m[2][0] * m[1][2]) );
	det = det + m[0][2] * ( (m[1][0] * m[2][1]) - (m[2][0] * m[1][1]) );
	return det;
}
//transpose function
void m_transpose(float S[3][3], float S_T[3][3], int size){
	for(int i = 0;i<size;i++){
		for(int j = 0; j<size;j++){
			S_T[i][j] = S[j][i];
		}
	}
}

void m_transpose2(float **S, float **S_T, int n, int m){
	for(int i = 0; i < n;i++){
		for(int j = 0; j<m; j++){
			S_T[i][j] = S[j][i];
		}
	}
}
//matrix multiply 
void m_multiply(float m[3][3],float value, int size){
	for(int i = 0; i < size; i++){
		for(int j = 0; j <size;j++){
			m[i][j] *=value;
		}
	}
}
//inverse function
void m_inverse(float S[3][3],float S_I[3][3]){
	

	float a = S[0][0];
	float b = S[0][1];
	float c = S[0][2];
	float d = S[1][0];
	float e = S[1][1];
	float f = S[1][2];
	float g = S[2][0];
	float h = S[2][1];
	float k = S[2][2];
	float A = (e*k)-(f*h);
	float B = (f*g)-(d*k);
	float C = (d*h)-(e*g);
	float D = (c*h)-(b*k);
	float E = (a*k)-(c*g);
	float F = (g*b)-(a*h);
	float G = (b*f)-(c*e);
	float H = (c*d)-(a*f);
	float K = (a*e)-(b*d);
	S_I[0][0] = A;
	S_I[0][1] = D;
	S_I[0][2] = G;
	S_I[1][0] = B;
	S_I[1][1] = E;
	S_I[1][2] = H;
	S_I[2][0] = C;
	S_I[2][1] = F;
	S_I[2][2] = K;


	m_multiply(S_I,1/det(S),3);

}



//matrix matrix multiply
void m_m(float S[3][3], float I[3], float M[3]){
	for(int i = 0; i < 3; i++){
		float temp=0;
		for(int j = 0; j<3;j++){
			temp+=S[i][j]*I[j]; 
		}
		M[i] = temp;
	}
}



int main(int argc, char *argv[]) {
	//check for arguments
	if(argc!=11){
		cout<<"usage: "<< argv[0] <<" <input directions><image 1><image 2>";
		cout<<"<image 3><image 4><image 5><mask><normal map><albedo map><output gradient"<<endl;
		return -1;
	}
	

	//read in directions and images
	ifstream directionsFile(argv[1]);
	float direct[5][3];

	for(int i = 0; i < 5;i++){
		//read in images 1 -> 5
		readImage(&imageStore[i],argv[i+2]);
		for(int j = 0;j<3;j++){
			directionsFile>>direct[i][j];
		}
			
	}
	directionsFile.close();
	
	int nCols = getNCols(&imageStore[0]);
	int nRows = getNRows(&imageStore[0]);
	float maxP= 0; 
	float maxQ= 0;
	//read in mask
	Image *mask = (Image*)malloc(sizeof(Image));
	readImage(mask,argv[7]);

	//create normal map
	ImageColor *normalMap = (ImageColor*)malloc(sizeof(ImageColor));
	setSizeColor(normalMap,nCols,nRows);
	setColorsColor(normalMap,255);

	//create albedo map
	Image *albedoMap = (Image*)malloc(sizeof(Image));
	setSize(albedoMap,nCols,nRows);
	setColors(albedoMap,255);

	float bright[3][2];
	float S[3][3],*I,S_I[3][3],*M;
	float A[nRows][nCols];
	float albedoMax = 0;
	Gradient gradients[nRows][nCols];
	//for each point in the image
	for(int i = 0; i<nRows; i++){
		for(int j = 0; j<nCols; j++){
			if(getPixel(mask,i,j)){
				//go thru each pixel of each image find 3 brightest
				bright[0][0] = -1; bright[1][0]=-1; bright[2][0]=-1;
				bright[0][1] = -1; bright[1][1]=-1; bright[2][1]=-1;
				
				for(int source = 0; source<5;source++){
					int tempPix = getPixel(&imageStore[source],i,j);
				//	cout<<endl<<endl<<"pixval";
					//cout<<tempPix<<endl;
					if(bright[0][1]==-1){
						bright[0][0] = tempPix;
						bright[0][1] = source;
						continue;
					}
					else if(bright[1][1]==-1){
						bright[1][0] = tempPix;
						bright[1][1] = source;
						continue;
					}
					else if(bright[2][1]==-1){
						bright[2][0] = tempPix;
						bright[2][1] = source;
						continue;
					}


					if(bright[0][0]<tempPix){
						int movePix1 = bright[0][0];
						int moveSource1 = bright[0][1];
						bright[0][0] = tempPix;
						bright[0][1] = source;

						if(movePix1 > bright[1][0]){
							int movePix2 = bright[1][0];
							bright[1][0] = movePix1;
							int moveSource2 = bright[1][1];
							bright[1][1] = moveSource1;
							if(movePix2>bright[2][0]){
								bright[2][0] = movePix2;
								bright[2][1] = moveSource2;
							}
						}
						else if(movePix1>bright[2][0]){
							bright[2][0] = movePix1;
							bright[2][1] = moveSource1;
						}
						
					}
					else if (bright[1][0]<tempPix){
						int movePix = bright[1][0];
						bright[1][0] = tempPix;
						int moveSource = bright[1][1];
						bright[1][1] = source;
						if(movePix>bright[2][0]){
							bright[2][0] = movePix;
							bright[2][1] = moveSource;
						}
					}
					else if(bright[2][0]<tempPix){
						bright[2][0] = tempPix;
						bright[2][1] = source;
					}


				}

				for(int m = 0; m<3; m++){
					if(bright[m][0]==0){
						 
					}
				}

				//create intesity matrix, with the 3 brightest found above
				I = (float *)malloc(3*sizeof(float));
				
				for(int m = 0; m<3;m++){
					int source = bright[m][1];
					I[m] = bright[m][0];
					for(int n = 0; n<3;n++){
						S[m][n] = direct[source][n];
					}
				}

				//compute inverse of source directions
				m_inverse(S,S_I);
				//multiply inverse and intensities store in M
				M = (float *)malloc(3*sizeof(float));
				m_m(S_I,I,M);
				//find the magnitude of M
				float mag = sqrt(pow(M[0],2)+pow(M[1],2)+pow(M[2],2));
			//	cout<<mag<<endl;
				A[i][j] = mag;

				if(mag>albedoMax){
					albedoMax = mag;
				}

				//normalize
			
				float x = M[0]/mag;
				float y = M[1]/mag;
				float z = M[2]/mag;
				//map to rgb values	
				int xMap = map_rgb(x);
				int yMap = map_rgb(y);
				int zMap = map_rgb(z);

				setPixelColor(normalMap,i,j,xMap,yMap,zMap);

				//z scale 
				float zScale = 1/M[2];

				//surface gradients for each pixel
				float p = x/zScale;
				float q = y/zScale;

				//store in gradient data struct

                gradients[i][j].p = p;
                gradients[i][j].q = q;
                if(mag>1.0)
                	maxP++;
			}
			else
				setPixelColor(normalMap,i,j,0,0,0);
		}
	}
	writeImageColor(normalMap,argv[8]);
	free(normalMap);

	//write gradient binary datafile
	ofstream gradFile(argv[10], ios::out | ios::binary);
	if(!gradFile){
		cerr<<"Cannot open"<<argv[10]<<endl;
		exit(1);
	}
	gradFile.write((char*)gradients,nCols*nRows*sizeof(Gradient));
	gradFile.close();

	//create albedo map
	for(int i = 0 ; i <nRows; i++){
		for(int j = 0; j<nCols;j++){
			int map = 255*A[i][j]/albedoMax;
			setPixel(albedoMap,i,j,map);
		}
	}
	
	writeImage(albedoMap,argv[9]);
	free(albedoMap);
}
