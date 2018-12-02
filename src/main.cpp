#include <iostream>
#include<opencv2/opencv.hpp>
#include<math.h>
#include"armordetector.h"
#include"MindVisionCamera.h"
int main()
{
   // cv::VideoCapture cap("E:/outClass/roboStudio/video/WIN_20181112_14_52_44_Pro.mp4");
    cv::Mat frame;
    cv::Mat dst;
    ArmorDetector armor;
 MindVisionCamera camera;
double run_time=0;
int count=0;
    camera.SetExposureTime(10);
    while(true){
	count++;
	double start = static_cast<double>(cv::getTickCount());
	camera.ReadFrame(frame);
      	double time = ((double)cv::getTickCount() - start) / cv::getTickFrequency();
        std::cout << "read time:" << time *1000<< "ms" << std::endl;  
        std::vector<rotatedCouple> couples;
        bool flag=!false;
    /****search armors****/
        armor.findArmor(frame,couples,flag);
	
        
        double Totaltime = ((double)cv::getTickCount() - start) / cv::getTickFrequency();
	double ptime = Totaltime-time;
	std::cout << "process time:" << ptime *1000<< "ms" << std::endl;
        std::cout << "total time:" << Totaltime *1000<< "ms" << std::endl;
	run_time+=Totaltime;
	std::cout << "count times:"<<count<<"  averTime="<<run_time/count*1000<<"ms"<<std::endl<<std::endl;
        cv::imshow("dst",frame);
        if(cv::waitKey(1)=='q'){
            std::cout<<"quit to display!"<<std::endl;
            break;
        }
    }
    return 0;
}
