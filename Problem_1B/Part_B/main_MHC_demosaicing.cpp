//NAME: Amruta Kulkarni
//USC ID: 6914970827
//USC Email: arkulkar@usc.edu
//Submission date: 20 Sept 2015

//Problem 1B, part 2
//This code performs demosaicing using MHC technique for creating a colored 24 bits image
#include <stdio.h>
#include <iostream>
#include <stdlib.h>

using namespace std;

int main(int argc, char *argv[])
{
    FILE *file;
    
    int bytesPerPixelCFA = 1;                                                           //every pixel has 1 byte in CFA
    int bytesPerPixelbldm = 3;                                                          //every pixel has 3 bytes in bilinear demosaiced image
    int width = 424;
    int height = 636;
        
    unsigned char CFAImageData[height][width][bytesPerPixelCFA];                       //array size will be 636 * 424 * 1
    
    /* open the image and extract the data into an array */
    
    if (!(file = fopen(argv[1], "rb")))
    {
        cout << "cannot open file: " << argv[1] << endl;
        exit(1);
    }
    
    fread(CFAImageData, sizeof(unsigned char), height*width*bytesPerPixelCFA, file);
    fclose(file);
    
    
    /* Create an extended CFA image array and the data array to store MHC demosaiced pixel values */
    
    int widthCFAextended = 428;
    int heightCFAextended = 640;
    
    unsigned char CFAExtended[heightCFAextended][widthCFAextended][bytesPerPixelCFA];          //array size will be 640 * 428 * 1
    unsigned char bldmImageData[height][width][bytesPerPixelbldm];                                      //array size will be 636 * 424 * 3
    
    /* Generate reflections in the extended CFA */
    
    for(int row = 0; row < 636; row++)
    {
        for(int column = 0;column < 424; column++)
            
        {
            CFAExtended[row+2][column+2][0] = CFAImageData[row][column][0];//copy the CFA data in the central part of the extended image
                                                                                    //i.e from row2-column2 to row637-column425
            CFAExtended[1][column+2][0] = CFAImageData[1][column][0];      //copy row 1 of CFA to row 1 of extended CFA. Column starts from 2
            CFAExtended[0][column+2][0] = CFAImageData[2][column][0];      //copy row 2 of CFA to row 0 of extendedCFA.
            CFAExtended[638][column+2][0] = CFAImageData[634][column][0];  //copy row 634 of CFA to row 638 of extended CFA
            CFAExtended[639][column+2][0] = CFAImageData[633][column][0];  //copy row 633 of CFA to row 639 of extended CFA
            
        }
    }
    
    for (int row = 0; row < 640; row++)
    {
        
            CFAExtended[row][1][0] = CFAExtended[row][3][0];      //copy column 3 of new extended to column 1
            CFAExtended[row][0][0] = CFAExtended[row][4][0];      //copy column 4 of new extended to column 0
            CFAExtended[row][426][0] = CFAExtended[row][424][0];  //copy column 424 of new extended to column 426
            CFAExtended[row][427][0] = CFAExtended[row][423][0];  //copy column 423 of new extended to column 427
        
    }
    
    
    /* Start MHC Demosaicing */
    
   /* int row = 0, column = 0;
    
    unsigned char filter_GreenAtRed = ( ( 4*CFAExtended[row][column][0]
            + 2* (CFAExtended[row+1][column][0] + CFAExtended[row-1][column][0] + CFAExtended[row][column+1][0] + CFAExtended[row][column-1][0])
            - (CFAExtended[row+2][column][0] + CFAExtended[row-2][column][0] + CFAExtended[row][column+2][0] + CFAExtended[row][column-2][0]) ) );
    
    unsigned char filter_BlueAtRed = ( ( 6*CFAExtended[row][column][0]
            + 2* (CFAExtended[row+1][column+1][0] + CFAExtended[row+1][column-1][0] + CFAExtended[row-1][column+1][0] + CFAExtended[row-1][column-1][0])
            - (3*(CFAExtended[row+2][column][0] + CFAExtended[row-2][column][0] + CFAExtended[row][column+2][0] + CFAExtended[row][column-2][0])/2) ) );
    
    unsigned char filter_GreenAtBlue = ( ( 4*CFAExtended[row][column][0]
            + 2* (CFAExtended[row+1][column][0] + CFAExtended[row-1][column][0] + CFAExtended[row][column+1][0] + CFAExtended[row][column-1][0])
            - (CFAExtended[row+2][column][0] + CFAExtended[row-2][column][0] + CFAExtended[row][column+2][0] + CFAExtended[row][column-2][0]) ) );
    

    unsigned char filter_RedAtBlue = ( ( 6*CFAExtended[row][column][0]
            + 2* (CFAExtended[row+1][column+1][0] + CFAExtended[row+1][column-1][0] + CFAExtended[row-1][column+1][0] + CFAExtended[row-1][column-1][0])
            - (3*(CFAExtended[row+2][column][0] + CFAExtended[row-2][column][0] + CFAExtended[row][column+2][0] + CFAExtended[row][column-2][0])/2) ) );
    

    unsigned char filter_BlueAtGreen_OddRow = ( ( 5*CFAExtended[row][column][0]
            + 4* (CFAExtended[row][column+1][0] + CFAExtended[row][column-1][0])
            - (CFAExtended[row+1][column+1][0] + CFAExtended[row+1][column-1][0] + CFAExtended[row-1][column+1][0] + CFAExtended[row-1][column-1][0] + CFAExtended[row][column+2][0] + CFAExtended[row][column-2][0])
            + ( (CFAExtended[row+2][column][0] + CFAExtended[row-2][column][0])/2) ) );
                                  
    unsigned char filter_BlueAtGreen_EvenRow = ( ( 5*CFAExtended[row][column][0]
            + 4* (CFAExtended[row+1][column][0] + CFAExtended[row-1][column][0])
            - (CFAExtended[row+1][column+1][0] + CFAExtended[row+1][column-1][0] + CFAExtended[row-1][column+1][0] + CFAExtended[row-1][column-1][0] + CFAExtended[row+2][column][0] + CFAExtended[row-2][column][0])
            + ((CFAExtended[row][column+2][0] + CFAExtended[row][column-2][0])/2) ) );
    
    unsigned char filter_RedAtGreen_OddRow = ( ( 5*CFAExtended[row][column][0]
            + 4* (CFAExtended[row+1][column][0] + CFAExtended[row-1][column][0])
            - (CFAExtended[row+1][column+1][0] + CFAExtended[row+1][column-1][0] + CFAExtended[row-1][column+1][0] + CFAExtended[row-1][column-1][0] + CFAExtended[row+2][column][0] + CFAExtended[row-2][column][0])
            + ((CFAExtended[row][column+2][0] + CFAExtended[row][column-2][0])/2) ) );
    
    unsigned char filter_RedAtGreen_EvenRow = ( ( 5*CFAExtended[row][column][0]
            + 4* (CFAExtended[row][column+1][0] + CFAExtended[row][column-1][0])
            - (CFAExtended[row+1][column+1][0] + CFAExtended[row+1][column-1][0] + CFAExtended[row-1][column+1][0] + CFAExtended[row-1][column-1][0] + CFAExtended[row][column+2][0] + CFAExtended[row][column-2][0])
            + ((CFAExtended[row+2][column][0] + CFAExtended[row-2][column][0])/2) ) );
    
   */ 
    
    for ( int row = 2; row < 638; row++)
    {
        for( int column = 2; column < 426; column++)
        {
           if (row % 2 != 0 && column % 2 != 0)                                    //pixel is blue
            {
                bldmImageData[row - 2][column - 2][0] =  ( ( (6*CFAExtended[row][column][0])
                                                            + (2* (CFAExtended[row+1][column+1][0] + CFAExtended[row+1][column-1][0] + CFAExtended[row-1][column+1][0] + CFAExtended[row-1][column-1][0] ))
                                                            - (3*(CFAExtended[row+2][column][0] + CFAExtended[row-2][column][0] + CFAExtended[row][column+2][0] + CFAExtended[row][column-2][0])/2) )/8 );
                
//filter_RedAtBlue;                          //Get RED
                
                bldmImageData[row - 2][column - 2][1] = ( ( (4*CFAExtended[row][column][0])
                                                           + (2* (CFAExtended[row+1][column][0] + CFAExtended[row-1][column][0] + CFAExtended[row][column+1][0] + CFAExtended[row][column-1][0]))
                                                           - (CFAExtended[row+2][column][0] + CFAExtended[row-2][column][0] + CFAExtended[row][column+2][0] + CFAExtended[row][column-2][0]) )/8 );
                
//filter_GreenAtBlue;                        //Get GREEN
                                                            
                bldmImageData[row - 2][column - 2][2] = CFAExtended[row][column][0];              //Already BLUE
                
            }
           
            else if (row % 2 == 0 && column % 2 == 0)                                //pixel is red
            {
                bldmImageData[row - 2][column - 2][0] = CFAExtended[row][column][0];              //Already RED
                
                bldmImageData[row - 2][column - 2][1] = ( ( (4*CFAExtended[row][column][0])
                                                           + (2* (CFAExtended[row+1][column][0] + CFAExtended[row-1][column][0] + CFAExtended[row][column+1][0] + CFAExtended[row][column-1][0]))
                                                           - (CFAExtended[row+2][column][0] + CFAExtended[row-2][column][0] + CFAExtended[row][column+2][0] + CFAExtended[row][column-2][0]) )/8 );
                
//filter_GreenAtRed;                        //Get GREEN
                                                            
                bldmImageData[row - 2][column - 2][2] = ( ( (6*CFAExtended[row][column][0])
                                                          + (2* (CFAExtended[row+1][column+1][0] + CFAExtended[row+1][column-1][0] + CFAExtended[row-1][column+1][0] + CFAExtended[row-1][column-1][0]))
                                                          - (3*(CFAExtended[row+2][column][0] + CFAExtended[row-2][column][0] + CFAExtended[row][column+2][0] + CFAExtended[row][column-2][0])/2) )/8 );
                
//filter_BlueAtRed;                         //Get BLUE
            }
            
            else if (row % 2 == 0 && column % 2 != 0)                              //pixel is green at even row
            {
                bldmImageData[row - 2][column - 2][0] = ( ( (5*CFAExtended[row][column][0])
                                                           + (4* (CFAExtended[row][column+1][0] + CFAExtended[row][column-1][0]) )
                                                           - (CFAExtended[row+1][column+1][0] + CFAExtended[row+1][column-1][0] + CFAExtended[row-1][column+1][0] + CFAExtended[row-1][column-1][0] + CFAExtended[row][column+2][0] + CFAExtended[row][column-2][0])
                                                           + ((CFAExtended[row+2][column][0] + CFAExtended[row-2][column][0])/2) )/8 );
                
//filter_RedAtGreen_EvenRow;                //Get RED
                                            
                bldmImageData[row - 2][column - 2][1] = CFAExtended[row][column][0];              //Already GREEN
                
                bldmImageData[row - 2][column - 2][2] = ( ( (5*CFAExtended[row][column][0])
                                                           + (4* (CFAExtended[row+1][column][0] + CFAExtended[row-1][column][0]) )
                                                           - (CFAExtended[row+1][column+1][0] + CFAExtended[row+1][column-1][0] + CFAExtended[row-1][column+1][0] + CFAExtended[row-1][column-1][0] + CFAExtended[row+2][column][0] + CFAExtended[row-2][column][0])
                                                           + ( (CFAExtended[row][column+2][0] + CFAExtended[row][column-2][0])/2) )/8 );
                
//filter_BlueAtGreen_EvenRow;               //Get BLUE
            }
            
            else                                                                   //pixel is green at odd row
            {
                
                bldmImageData[row - 2][column - 2][0] = ( ( (5*CFAExtended[row][column][0])
                                                           + (4* (CFAExtended[row+1][column][0] + CFAExtended[row-1][column][0]) )
                                                           - (CFAExtended[row+1][column+1][0] + CFAExtended[row+1][column-1][0] + CFAExtended[row-1][column+1][0] + CFAExtended[row-1][column-1][0] + CFAExtended[row+2][column][0] + CFAExtended[row-2][column][0])
                                                           + ((CFAExtended[row][column+2][0] + CFAExtended[row][column-2][0])/2) )/8 );
                
//filter_RedAtGreen_OddRow;                //Get RED
                
                bldmImageData[row - 2][column - 2][1] = CFAExtended[row][column][0];             //Already GREEN
                
                bldmImageData[row - 2][column - 2][2] = ( ( (5*CFAExtended[row][column][0])
                                                           + (4* (CFAExtended[row][column+1][0] + CFAExtended[row][column-1][0]) )
                                                           - (CFAExtended[row+1][column+1][0] + CFAExtended[row+1][column-1][0] + CFAExtended[row-1][column+1][0] + CFAExtended[row-1][column-1][0] + CFAExtended[row][column+2][0] + CFAExtended[row][column-2][0])
                                                           + ( (CFAExtended[row+2][column][0] + CFAExtended[row-2][column][0])/2) )/8 );
                
//filter_BlueAtGreen_OddRow;               //Get BLUE
            }
        }
    }
            
    
    /* Write the MHC demosaiced image to a new file */
    
    if (!(file=fopen(argv[2],"wb")))
    {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(bldmImageData, sizeof(unsigned char), height*width*bytesPerPixelbldm, file);
	fclose(file);
    
	return 0;
    
}