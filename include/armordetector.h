#ifndef ARMORDETECTOR_H
#define ARMORDETECTOR_H

#include<opencv2/opencv.hpp>

struct couple
{
    int husband;
    int wife;
    couple(int i,int j){
        husband=i;
        wife=j;
    }
    void show(){
        std::cout<<"["<<husband<<","<<wife<<"]";
    }
};
struct rotatedCouple
{
    rotatedCouple() {}
    cv::RotatedRect rect1;
    cv::RotatedRect rect2;
};

class ArmorDetector
{
public:
    ArmorDetector();
    std::vector<cv::Point> findArmor(cv::Mat &src,std::vector<rotatedCouple> &rotRect,bool flag);
    void ROI(std::vector<rotatedCouple> &rotRect,cv::Mat &outPutArray);
};

#endif // ARMORDETECTOR_H
