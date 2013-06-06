#include<cv.h>
#include<highgui.h>
#include<cxcore.h>
//#include<stdio.h>
using namespace cv;
#define RAD 50
#define RAD_EPS 15
#define HIGH 80
#define LOW 70

int main()
{
	cvNamedWindow("Vid",CV_WINDOW_AUTOSIZE);
	CvCapture* detect=cvCreateCameraCapture(CV_CAP_ANY);
	if(detect==NULL) return -1;
	
	cvSetCaptureProperty(detect,CV_CAP_PROP_FRAME_HEIGHT,1280);
	cvSetCaptureProperty(detect,CV_CAP_PROP_FRAME_WIDTH,1920);
	
	CvArr* frame,*final,*copy,*ans;
	char c;
	CvMemStorage* str=cvCreateMemStorage(0);
	CvSeq* circ;
	frame=cvQueryFrame(detect);
	copy=cvCreateImage(cvGetSize(frame),IPL_DEPTH_8U,3);
	final=cvCreateImage(cvGetSize(frame),IPL_DEPTH_8U,1);
	ans=cvCreateImage(cvGetSize(frame),IPL_DEPTH_8U,1);
	while(1)
	{
		
		if(!frame)break;
	
		
		cvCvtColor(frame,copy,CV_BGR2HSV);
		
		cvInRangeS(copy,cvScalar(20,100,100),cvScalar(35,255,255),final);
		
		cvDilate(final,final,NULL,1);
			
		
		circ=cvHoughCircles(final,str,CV_HOUGH_GRADIENT,2,32.0,HIGH,LOW);
		
		for( int i = 0; i < circ->total; i++ ) 
		{
    		float* p = (float*) cvGetSeqElem( circ, i );
    		CvPoint pt = cvPoint( cvRound( p[0] ), cvRound( p[1] ) );
    		cvCircle(ans,pt,cvRound( p[2] ),CV_RGB(255,255,255),4);
		}
		
		cvShowImage("Vid",ans);
		cvSet(ans,cvScalar(0,0,0));
		
		
		
		c=cvWaitKey(20);
		
	//	cvSaveImage("p.jpg",final);
		if(c==27)break;
	//	for(int i=0;i<10;i++)cvQueryFrame(detect);
		frame=cvQueryFrame(detect);
	}	
	
	cvDestroyWindow("Vid");
	cvReleaseCapture(&detect);
	
	
	return 0;
}
