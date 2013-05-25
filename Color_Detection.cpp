#include "stdafx.h"
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include <stdio.h>
#include <string.h>
#include <numeric>
#include <time.h>
#include <windows.h>
#include <mmsystem.h> 
 
int main(int argc, char** argv)
{
	// Default capture size - 640x480
    CvSize size = cvSize(640,480); // 640 width 480 height
//*****************************************************************************
//*							Variable declartion
//*****************************************************************************
		DWORD     bytes_read    = 0;				 // Number of bytes read from port
		DWORD     bytes_written =0;					 // Number of bytes written to the port
		HANDLE      comport;						 // Handle COM port
		int			bStatus=0;
		DCB         comSettings;
		
		float		x_tracking=0;					// track X coordinate of arm
		int			x_max = 640;					//x_max tracking
		int			x_min = 0;						//x_min tracking
		int			x_center = x_max/2;				//x_center tracking
		int			x_center_threshold=20;			// change me for better centering
		int     x_minlb = toupper(x_center * (1-(x_center_threshold * .01))); //20% Threshold = 256 
		int	    x_maxub = toupper(x_center * (1+(x_center_threshold * .01))); //20% Threshold = 384
		
		
		float		y_tracking=0;					// track y coordinate of arm
		int			y_max = 480;					//y_max tracking
		int			y_min = 0;						//y_min tracking
		int			y_center = y_max/2;				//y_center tracking
		int			y_center_threshold=20;			// change me for better centering
		int     y_minlb = toupper(y_center * (1-(y_center_threshold * .01))); //20% Threshold = 256 
		int	    y_maxub = toupper(y_center * (1+(y_center_threshold * .01))); //20% Threshold = 384
		
		float		r_tracking=0;					// track r coordinate of arm					
		int			r_max = 180;					//r_max tracking
		int			r_min = 0;						//r_min tracking
		int			r_center = r_max/3;				//y_center tracking
		int			r_center_threshold=20;			// change me for better centering
		int     r_minlb = toupper(r_center * (1-(r_center_threshold * .01))); //20% Threshold = 256 
		int	    r_maxub = toupper(r_center * (1+(r_center_threshold * .01))); //20% Threshold = 384
		int			r_movespos=0;						//Need to keep track of how many moves 
		int			r_movesneg=2;						//Need to keep track of how many moves
		int			y_movespos=0;						//Need to keep track of how many moves 
		int			y_movesneg=0;						//Need to keep track of how many moves

		int delay = 750; //Delay to send out command in milliseconds
		int delayy=750; //Delay to send out command in milliseconds
		int delayr=750; //Delay to send out command in milliseconds

		int standbytime=delay + clock();
		int standbytimey=delayy + clock();
		int standbytimer=delayr + clock();
		//COMMTIMEOUTS CommTimeouts;	// Contains various port settings
		
	
		unsigned char buffer1[9];		// Hex command to make Robot Arm Right
		unsigned char buffer2[9];		// Hex command to make Robot Arm Left
		unsigned char buffer3[9];		// Hex command to make Robot Wrist Up
		unsigned char buffer4[9];		// Hex command to make Robot Wrist Down
		unsigned char buffer5[9];		// Hex command to make Robot Hand Open
		unsigned char buffer6[9];		// Hex command to make Robot Hand Close
	

		float smooth_arm[5]={x_center};		// Initialize smooth arm array
		float smooth_army[5]={y_center};	// Initialize smooth arm y array
		float smooth_armr[7]={r_center};	// Initialize smooth arm hand array

		//std::fill( smooth_arm, smooth_arm + 100, x_center );
		float		x_avg = 0;			// Initialize x_avg array value
		float       y_avg = 0;			// Initialize y_avg array value
		float		r_avg = 0;			// Initialize r_avg array value

		int Count = sizeof(smooth_arm) / sizeof(smooth_arm[0]); // Calculates size of the array
		int Count_y = sizeof(smooth_army) / sizeof(smooth_army[0]); // Calculates size of the array
		int Count_r = sizeof(smooth_armr) / sizeof(smooth_armr[0]); // Calculates size of the array
		
//****************************************
//*			Robot Arm Right
//****************************************
		buffer1[0] = 0x07; //  
		buffer1[1] = 0x00; //  
		buffer1[2] = 0x80; //  
		buffer1[3] = 0x09; //  
		buffer1[4] = 0x00; //        
		buffer1[5] = 0x03; //     
		buffer1[6] = 0x02; //  
		buffer1[7] = 0x8A; //  
		buffer1[8] = 0x00; //
//****************************************
//*			Robot Arm Left
//****************************************
		buffer2[0] = 0x07; //  
		buffer2[1] = 0x00; //  
		buffer2[2] = 0x80; //  
		buffer2[3] = 0x09; //  
		buffer2[4] = 0x00; //        
		buffer2[5] = 0x03; //     
		buffer2[6] = 0x02; //  
		buffer2[7] = 0x0A; //  
		buffer2[8] = 0x00; //
//****************************************
//*		Robot Arm Wrist Up
//****************************************
		buffer3[0] = 0x07; //  
		buffer3[1] = 0x00; //  
		buffer3[2] = 0x80; //  
		buffer3[3] = 0x09; //  
		buffer3[4] = 0x00; //        
		buffer3[5] = 0x03; //     
		buffer3[6] = 0x04; //  
		buffer3[7] = 0x8A; //  
		buffer3[8] = 0x00; //
//****************************************
//*	 Robot Arm Wrist Down
//****************************************
		buffer4[0] = 0x07; //  
		buffer4[1] = 0x00; //  
		buffer4[2] = 0x80; //  
		buffer4[3] = 0x09; //  
		buffer4[4] = 0x00; //        
		buffer4[5] = 0x03; //     
		buffer4[6] = 0x04; //  
		buffer4[7] = 0x0A; //  
		buffer4[8] = 0x00; //
//****************************************
//*	 Robot Arm Hand Open
//****************************************
		buffer5[0] = 0x07; //  
		buffer5[1] = 0x00; //  
		buffer5[2] = 0x80; //  
		buffer5[3] = 0x09; //  
		buffer5[4] = 0x00; //        
		buffer5[5] = 0x03; //     
		buffer5[6] = 0x06; //  
		buffer5[7] = 0x8A; //  
		buffer5[8] = 0x00; //
//****************************************
//*		Robot Arm Hand Close
//****************************************
		buffer6[0] = 0x07; //  
		buffer6[1] = 0x00; //  
		buffer6[2] = 0x80; //  
		buffer6[3] = 0x09; //  
		buffer6[4] = 0x00; //        
		buffer6[5] = 0x03; //     
		buffer6[6] = 0x06; //  
		buffer6[7] = 0x0A; //  
		buffer6[8] = 0x00; //

//********************************************************
//*  Open com port 6 and sends command to beep on the NXT
//********************************************************
		comport = CreateFile(TEXT("\\\\.\\COM6"),      // open com6
                    GENERIC_READ | GENERIC_WRITE, // for reading and writing
                    0,                            // exclusive access
                    NULL,                         // no security attributes
                    OPEN_EXISTING,              
                    FILE_ATTRIBUTE_NORMAL,
                    NULL);//== INVALID_HANDLE_VALUE)
	
	// Set Port parameters.
	printf("\nSet Port Parameters...................");
	GetCommState(comport, &comSettings);
    
	comSettings.BaudRate = 9600;
	printf("\n %d = Baud Rate",comSettings.BaudRate);
    
	comSettings.StopBits = ONESTOPBIT;
	printf("\n %c = Stop Bits", comSettings.StopBits);
    
	comSettings.ByteSize = 8;
	printf("\n %d = Byte Size", comSettings.ByteSize);
    
	comSettings.Parity   = NOPARITY;
	printf("\n %c = Parity", comSettings.Parity);
    comSettings.fParity  = FALSE;

	//***********************************************************************
	//*				Capture image function for face detection
	//***********************************************************************
 
    // Open capture device. 0 is /dev/video0, 1 is /dev/video1, etc.
	ShellExecuteA(NULL, "open", "c:\\vsa\\9.bat", NULL, NULL, SW_SHOWDEFAULT);
    CvCapture* capture = cvCaptureFromCAM( 0 );
    if( !capture )
    {
            fprintf( stderr, "ERROR: Problem capture image from default device \n" );
            getchar();
            return -1;
    }
 
    // Create a window in which the captured images will be presented
    cvNamedWindow( "Camera", CV_WINDOW_AUTOSIZE );
    cvNamedWindow( "HSV", CV_WINDOW_AUTOSIZE );
    //cvNamedWindow( "EdgeDetection", CV_WINDOW_AUTOSIZE );
 
    // Detect a red ball
    CvScalar hsv_min = cvScalar(150, 84, 130, 0);
    CvScalar hsv_max = cvScalar(358, 256, 255, 0);//358 max hue for red
	
	//// Detect a green ball
	// CvScalar hsv_min = cvScalar(125, 84, 130, 0);
	// CvScalar hsv_max = cvScalar(175, 256, 255, 0);


	//Detect a yellow ball
	 /*CvScalar hsv_min = cvScalar(20, 100, 100, 0);
    CvScalar hsv_max = cvScalar(30, 255, 255, 0);*/

	//Detect a pink Ball
	/*CvScalar hsv_min = cvScalar(0, 50, 170, 0);
    CvScalar hsv_max = cvScalar(10, 180, 256, 0);
    CvScalar hsv_min2 = cvScalar(170, 50, 170, 0);
    CvScalar hsv_max2 = cvScalar(256, 180, 256, 0);*/

	IplImage *  hsv_frame    = cvCreateImage(size, IPL_DEPTH_8U, 3);
    IplImage*  thresholded   = cvCreateImage(size, IPL_DEPTH_8U, 1);
 
    while( 1 )
    {
        // Capture image from default camera
        IplImage* frame = cvQueryFrame( capture );
        if( !frame )
        {
                fprintf( stderr, "ERROR: frame is null...\n" );
                getchar();
                break;
        }
 
        // Covert color space to HSV as it is much easier to filter colors in the HSV color-space.
        cvCvtColor(frame, hsv_frame, CV_BGR2HSV);
        // Filter out colors which are out of range.
        cvInRangeS(hsv_frame, hsv_min, hsv_max, thresholded);
 
        // Memory for hough circles
        CvMemStorage* storage = cvCreateMemStorage(0);
        // hough detector works great with smooth image
        cvSmooth( thresholded, thresholded, CV_GAUSSIAN, 9, 9 );
        CvSeq* circles = cvHoughCircles(thresholded, storage, CV_HOUGH_GRADIENT, 2,
                                        thresholded->height/4, 100, 50, 10, 400);
 
        for (int i = 0; i < circles->total; i++)
        {
            float* p = (float*)cvGetSeqElem( circles, i );
            printf("Red Detected.... x=%f y=%f r=%f\n\r",p[0],p[1],p[2]);
            x_tracking = p[0];
			y_tracking = p[1];
			r_tracking = p[2];
			
			cvCircle( frame, cvPoint(cvRound(p[0]),cvRound(p[1])),
                                    3, CV_RGB(0,255,0), -1, 8, 0 );
            cvCircle( frame, cvPoint(cvRound(p[0]),cvRound(p[1])),
                                    cvRound(p[2]), CV_RGB(255,0,0), 3, 8, 0 );

			x_avg = (x_avg + x_tracking)/2; // Average x value of the circles since many can be detected
			y_avg=  (y_avg + y_tracking)/2; // Average y value of the circles since many can be detected
			r_avg=  (r_avg + r_tracking)/2; // Average r value of the circles since many can be detected
        }
		
		//*********************************************************************
		//* Captures time in a matrix. This needed to smooth out arm movements
		//*********************************************************************

		// Calculates the average value of the array. Functions written for both Int and Float numbers
	   
		smooth_arm[0]=x_avg;
		smooth_army[0]=y_avg;
		smooth_armr[0]=r_avg;

		float x_average = std::accumulate( smooth_arm, smooth_arm + Count, 0.0f )/Count;
		float y_average = std::accumulate( smooth_army, smooth_army + Count_y, 0.0f )/Count_y;
		float r_average = std::accumulate( smooth_armr, smooth_armr + Count_r, 0.0f )/Count_r;

		printf("Average X.... Average=%f\n\r",x_average);
		printf("Average Y.... Average=%f\n\r",y_average);
		printf("Average R.... Average=%f\n\r",r_average);
		
		if (standbytime <= clock())
		{
			if  (x_average > x_min && x_average < x_minlb)
				{
					printf("TURN RIGHT");
					printf("TURN RIGHT");
					printf("TURN RIGHT");
					WriteFile(comport,     // Handle
					&buffer2,			  // Outgoing data
					9,             		   // Number of bytes to write
					&bytes_written,
					NULL);      // Number of bytes written)
					WriteFile(comport,     // Handle
					&buffer2,			  // Outgoing data
					9,             		   // Number of bytes to write
					&bytes_written,
					NULL);      // Number of bytes written)
				}
			else if (x_average > x_maxub && x_average < x_max)
				{
					printf("TURN LEFT");
					printf("TURN LEFT");
					printf("TURN LEFT");
					WriteFile(comport,     // Handle
					&buffer1,			  // Outgoing data
					9,             		   // Number of bytes to write
					&bytes_written,
					NULL);      // Number of bytes written)
					WriteFile(comport,     // Handle
					&buffer1,			  // Outgoing data
					9,             		   // Number of bytes to write
					&bytes_written,
					NULL);      // Number of bytes written)
				}
			standbytime = delay + clock();
		}
	if (standbytimey <= clock())
		{
			if  (y_average > y_min && y_average < y_minlb)
			{
				if (y_movesneg < 3)
				{
					printf("TURN RIGHT");
					printf("TURN RIGHT");
					printf("TURN RIGHT");
					WriteFile(comport,     // Handle
					&buffer3,			  // Outgoing data
					9,             		   // Number of bytes to write
					&bytes_written,
					NULL);      // Number of bytes written)
					y_movespos-=1;
					y_movesneg+=1;
				}
			}
			else if (y_average > y_maxub && y_average < y_max)
			{
				if (y_movespos < 3) // default set to zero 
				{
					printf("TURN LEFT");
					printf("TURN LEFT");
					printf("TURN LEFT");
					WriteFile(comport,     // Handle
					&buffer4,			  // Outgoing data
					9,             		   // Number of bytes to write
					&bytes_written,
					NULL);      // Number of bytes written)
					y_movespos+=1;
					y_movesneg-=1;
				}
			}
			standbytimey = delayy + clock();
		}

	if (standbytimer <= clock())
		{
			if  (r_average > r_min && r_average < r_minlb )
			{
				if (r_movesneg < 2)
				{
					printf("Hand Close");
					WriteFile(comport,     // Handle
					&buffer6,			  // Outgoing data Close Hand
					9,             		   // Number of bytes to write
					&bytes_written,
					NULL);      // Number of bytes written)

					r_movespos-=1;
					r_movesneg+=1;

					printf("Hand Close");
					WriteFile(comport,     // Handle
					&buffer6,			  // Outgoing data Close Hand
					9,             		   // Number of bytes to write
					&bytes_written,
					NULL);      // Number of bytes written)
					printf("Hand Close");
					WriteFile(comport,     // Handle
					&buffer6,			  // Outgoing data Close Hand
					9,             		   // Number of bytes to write
					&bytes_written,
					NULL);      // Number of bytes written)
				}
			}
			else if (r_average > r_maxub && r_average < r_max)
			{
				if (r_movespos < 2) // default set to zero 
				{
					printf("Hand Open");
					WriteFile(comport,     // Handle
					&buffer5,			  // Outgoing data Open Hand
					9,             		   // Number of bytes to write
					&bytes_written,
					NULL);      // Number of bytes written)
					r_movesneg-=1;
					r_movespos+=1;
					printf("Hand Open");
					WriteFile(comport,     // Handle
					&buffer5,			  // Outgoing data Open Hand
					9,             		   // Number of bytes to write
					&bytes_written,
					NULL);      // Number of bytes written)
				}
			}
			standbytimer = delayr + clock();
		}

		// Highly efficient to move array one place to the right and purge the last value
		memmove(smooth_arm +1, smooth_arm, sizeof(smooth_arm) -sizeof( smooth_arm[ 0 ] ) ); 
		memmove(smooth_army +1, smooth_army, sizeof(smooth_army) -sizeof( smooth_army[ 0 ] ) );
        memmove(smooth_armr +1, smooth_armr, sizeof(smooth_armr) -sizeof( smooth_armr[ 0 ] ) );
		
		cvShowImage( "Camera", frame ); // Original stream with detected ball overlay
        cvShowImage( "HSV", hsv_frame); // Original stream in the HSV color space
        cvShowImage( "After Color Filtering", thresholded ); // The stream after color filtering
 
        cvReleaseMemStorage(&storage);
 
        //If ESC key pressed exit program
        char c = cvWaitKey(33);
		if( c == 27 ) break;
    }
     // Release the storage elements
     cvReleaseCapture( &capture );
     cvDestroyWindow( "mywindow" );
     return 0;
}


	

