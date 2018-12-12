#ifndef ARMORDETECTOR_H
#define ARMORDETECTOR_H

#include<opencv2/opencv.hpp>
#define showMessage(message,value) std::cout<<message<<";"<<value<<std::endl

static float minLength;
static float minWidth;
static float minRatio;
static float MaxDisX;
static float MinDisX;
static float MaxDisY;
static float MinDisY;
static float angleError;
static int BMAXH;
static int BMINH;
static int BMAXS;
static int BMINS;
static int BMAXV;
static int BMINV;

struct rotatedPair
{
    uint i=0;
    uint j=0;
    cv::RotatedRect rect1;
    cv::RotatedRect rect2;
    rotatedPair() {}
    void input(cv::RotatedRect rec1,cv::RotatedRect rec2,uint index1=0,uint index2=0)
    {
        i=index1;
        j=index2;
        rect1=rec1;
        rect2=rec2;
    }
    void showPair(){
        std::cout<<"["<<i<<","<<j<<"]"<<std::endl;
    }
};

class ArmorDetector
{
public:
    std::vector<float> rotRectAngle;
    std::vector<float> rotRectHeight;
    std::vector<float> rotRectWidth;
    std::vector<rotatedPair>armors;
    std::vector<cv::RotatedRect> rotArmors;
    std::vector<cv::RotatedRect> rotatedRects;
    std::vector<std::vector<cv::Point>> contours;
    ArmorDetector();
    void argsWrite();
    void argsRead();
    std::vector<cv::Point> findArmor(cv::Mat &src,bool flag);
    void ROI(cv::Mat &src);
};

#endif // ARMORDETECTOR_H
