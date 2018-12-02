#include "armordetector.h"
#include<opencv2/opencv.hpp>
#include<iostream>

#define minLength 30
#define minWidth 10
#define minRatio 2
#define MaxDisX 6
#define MinDisX 0
#define MaxDisY 0.7
#define MinDisY 0
#define angleError 10
#define BMAXH 125
#define BMINH 70
#define BMAXS 210
#define BMINS 0
#define BMAXV 255
#define BMINV 180

#define RMAXH
#define RMINH
#define RMAXS
#define RMINS
#define RMAXV
#define RMINV

ArmorDetector::ArmorDetector(){}

std::vector<cv::Point> ArmorDetector::findArmor(cv::Mat &src,std::vector<rotatedCouple> &rotRect,bool flag)
{
    std::vector<int> rotRectAngle;
    std::vector<float> rotRectHeight;
    std::vector<float> rotRectWidth;
    std::vector<cv::RotatedRect> rotatedRects;
    std::vector<std::vector<cv::Point>> contours;
    cv::Mat dst;
    cv::pyrDown(src,src);
    //cv::imshow("src",src);
    cv::blur(src,src,cv::Size(3,3));
    cv::cvtColor(src,dst,CV_BGR2HSV);
    cv::inRange(dst,cv::Scalar(BMINH,BMINS,BMINV),cv::Scalar(BMAXH,BMAXS,BMAXV),dst);
    //cv::imshow("inRange",dst);
    cv::morphologyEx(dst,dst,cv::MORPH_OPEN,cv::Mat());
    //cv::imshow("OPEN",dst);
    cv::findContours(dst,contours,cv::RETR_EXTERNAL,cv::CHAIN_APPROX_NONE);

    /****get the points of all rotated rectangles****/
    for(uint i=0;i<contours.size();++i){           //secleting contours
        cv::RotatedRect rotatedRect;              //save a single contour interimly
        rotatedRect=cv::minAreaRect(contours[i]); //find minist rectangle
        cv::Point2f pt[4];                        //save four foint of a rotatedRectangle
        rotatedRect.points(pt);
        rotatedRects.push_back(rotatedRect);
        for(int i=0;i<4;i++){cv::line(src,pt[i],pt[(i+1)%4],cv::Scalar(0,255,0),2);}//draw rectangle
    }
    /****delete small contours****/
    for(uint i=0;i<rotatedRects.size();++i){
        if(rotatedRects[i].size.area()<minLength*minWidth){
            rotatedRects.erase(rotatedRects.begin()+i);
            --i;
        }
    }

    /****get the right angle,height,width****/
    for(uint i=0;i<rotatedRects.size();++i){
        if(rotatedRects[i].size.height>rotatedRects[i].size.width){
            rotRectAngle.push_back(-rotatedRects[i].angle);
            rotRectHeight.push_back(rotatedRects[i].size.height);
            rotRectWidth.push_back(rotatedRects[i].size.width);
        }
        else{
            rotRectAngle.push_back(-90-rotatedRects[i].angle);
            rotRectHeight.push_back(rotatedRects[i].size.width);
            rotRectWidth.push_back(rotatedRects[i].size.height);
        }
    }
    std::vector<couple>armors;           //save each two rectangle of an armor
    for(uint i=0;i<rotatedRects.size();++i){
        /**** compare ratio ****/
        if(rotRectHeight[i]/rotRectWidth[i]<minRatio)continue;
        for(uint j=i+1;j<rotatedRects.size();++j){
            if(rotRectHeight[j]/rotRectWidth[j]<minRatio)continue;
            /**** X localtion ****/
            if(abs(rotatedRects[i].center.x-rotatedRects[j].center.x)>
                    MaxDisX*(rotRectWidth[i]+rotRectWidth[j])){
                continue;
            }
            /**** Y localtion ****/
            if(abs(rotatedRects[i].center.y-rotatedRects[j].center.y)>
                    MaxDisY*(rotRectHeight[i]+rotRectHeight[j])){
                continue;
            }
            /**** angle ****/
            if(abs(rotRectAngle[i]-rotRectAngle[j])<angleError){
                armors.push_back(couple(i,j));
                if(flag){
                    rotatedCouple couple;
                    couple.rect1=rotatedRects[i];
                    couple.rect2=rotatedRects[j];
                    rotRect.push_back(couple);  //save the light bar location
                }
            }
        }
    }
    /****draw armor rect****/
    /****find the target center****/
    std::vector<cv::Point> target;
    for(uint i=0;i<armors.size();i++){
        /*cv::circle(src,
                   cv::Point((rotatedRects[armors[i].husband].center.x+rotatedRects[armors[i].wife].center.x)/2,
                (rotatedRects[armors[i].husband].center.y+rotatedRects[armors[i].wife].center.y)/2),
                abs(rotatedRects[armors[i].husband].center.x-rotatedRects[armors[i].wife].center.x)/2,
                cv::Scalar(0,255,0),2);
                */
        armors[i].show();
        cv::line(src,
                 cv::Point((rotatedRects[armors[i].husband].center.x+rotatedRects[armors[i].wife].center.x)/2,
                                 (rotatedRects[armors[i].husband].center.y+rotatedRects[armors[i].wife].center.y)/2),
                cv::Point((rotatedRects[armors[i].husband].center.x+rotatedRects[armors[i].wife].center.x)/2,
                                (rotatedRects[armors[i].husband].center.y+rotatedRects[armors[i].wife].center.y)/2),
                cv::Scalar(0,255,0),5);
        target.push_back(cv::Point((rotatedRects[armors[i].husband].center.x+rotatedRects[armors[i].wife].center.x)/2,
                (rotatedRects[armors[i].husband].center.y+rotatedRects[armors[i].wife].center.y)/2));
    }
    return target;
}

void ROI(std::vector<rotatedCouple> &rotRect,cv::Mat &outPutArray){
    for(int i=0;i<rotRect.size();++i){
        if(rotRect[i].rect1.center.x<rotRect[i].rect2.center.x){
            cv::RotatedRect number;
            cv::Point pt[4];
            pt[0].x=rotRect[i].rect1.center.x;
            //pt[0].y=rotRect[i].rect1.center.y-rotRect[i];
        }
    }
}
