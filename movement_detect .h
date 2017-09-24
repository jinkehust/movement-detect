#include "opencv2/objdetect/objdetect.hpp"  
#include "opencv2/highgui/highgui.hpp"  
#include "opencv2/imgproc/imgproc.hpp"  

#include <iostream>  
#include <stdio.h>  

using namespace std;  
using namespace cv;


Mat background;

      
int main()  
{  
          
    VideoCapture video(0);//定义VideoCapture类video  
    if (!video.isOpened())  //对video进行异常检测  
    {  
	cout << "video open error!" << endl;  
        return 0;  
    }  
	Mat frame;
	Mat lastframe;
	int flag = 0;

	while (1)
	{
		video >> frame;   //摄像头当前图片读给frame

		if (frame.empty())
			cout << "error" << endl;

		Mat GrayImage,GaussImage;
		cvtColor(frame, GrayImage, CV_BGR2GRAY);
		GaussianBlur(GrayImage, GaussImage, Size(17, 17), 0);
		
		//设置背景
		if (flag == 0)
		{
			background = GaussImage;
			flag = 1;
		}
		else {
			background = lastframe;
		}
		lastframe = GaussImage;

		//将background和frame做差
		Mat diff;
		absdiff(background, GrayImage, diff);
		//对差值图diff_thresh进行阈值化处理
		Mat diff_thresh;
		threshold(diff, diff_thresh, 100, 255, CV_THRESH_BINARY);
		//膨胀
		Mat kernel_dilate = getStructuringElement(MORPH_RECT, Size(15, 15));
		dilate(diff_thresh, diff_thresh, kernel_dilate);
		// 标记 
		vector<vector<Point>> contours;
		findContours(diff_thresh, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
		vector<Rect> boundRect(contours.size());
		for (int i = 0; i < contours.size(); i++)
		{
			/*
			boundRect[i]= boundingRect(contours[i]);
			Point center;
			int radius;
			center.x = cvRound((boundRect[i].x + boundRect[i].width * 2));
			center.y = cvRound((boundRect[i].y + boundRect[i].height * 2));
			radius = cvRound((boundRect[i].width + boundRect[i].height));
			circle(frame, center, radius, Scalar(0, 255, 0), 2);
			*/
			boundRect[i] = boundingRect(contours[i]);
			rectangle(frame, boundRect[i], Scalar(0, 255, 0), 2);//绘制正外接矩形  
		}

		imshow("video", frame);
		waitKey(10);
	}

	return 0;
}
