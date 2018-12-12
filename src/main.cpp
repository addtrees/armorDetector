#include <iostream>
#include<opencv2/opencv.hpp>
#include"armordetector.h"
#include"MindVisionCamera.h"

using namespace std;
int main()
{
    //cv::VideoCapture cap("/home/hero/Videos/WIN_20181112_14_52_44_Pro.mp4");
    cv::Mat frame;
    cv::Mat dst;
    MindVisionCamera cap;
    ArmorDetector args;
    args.argsRead();
    cv::VideoWriter videoSave;
    cap.ReadFrame(frame);
    if(frame.empty()){
        cout<<"cannot read from Camera"<<endl;
        return -1;
    }
    cv::Size frameSize=cv::Size(frame.rows,frame.cols);
    string videoPath="../files/armor.avi";
    videoSave.open(videoPath,CV_FOURCC('M','P','4','2'),60,frameSize);
    while(cap.ReadFrame(frame)){
        videoSave.write(frame);
       double start = static_cast<double>(cv::getTickCount());
       ArmorDetector armor;
        if(frame.empty()){
            std::cout<<"frame empty!"<<std::endl;
            break;
        }
        bool flag=!false;
        armor.findArmor(frame,flag);
        armor.ROI(frame);
        double time = ((double)cv::getTickCount() - start) / cv::getTickFrequency();
        std::cout << "time:" << time << "s" << std::endl;
        cv::imshow("frame",frame*10);
        if(cv::waitKey(15)=='q'){
            std::cout<<"quit to display!"<<std::endl;
            break;
        }
    }
    return 0;
}
