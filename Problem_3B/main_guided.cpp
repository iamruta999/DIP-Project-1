//NAME: Amruta Kulkarni
//USC ID: 6914970827
//USC Email: arkulkar@usc.edu
//Submission date: 20 Sept 2015

//Problem 3B
//This code performs guided filtering on the noisy pepper image corrupted with salt & pepper as well as gaussian noise
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <algorithm>

using namespace std;

int main(int argc,char *argv[])
{
    
    /* Define file pointer and variable */
    FILE *file;
    int bytesPerPixel = 3;
    int width = 512;
    int height = 512;
    
    /* Allocate image data array */
    unsigned char noisyImageData[height][width][bytesPerPixel];
    
    /* Read image */
    if (!(file=fopen(argv[1],"rb")))
    {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	fread(noisyImageData, sizeof(unsigned char), height*width*bytesPerPixel, file);
	fclose(file);
    
    unsigned char guidedFilterImage[height][width][bytesPerPixel];
    
    /* Separate R,G,B matrices */
    
    int red[height][width];
    int green[height][width];
    int blue[height][width];
    
    for ( int row = 0; row < height; row++)
    {
        for (int column = 0; column < width; column++)
        {
            red[row][column] = noisyImageData[row][column][0];
            green[row][column] = noisyImageData[row][column][1];
            blue[row][column] = noisyImageData[row][column][2];
        }
    }
    
    // Create an extended array
    int N = 3;
    int extendedWidth = width + (N-1);
    int extendedHeight = height + (N-1);
    int halfLength = (N-1)/2;
    
    int extended[extendedHeight][extendedWidth];
    
    //FOR RED
    
    //central, top and bottom part
    for(int row = 0; row < height; row++)
    {
        for(int column = 0;column < width; column++)
            
        {
            
            extended[row+halfLength][column+halfLength] = red[row][column];//copy the red data in the central part of the extended image
            
            int countTopR = 0;                                                 //Top part of extended Red
            for ( int emptyRow = 0; emptyRow < halfLength; emptyRow++)
            {
                extended[emptyRow][column+halfLength] = red[halfLength-countTopR][column];
                countTopR = countTopR + 1;
            }
            
            int countBottomR = 0;                                              //Bottom part of extended Red
            for ( int emptyrow = extendedHeight-1; emptyrow > ((height-1)+halfLength); emptyrow--)
            {
                extended[emptyrow][column+halfLength] = red[(height-1)-halfLength+countBottomR][column];
                countBottomR = countBottomR + 1;
            }
        }
    }
    
    //left and right part
    for (int row = 0; row < extendedHeight; row++)
    {
        int countLeftR = 0;                                                   //Left columns of extended Red
        for (int emptyColumn = 0; emptyColumn < halfLength; emptyColumn++)
        {
            extended[row][emptyColumn] = extended[row][(N-1)-countLeftR];
            countLeftR = countLeftR + 1;
        }
        
        int countRightR = 0;                                                 //Right columns of extended Red
        for (int emptyColumn = extendedWidth-1; emptyColumn > ((width-1)+halfLength); emptyColumn--)
        {
            extended[row][emptyColumn] = extended[row][(extendedWidth-1)-N+countRightR];
            countRightR = countRightR + 1;
        }
    }
    // Getting mean and variance for every window
    
    
    int I[N][N];
    int IbyI[N][N];
    float meanI, meanP, correlationI, correlationIP;
    float VarianceI, covarianceIP;
    float a,b;
    float epsilon = 100000;
    float aMap[extendedHeight][extendedWidth];
    float bMap[extendedHeight][extendedWidth];
    int sumI;     // For fmeanI
    int sumP;     // For fmeanP
    int sumIbyI;  // For fmean(I*I)
    int sumIbyP;  // For fmean(I*P)
    
    
    for ( int row = 0; row < extendedHeight; row ++)
    {
        for ( int column = 0; column < extendedWidth; column++)
        {
            for ( int i = 0; i < N; i++)
            {
                for ( int j = 0; j < N; j++)
                {
                    I[i][j] = extended[row + (i - halfLength)][column + (j - halfLength)];//I and P are the same
                }
            }
            
            
            for ( int i = 0; i < N; i++)
            {
                for ( int j = 0; j < N; j++)
                {
                    IbyI[i][j] = 0; // To avoid garbage values
                }
            }
            
            for ( int i = 0; i < N; i++)
            {
                for ( int j = 0; j < N; j++)
                {
                    for ( int inside = 0; inside < N; inside ++)
                    {
                        IbyI[i][j] = IbyI[i][j] + (I[i][inside] * I[inside][j]);
                    }
                }
            }
            sumI = 0;     // For fmeanI
            sumP = 0;     // For fmeanP
            sumIbyI = 0;  // For fmean(I*I)
            sumIbyP = 0;  // For fmean(I*P)
            
            for ( int i = 0; i < N; i++)
            {
                for ( int j = 0; j < N; j++)
                {
                    sumI = sumI + I[i][j];
                    sumP = sumP + I[i][j];
                    sumIbyI = sumIbyI + IbyI[i][j];
                    sumIbyP = sumIbyI;
                }
            }
            
            // Get all the mean and correlation values
            meanI = sumI / (N*N);
            meanP = sumP / (N*N);
            correlationI = sumIbyI / (N*N);
            correlationIP = sumIbyP / (N*N);
            // Get the variance and covariance
            VarianceI = correlationI - (meanI * meanI);
            covarianceIP = correlationIP - (meanI * meanP);
            // Get a and b
            a = covarianceIP / (VarianceI + epsilon);
            b = meanP - (a * meanI);
            // Store a and b of every window into aMap and bMap
            aMap[row][column] = a;
            bMap[row][column] = b;
        }
    }
    float aWindow[N][N];
    float bWindow[N][N];
    float aMean, bMean;
    
    for ( int row = 0; row < extendedHeight; row++)
    {
        for ( int column = 0; column < extendedWidth; column++)
        {
            // Get a-window and b-window from aMap and bMap
            for ( int i = 0; i < N; i++)
            {
                for ( int j = 0; j < N; j++)
                {
                    aWindow[i][j] = aMap[row + (i - halfLength)][column + (j - halfLength)];
                    bWindow[i][j] = bMap[row + (i - halfLength)][column + (j - halfLength)];
                }
            }
            
            float sumA = 0; // For fmean of a
            float sumB = 0; // For fmean of b
            // Get a window and b window from aMap and bMap
            for ( int i = 0; i < N; i++)
            {
                for ( int j = 0; j < N; j++)
                {
                    sumA = sumA + aWindow[i][j];
                    sumB = sumB + bWindow[i][j];
                }
            }
            // Now get the mean of a and b
            aMean = sumA / (N*N);
            bMean = sumB / (N*N);
            // Get the q of current pixel
            extended[row][column] = (aMean * extended[row][column]) + bMean;
            
        }
    }
    
    for ( int row = 1; row < extendedHeight - 1; row++)
    {
        for ( int column = 1; column < extendedWidth - 1; column ++)
        {
            guidedFilterImage[row-1][column-1][0] = extended[row][column]; //Store Red
        }
    }
    
    //FOR GREEN
    
    //central, top and bottom part
    for(int row = 0; row < height; row++)
    {
        for(int column = 0;column < width; column++)
            
        {
            
            extended[row+halfLength][column+halfLength] = green[row][column];//copy the red data in the central part of the extended image
            
            int countTopG = 0;                                                 //Top part of extended green
            for ( int emptyRow = 0; emptyRow < halfLength; emptyRow++)
            {
                extended[emptyRow][column+halfLength] = green[halfLength-countTopG][column];
                countTopG = countTopG + 1;
            }
            
            int countBottomG = 0;                                              //Bottom part of extended green
            for ( int emptyrow = extendedHeight-1; emptyrow > ((height-1)+halfLength); emptyrow--)
            {
                extended[emptyrow][column+halfLength] = green[(height-1)-halfLength+countBottomG][column];
                countBottomG = countBottomG + 1;
            }
        }
    }
    
    //left and right part
    for (int row = 0; row < extendedHeight; row++)
    {
        int countLeftG = 0;                                                   //Left columns of extended green
        for (int emptyColumn = 0; emptyColumn < halfLength; emptyColumn++)
        {
            extended[row][emptyColumn] = extended[row][(N-1)-countLeftG];
            countLeftG = countLeftG + 1;
        }
        
        int countRightG = 0;                                                 //Right columns of extended green
        for (int emptyColumn = extendedWidth-1; emptyColumn > ((width-1)+halfLength); emptyColumn--)
        {
            extended[row][emptyColumn] = extended[row][(extendedWidth-1)-N+countRightG];
            countRightG = countRightG + 1;
        }
    }
    // Getting mean and variance for every window
    
    for ( int row = 0; row < extendedHeight; row ++)
    {
        for ( int column = 0; column < extendedWidth; column++)
        {
            for ( int i = 0; i < N; i++)
            {
                for ( int j = 0; j < N; j++)
                {
                    I[i][j] = extended[row + (i - halfLength)][column + (j - halfLength)];//I and P are the same
                }
            }
            
            
            for ( int i = 0; i < N; i++)
            {
                for ( int j = 0; j < N; j++)
                {
                    IbyI[i][j] = 0; // To avoid garbage values
                }
            }
            
            for ( int i = 0; i < N; i++)
            {
                for ( int j = 0; j < N; j++)
                {
                    for ( int inside = 0; inside < N; inside ++)
                    {
                        IbyI[i][j] = IbyI[i][j] + (I[i][inside] * I[inside][j]);
                    }
                }
            }
            sumI = 0;     // For fmeanI
            sumP = 0;     // For fmeanP
            sumIbyI = 0;  // For fmean(I*I)
            sumIbyP = 0;  // For fmean(I*P)
            
            for ( int i = 0; i < N; i++)
            {
                for ( int j = 0; j < N; j++)
                {
                    sumI = sumI + I[i][j];
                    sumP = sumP + I[i][j];
                    sumIbyI = sumIbyI + IbyI[i][j];
                    sumIbyP = sumIbyI;
                }
            }
            
            // Get all the mean and correlation values
            meanI = sumI / (N*N);
            meanP = sumP / (N*N);
            correlationI = sumIbyI / (N*N);
            correlationIP = sumIbyP / (N*N);
            // Get the variance and covariance
            VarianceI = correlationI - (meanI * meanI);
            covarianceIP = correlationIP - (meanI * meanP);
            // Get a and b
            a = covarianceIP / (VarianceI + epsilon);
            b = meanP - (a * meanI);
            // Store a and b of every window into aMap and bMap
            aMap[row][column] = a;
            bMap[row][column] = b;
        }
    }
    
    for ( int row = 0; row < extendedHeight; row++)
    {
        for ( int column = 0; column < extendedWidth; column++)
        {
            // Get a-window and b-window from aMap and bMap
            for ( int i = 0; i < N; i++)
            {
                for ( int j = 0; j < N; j++)
                {
                    aWindow[i][j] = aMap[row + (i - halfLength)][column + (j - halfLength)];
                    bWindow[i][j] = bMap[row + (i - halfLength)][column + (j - halfLength)];
                }
            }
            
            float sumA = 0; // For fmean of a
            float sumB = 0; // For fmean of b
            // Get a window and b window from aMap and bMap
            for ( int i = 0; i < N; i++)
            {
                for ( int j = 0; j < N; j++)
                {
                    sumA = sumA + aWindow[i][j];
                    sumB = sumB + bWindow[i][j];
                }
            }
            // Now get the mean of a and b
            aMean = sumA / (N*N);
            bMean = sumB / (N*N);
            // Get the q of current pixel
            extended[row][column] = (aMean * extended[row][column]) + bMean;
            
        }
    }
    
    for ( int row = 1; row < extendedHeight - 1; row++)
    {
        for ( int column = 1; column < extendedWidth - 1; column ++)
        {
            guidedFilterImage[row-1][column-1][1] = extended[row][column]; // Store Green
        }
    }
    
    //FOR BLUE
    
    //central, top and bottom part
    for(int row = 0; row < height; row++)
    {
        for(int column = 0;column < width; column++)
            
        {
            
            extended[row+halfLength][column+halfLength] = blue[row][column];//copy the red data in the central part of the extended image
             
            int countTopB = 0;                                                 //Top part of extended blue
            for ( int emptyRow = 0; emptyRow < halfLength; emptyRow++)
            {
                extended[emptyRow][column+halfLength] = blue[halfLength-countTopB][column];
                countTopB = countTopB + 1;
            }
            
            int countBottomB = 0;                                              //Bottom part of extended blue
            for ( int emptyrow = extendedHeight-1; emptyrow > ((height-1)+halfLength); emptyrow--)
            {
                extended[emptyrow][column+halfLength] = blue[(height-1)-halfLength+countBottomB][column];
                countBottomB = countBottomB + 1;
            }
        }
    }
    
    //left and right part
    for (int row = 0; row < extendedHeight; row++)
    {
        int countLeftB = 0;                                                   //Left columns of extended blue
        for (int emptyColumn = 0; emptyColumn < halfLength; emptyColumn++)
        {
            extended[row][emptyColumn] = extended[row][(N-1)-countLeftB];
            countLeftB = countLeftB + 1;
        }
        
        int countRightB = 0;                                                 //Right columns of extended blue
        for (int emptyColumn = extendedWidth-1; emptyColumn > ((width-1)+halfLength); emptyColumn--)
        {
            extended[row][emptyColumn] = extended[row][(extendedWidth-1)-N+countRightB];
            countRightB = countRightB + 1;
        }
    }
    // Getting mean and variance for every window
    
    for ( int row = 0; row < extendedHeight; row ++)
    {
        for ( int column = 0; column < extendedWidth; column++)
        {
            for ( int i = 0; i < N; i++)
            {
                for ( int j = 0; j < N; j++)
                {
                    I[i][j] = extended[row + (i - halfLength)][column + (j - halfLength)];//I and P are the same
                }
            }
            
            
            for ( int i = 0; i < N; i++)
            {
                for ( int j = 0; j < N; j++)
                {
                    IbyI[i][j] = 0; // To avoid garbage values
                }
            }
            
            for ( int i = 0; i < N; i++)
            {
                for ( int j = 0; j < N; j++)
                {
                    for ( int inside = 0; inside < N; inside ++)
                    {
                        IbyI[i][j] = IbyI[i][j] + (I[i][inside] * I[inside][j]);
                    }
                }
            }
            sumI = 0;     // For fmeanI
            sumP = 0;     // For fmeanP
            sumIbyI = 0;  // For fmean(I*I)
            sumIbyP = 0;  // For fmean(I*P)
            
            for ( int i = 0; i < N; i++)
            {
                for ( int j = 0; j < N; j++)
                {
                    sumI = sumI + I[i][j];
                    sumP = sumP + I[i][j];
                    sumIbyI = sumIbyI + IbyI[i][j];
                    sumIbyP = sumIbyI;
                }
            }
            
            // Get all the mean and correlation values
            meanI = sumI / (N*N);
            meanP = sumP / (N*N);
            correlationI = sumIbyI / (N*N);
            correlationIP = sumIbyP / (N*N);
            // Get the variance and covariance
            VarianceI = correlationI - (meanI * meanI);
            covarianceIP = correlationIP - (meanI * meanP);
            // Get a and b
            a = covarianceIP / (VarianceI + epsilon);
            b = meanP - (a * meanI);
            // Store a and b of every window into aMap and bMap
            aMap[row][column] = a;
            bMap[row][column] = b;
        }
    }
    
    for ( int row = 0; row < extendedHeight; row++)
    {
        for ( int column = 0; column < extendedWidth; column++)
        {
            // Get a-window and b-window from aMap and bMap
            for ( int i = 0; i < N; i++)
            {
                for ( int j = 0; j < N; j++)
                {
                    aWindow[i][j] = aMap[row + (i - halfLength)][column + (j - halfLength)];
                    bWindow[i][j] = bMap[row + (i - halfLength)][column + (j - halfLength)];
                }
            }
            
            float sumA = 0; // For fmean of a
            float sumB = 0; // For fmean of b
            // Get a window and b window from aMap and bMap
            for ( int i = 0; i < N; i++)
            {
                for ( int j = 0; j < N; j++)
                {
                    sumA = sumA + aWindow[i][j];
                    sumB = sumB + bWindow[i][j];
                }
            }
            // Now get the mean of a and b
            aMean = sumA / (N*N);
            bMean = sumB / (N*N);
            // Get the q of current pixel
            extended[row][column] = (aMean * extended[row][column]) + bMean;
            
        }
    }
    
    for ( int row = 1; row < extendedHeight - 1; row++)
    {
        for ( int column = 1; column < extendedWidth - 1; column ++)
        {
            guidedFilterImage[row-1][column-1][2] = extended[row][column]; //Store Blue
        }
    }
    
    if (!(file=fopen(argv[2],"wb")))
    {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(guidedFilterImage, sizeof(unsigned char),height*width*bytesPerPixel , file);
	fclose(file);
    
    
}
