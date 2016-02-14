//NAME: Amruta Kulkarni
//USC ID: 6914970827
//USC Email: arkulkar@usc.edu
//Submission date: 20 Sept 2015

//Problem 3A
//This code is for noise removal from a noisy image corrupted with selt & pepper as well as Gaussian noise
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <algorithm>

using namespace std;

int main(int argc, char *argv[])
{
    
    /* Define file pointer and variable */
    FILE *file;
    int bytesPerPixel = 3;
    int width = 512;
    int height = 512;
    
    /* Allocate image data array */
    unsigned char imageData[height][width][bytesPerPixel];
    
    /* Read image */
    if (!(file=fopen(argv[1],"rb")))
    {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	fread(imageData, sizeof(unsigned char), height*width*bytesPerPixel, file);
	fclose(file);
    
    /* Separate R,G,B matrices */
    
    unsigned char red[height][width];
    unsigned char green[height][width];
    unsigned char blue[height][width];
    
    for ( int row = 0; row < height; row++)
    {
        for (int column = 0; column < width; column++)
        {
            red[row][column] = imageData[row][column][0];
            green[row][column] = imageData[row][column][1];
            blue[row][column] = imageData[row][column][2];
        }
    }
    
                                                    
    /* Denoising */
    
    int N = 7;
    int extendedWidth = width + (N-1);
    int extendedHeight = height + (N-1);
    int halfLength = (N-1)/2;
    
    // Create en extended array 
    
    int extendedRed[extendedHeight][extendedWidth];
    int extendedGreen[extendedHeight][extendedWidth];
    int extendedBlue[extendedHeight][extendedWidth];
    
    //central, top and bottom part
    for(int row = 0; row < height; row++)
    {
        for(int column = 0;column < width; column++)
            
        {
            //FOR RED
            extendedRed[row+halfLength][column+halfLength] = red[row][column];//copy the red data in the central part of the extended image
            
            int countTopR = 0;                                                 //Top part of extended
            for ( int emptyRow = 0; emptyRow < halfLength; emptyRow++)
            {
                extendedRed[emptyRow][column+halfLength] = red[halfLength-countTopR][column];
                countTopR = countTopR + 1;
            }
            
            int countBottomR = 0;                                              //Bottom part of extended
            for ( int emptyrow = extendedHeight-1; emptyrow > ((height-1)+halfLength); emptyrow--)
            {
                extendedRed[emptyrow][column+halfLength] = red[(height-1)-halfLength+countBottomR][column];
                countBottomR = countBottomR + 1;
            }
            
            //FOR GREEN
            extendedGreen[row+halfLength][column+halfLength] = green[row][column];//copy the red data in the central part of the extended image
            
            int countTopG = 0;                                                 //Top part of extended
            for ( int emptyRow = 0; emptyRow < halfLength; emptyRow++)
            {
                extendedGreen[emptyRow][column+halfLength] = green[halfLength-countTopG][column];
                countTopG = countTopG + 1;
            }
            
            int countBottomG = 0;                                              //Bottom part of extended
            for ( int emptyrow = extendedHeight-1; emptyrow > ((height-1)+halfLength); emptyrow--)
            {
                extendedGreen[emptyrow][column+halfLength] = green[(height-1)-halfLength+countBottomG][column];
                countBottomG = countBottomG + 1;
            }

            //FOR BLUE
            extendedBlue[row+halfLength][column+halfLength] = blue[row][column];//copy the red data in the central part of the extended image
            
            int countTopB = 0;                                                 //Top part of extended
            for ( int emptyRow = 0; emptyRow < halfLength; emptyRow++)
            {
                extendedBlue[emptyRow][column+halfLength] = blue[halfLength-countTopB][column];
                countTopB = countTopB + 1;
            }
            
            int countBottomB = 0;                                              //Bottom part of extended
            for ( int emptyrow = extendedHeight-1; emptyrow > ((height-1)+halfLength); emptyrow--)
            {
                extendedBlue[emptyrow][column+halfLength] = blue[(height-1)-halfLength+countBottomB][column];
                countBottomB = countBottomB + 1;
            }

        }
    }
    
    
    //left and right part
    for (int row = 0; row < extendedHeight; row++)
    {
        //FOR RED
        int countLeftR = 0;                                                   //Left columns of extended
        for (int emptyColumn = 0; emptyColumn < halfLength; emptyColumn++)
        {
            extendedRed[row][emptyColumn] = extendedRed[row][(N-1)-countLeftR];
            countLeftR = countLeftR + 1;
        }
        
        int countRightR = 0;                                                 //Right columns of extended
        for (int emptyColumn = extendedWidth-1; emptyColumn > ((width-1)+halfLength); emptyColumn--)
        {
            extendedRed[row][emptyColumn] = extendedRed[row][(extendedWidth-1)-N+countRightR];
            countRightR = countRightR + 1;
        }
        
        //FOR GREEN
        int countLeftG = 0;                                                   //Left columns of extended
        for (int emptyColumn = 0; emptyColumn < halfLength; emptyColumn++)
        {
            extendedGreen[row][emptyColumn] = extendedGreen[row][(N-1)-countLeftG];
            countLeftG = countLeftG + 1;
        }
        
        int countRightG = 0;                                                 //Right columns of extended
        for (int emptyColumn = extendedWidth-1; emptyColumn > ((width-1)+halfLength); emptyColumn--)
        {
            extendedGreen[row][emptyColumn] = extendedGreen[row][(extendedWidth-1)-N+countRightG];
            countRightG = countRightG + 1;
        }

        //FOR BLUE
        int countLeftB = 0;                                                   //Left columns of extended
        for (int emptyColumn = 0; emptyColumn < halfLength; emptyColumn++)
        {
            extendedBlue[row][emptyColumn] = extendedBlue[row][(N-1)-countLeftB];
            countLeftB = countLeftB + 1;
        }
        
        int countRightB = 0;                                                 //Right columns of extended
        for (int emptyColumn = extendedWidth-1; emptyColumn > ((width-1)+halfLength); emptyColumn--)
        {
            extendedBlue[row][emptyColumn] = extendedBlue[row][(extendedWidth-1)-N+countRightB];
            countRightB = countRightB + 1;
        }

    }
    
                            /* DENOISING */
    
    // For Red which has both gaussian as well as impulse noise
    
    int neighborhoodMatrixR[N][N];
    int arrayR[N*N];
    float gaussianKernel[N][N];
    
    for (int row = 0; row < extendedHeight; row++)
    {
        for (int column = 0; column < extendedWidth; column++)
        {
            int currentPixelValue = extendedRed[row][column];
            
            //Get the N * N window
            for ( int i = 0; i < N; i++)
            {
                for ( int j = 0; j < N; j++)
                {
                    neighborhoodMatrixR[i][j] = extendedRed[row + (i - halfLength)][column + (j - halfLength)];
                }
            }
            
            int indexR = 0;
            //Calculate minimum, maximum and median of the window
            for ( int i = 0; i < N; i++)
            {
                for ( int j = 0; j < N; j++)
                {
                    arrayR[indexR] = neighborhoodMatrixR[i][j];
                    indexR = indexR + 1;
                }
            }
            
            int size = sizeof(arrayR) / sizeof(arrayR[0]);
            sort(arrayR, arrayR + size); //arrange the array in ascending order
            
                    /* Rank order based adaptive median filter */
            
            // if median is strictly between min and max of the window
            if ( arrayR[(((N*N)-1)/2)] > arrayR[0] && arrayR[(((N*N)-1)/2)] < arrayR[(N*N)-1] )
            
            {
                // if pixel is strictly between min and max of the window then retain it
                if( extendedRed[row][column] > arrayR[0] && extendedRed[row][column] < arrayR[(N*N)-1]  )
                {
                    extendedRed[row][column] = currentPixelValue;
                }
                // if pixel is not strictly between min and max then replace it with median of the window
                else
                {
                    extendedRed[row][column] = arrayR[(((N*N)-1)/2)];
                }
            }
        }
    }
    
    /* Mean filter */
    
     int sum;
     float mean;
        for (int row = 0; row < extendedHeight; row++)
        {
            for (int column = 0; column < extendedWidth; column++)
            {
     
                //Get the N * N window
                for ( int i = 0; i < N; i++)
                {
                    for ( int j = 0; j < N; j++)
                    {
                        neighborhoodMatrixR[i][j] = extendedRed[row + (i - halfLength)][column + (j - halfLength)];
                    }
                }
     
                sum = 0;
                for ( int i = 0; i < N; i++)
                {
                    for ( int j = 0; j < N; j++)
                        {
                            sum = sum + neighborhoodMatrixR[i][j];
                        }
                }
                mean = sum / (N*N);              // Calculate mean of the window
     
                extendedRed[row][column] = mean; // Assign the mean of the window to the current pixel
            }
        }

    
    // Uncomment the following to use gaussian instead of mean filter
    /* Gaussian filter */
    /* 
    for (int row = 0; row < extendedHeight; row++)
        {
            for (int column = 0; column < extendedWidth; column++)
            {
                //first create a gaussian kernel
                int sumConvolution = 0;
                float stdDev = 8;
                float sumKernel = 0;
                for (int i = 0; i <= N; i++)
                {
                    for (int j = 0; j <= N; j++)
                    {
                        gaussianKernel[i][j] = exp(-(((i-halfLength)*(i-halfLength)) + ((j-halfLength)*(j-halfLength)))/(2*stdDev*stdDev));
                    }
                }
     
                //Get the N * N window
                for ( int i = 0; i < N; i++)
                {
                    for ( int j = 0; j < N; j++)
                    {
                        neighborhoodMatrixR[i][j] = extendedRed[row + (i - halfLength)][column + (j - halfLength)];
                    }
                }
     
     
                for (int i = 0; i < N; i++)
                {
                    for (int j = 0; j < N; j++)
                    {
                        sumConvolution = sumConvolution + (neighborhoodMatrixR[i][j] * gaussianKernel[i][j]);
                        sumKernel = sumKernel + gaussianKernel[i][j];
                    }
                }
     
                extendedRed[row][column] = sumConvolution / sumKernel;
            }
     }
     */
   
    // For Green which has impulse noise
    
    int neighborhoodMatrixG[N][N];
    int arrayG[N*N];
    for (int row = 0; row < extendedHeight; row++)
    {
        for (int column = 0; column < extendedWidth; column++)
        {
            int currentPixelValue = extendedGreen[row][column];
            
            //Get the N * N window
            for ( int i = 0; i < N; i++)
            {
                for ( int j = 0; j < N; j++)
                {
                    neighborhoodMatrixG[i][j] = extendedGreen[row + (i - halfLength)][column + (j - halfLength)];
                }
            }
            
            int indexG = 0;
            //Calculate minimum, maximum and median of the window
            for ( int i = 0; i < N; i++)
            {
                for ( int j = 0; j < N; j++)
                {
                    arrayG[indexG] = neighborhoodMatrixG[i][j];
                    indexG = indexG + 1;
                }
            }
            
            int size = sizeof(arrayG) / sizeof(arrayG[0]);
            sort(arrayG, arrayG + size); //arrange the array in ascending order
            
            /* Rank order based adaptive median filter */
            
            // if median is strictly between min and max of the window
            if ( arrayG[(((N*N)-1)/2)] > arrayG[0] && arrayG[(((N*N)-1)/2)] < arrayG[(N*N)-1] )
                
            {
                // if pixel is strictly between min and max of the window then retain it
                if( extendedGreen[row][column] > arrayG[0] && extendedGreen[row][column] < arrayG[(N*N)-1]  )
                {
                    extendedGreen[row][column] = currentPixelValue;
                }
                // if pixel is not strictly between min and max then replace it with median of the window
                else
                {
                    extendedGreen[row][column] = arrayG[(((N*N)-1)/2)];
                }
            }
        }
    }

    
    // For Blue which has impulse noise and gaussian noise
    
    int neighborhoodMatrixB[N][N];
    int arrayB[N*N];
    for (int row = 0; row < extendedHeight; row++)
    {
        for (int column = 0; column < extendedWidth; column++)
        {
            int currentPixelValue = extendedBlue[row][column];
            
            //Get the N * N window
            for ( int i = 0; i < N; i++)
            {
                for ( int j = 0; j < N; j++)
                {
                    neighborhoodMatrixB[i][j] = extendedBlue[row + (i - halfLength)][column + (j - halfLength)];
                }
            }
            
            int indexB = 0;
            //Calculate minimum, maximum and median of the window
            for ( int i = 0; i < N; i++)
            {
                for ( int j = 0; j < N; j++)
                {
                    arrayB[indexB] = neighborhoodMatrixB[i][j];
                    indexB = indexB + 1;
                }
            }
            
            int size = sizeof(arrayB) / sizeof(arrayB[0]);
            sort(arrayB, arrayB + size); //arrange the array in ascending order
            
            /* Rank order based adaptive median filter */
            
            // if median is strictly between min and max of the window
            if ( arrayB[(((N*N)-1)/2)] > arrayB[0] && arrayB[(((N*N)-1)/2)] < arrayB[(N*N)-1] )
                
            {
                // if pixel is strictly between min and max of the window then retain it
                if( extendedBlue[row][column] > arrayB[0] && extendedBlue[row][column] < arrayB[(N*N)-1]  )
                {
                    extendedBlue[row][column] = currentPixelValue;
                }
                // if pixel is not strictly between min and max then replace it with median of the window
                else
                {
                    extendedBlue[row][column] = arrayB[(((N*N)-1)/2)];
                }
            }
        }
    }
    
    /* Mean filter */
    
    for (int row = 0; row < extendedHeight; row++)
    {
        for (int column = 0; column < extendedWidth; column++)
        {
            
            //Get the N * N window
            for ( int i = 0; i < N; i++)
            {
                for ( int j = 0; j < N; j++)
                {
                    neighborhoodMatrixB[i][j] = extendedBlue[row + (i - halfLength)][column + (j - halfLength)];
                }
            }
            
            sum = 0;
            for ( int i = 0; i < N; i++)
            {
                for ( int j = 0; j < N; j++)
                {
                    sum = sum + neighborhoodMatrixB[i][j];
                }
            }
            mean = sum / (N*N);              // Calculate mean of the window
            
            extendedBlue[row][column] = mean; // Assign the mean of the window to the current pixel
        }
    }
    
    //Uncomment the following to use gaussian instead of mean filter
    /* Gaussian filter */
    /*
    for (int row = 0; row < extendedHeight; row++)
    {
        for (int column = 0; column < extendedWidth; column++)
        {
            //first create a gaussian kernel
            int sumConvolution = 0;
            float stdDev = 8;
            float sumKernel = 0;
            for (int i = 0; i <= N; i++)
            {
                for (int j = 0; j <= N; j++)
                {
                    gaussianKernel[i][j] = exp(-(((i-halfLength)*(i-halfLength)) + ((j-halfLength)*(j-halfLength)))/(2*stdDev*stdDev));
                }
            }
            
            //Get the N * N window
            for ( int i = 0; i < N; i++)
            {
                for ( int j = 0; j < N; j++)
                {
                    neighborhoodMatrixB[i][j] = extendedBlue[row + (i - halfLength)][column + (j - halfLength)];
                }
            }
            
            
            for (int i = 0; i < N; i++)
            {
                for (int j = 0; j < N; j++)
                {
                    sumConvolution = sumConvolution + (neighborhoodMatrixB[i][j] * gaussianKernel[i][j]);
                    sumKernel = sumKernel + gaussianKernel[i][j];
                }
            }
            
            extendedBlue[row][column] = sumConvolution / sumKernel;
        }
    }
    */

    // Create the final noise free image

    unsigned char noiseFreeImage[height][width][bytesPerPixel];
    for(int i = halfLength; i < extendedHeight-halfLength; i++)
    {
        for(int j = halfLength; j < extendedWidth-halfLength; j++)
        {
            noiseFreeImage[i-halfLength][j-halfLength][0] = extendedRed[i][j];
            noiseFreeImage[i-halfLength][j-halfLength][1] = extendedGreen[i][j];
            noiseFreeImage[i-halfLength][j-halfLength][2] = extendedBlue[i][j];
        }
    }
    
    
    if (!(file=fopen(argv[2],"wb")))
    {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(noiseFreeImage, sizeof(unsigned char), height*width*bytesPerPixel, file);
	fclose(file);
    
    /* PSNR Calculations */
    
    unsigned char imageDataOriginal[height][width][bytesPerPixel];
    
    /* Read image */
    
    if (!(file=fopen(argv[3],"rb")))
    {
		cout << "Cannot open file: " << argv[3] <<endl;
		exit(1);
	}
	fread(imageDataOriginal, sizeof(unsigned char), height*width*bytesPerPixel, file);
	fclose(file);
    
    double MSERedPixel = 0;
    double sumRed = 0;
    double MSERed = 0;
    double PSNRRed = 0;
    
    double MSEGreenPixel = 0;
    double sumGreen = 0;
    double MSEGreen = 0;
    double PSNRGreen = 0;
    
    double MSEBluePixel = 0;
    double sumBlue= 0 ;
    double MSEBlue = 0;
    double PSNRBlue = 0;
    
    const int MAX = 255;
    
    for (int row = 0; row < height; row++)
    {
        for (int column = 0; column < width; column++)
        {
            MSERedPixel = (imageDataOriginal[row][column][0] - noiseFreeImage[row][column][0]) * (imageDataOriginal[row][column][0] - noiseFreeImage[row][column][0]) ;
            sumRed = sumRed + MSERedPixel;
            
            MSEGreenPixel = (imageDataOriginal[row][column][1] - noiseFreeImage[row][column][1]) * (imageDataOriginal[row][column][1] - noiseFreeImage[row][column][1]);
            sumGreen = sumGreen + MSEGreenPixel;
            
            MSEBluePixel = (imageDataOriginal[row][column][2] - noiseFreeImage[row][column][2]) * (imageDataOriginal[row][column][2] - noiseFreeImage[row][column][2]) ;
            sumBlue = sumBlue + MSEBluePixel;
            
        }
    }
    
    MSERed = sumRed / (height * width);
    PSNRRed = 10 * log10((MAX * MAX)/MSERed);
    
    MSEGreen = sumGreen / (height * width);
    PSNRGreen = 10 * log10((MAX * MAX)/MSEGreen);
    
    MSEBlue = sumBlue / (height * width);
    PSNRBlue = 10 * log10((MAX * MAX)/MSEBlue);
    
    cout << "PSNR for Red = " << PSNRRed << "PSNR for Green = " << PSNRGreen << "PSNR for Blue = " << PSNRBlue;
    
    
    return 0;
    
}

