#include<opencv2/opencv.hpp>
#include<iostream>
#include"armordetector.h"
using namespace std;
ArmorDetector::ArmorDetector(){}

/*****************************************************************
 * @brief find the armor center base on the position of the lightbar
 * @param src:the source image
 * @param &rotRect:save the armor's number rectangle
 * @param flag :enable to get the armor numbers
 * @return :position of armor
 ****************************************************************/
 void ArmorDetector::argsWrite() {
    cv::FileStorage fs;
    fs.open("../files/args.yaml",cv::FileStorage::WRITE);
    fs<<"minLength"<<30;
    fs<<"minWidth"<<10;
    fs<<"minRatio"<<2;
    fs<<"MaxDisX"<<6;
    fs<<"MinDisX"<<0;
    fs<<"MaxDisY"<<0.7;
    fs<<"MinDisY"<<0;
    fs<<"angleError"<<10;
    fs<<"BMAXH"<<125;
    fs<<"BMINH"<<70;
    fs<<"BMAXS"<<210;
    fs<<"BMINS"<<0;
    fs<<"BMAXV"<<255;
    fs<<"BMINV"<<180;
    fs.release();
 }
void ArmorDetector::argsRead() {
    int maxh=0;
    cv::FileStorage fs;
    fs.open("../files/args.yaml",cv::FileStorage::READ);
    fs["minLength"]>>minLength;
    fs["minWidth"]>>minWidth;
    fs["minRatio"]>>minRatio;
    fs["MaxDisX"]>>MaxDisX;
    fs["MinDisX"]>>MinDisX;
    fs["MaxDisY"]>>MaxDisY;
    fs["MinDisY"]>>MinDisY;
    fs["angleError"]>>angleError;
    fs["BMAXH"]>>BMAXH;
    fs["BMINH"]>>BMINH;
    fs["BMAXS"]>>BMAXS;
    fs["BMINS"]>>BMINS;
    fs["BMAXV"]>>BMAXV;
    fs["BMINV"]>>BMINV;
    showMessage("minLength",minLength);
    showMessage("minWidth",minWidth);
    showMessage("MaxDisX",MaxDisX);
    showMessage("MinDisX",MinDisX);
    showMessage("MinDisY",MinDisY);
    showMessage("MinDisY",MinDisY);
    showMessage("angleError",angleError);
    showMessage("BMAXH",BMAXH);
    showMessage("BMINH",BMINH);
    showMessage("BMAXS",BMAXS);
    showMessage("BMINS",BMINS);
    showMessage("BMAXV",BMAXV);
    showMessage("BMINV",BMINV);
    fs.release();
}

void on_Trackbar(int,char*)
{

}
std::vector<cv::Point> ArmorDetector::findArmor(cv::Mat &src,bool flag)
{
    cv::Mat dst;

    cv::blur(src,src,cv::Size(3,3));
    cv::cvtColor(src,dst,CV_BGR2HSV);
    cv::inRange(dst,cv::Scalar(BMINH,BMINS,BMINV),cv::Scalar(BMAXH,BMAXS,BMAXV),dst);
    cv::morphologyEx(dst,dst,cv::MORPH_OPEN,cv::Mat());
    cv::findContours(dst,contours,cv::RETR_EXTERNAL,cv::CHAIN_APPROX_NONE);
    //cv::drawContours(src,contours,0,cv::Scalar(0,255,0),3);
    /****get the points of all rotated rectangles****/
    for(uint i=0;i<contours.size();++i){
        cv::RotatedRect rotatedRect;              //save a single contour interimly
        rotatedRect=cv::minAreaRect(contours[i]); //find minist rectangle
        cv::Point2f pt[4];
        rotatedRect.points(pt);                   //get four foints of a rotated Rectangle
        rotatedRects.push_back(rotatedRect);
        for(int i=0;i<4;i++){cv::line(src,pt[i],pt[(i+1)%4],cv::Scalar(0,255,0),2);}//draw rectangle
        cout<<pt<<endl;
    }
    cv::imshow("src",src);
    /****didn't using contours start here****/
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
    //save each two rectangle of an armor
    for(uint i=0;i<rotatedRects.size();++i){
        /**** compare ratio ****/
        if(rotRectHeight[i]/rotRectWidth[i]<minRatio)continue;
        for(uint j=i+1;j<rotatedRects.size();++j){
            if(rotRectHeight[j]/rotRectWidth[j]<minRatio)continue;
            /**** X localtion ****/
            if(std::abs(rotatedRects[i].center.x-rotatedRects[j].center.x)>
                    MaxDisX*(rotRectWidth[i]+rotRectWidth[j])){
                continue;
            }
            /**** Y localtion ****/
            if(std::abs(rotatedRects[i].center.y-rotatedRects[j].center.y)>
                    MaxDisY*(rotRectHeight[i]+rotRectHeight[j])){
                continue;
            }
            /**** angle ****/
            if(std::abs(rotRectAngle[i]-rotRectAngle[j])<angleError){
                rotatedPair armor;
                armor.input(rotatedRects[i],rotatedRects[j],i,j);
                armors.push_back(armor);
            }
        }
    }
    //std::cout<<armors.size()<<std::endl;
    /****find the target center and draw point****/
    std::vector<cv::Point> target;
    for(uint i=0;i<armors.size();i++){
        armors[i].showPair();
        cv::Point targetCenter((armors[i].rect1.center.x+armors[i].rect2.center.x)/2,
                               (armors[i].rect2.center.y+armors[i].rect2.center.y)/2);
        //cv::circle(src,targetCenter,3,cv::Scalar(0,0,255),3);
        target.push_back(targetCenter);
    }
    return target;
}

void ArmorDetector::ROI(cv::Mat &src){
    for(uint i=0;i<armors.size();++i){
        cv::RotatedRect armorNum;
        cv::Point2f pt[4];
        cv::Point2f dst[4];
        if(armors[i].rect1.center.x<armors[i].rect2.center.x){
            pt[0].x=armors[i].rect1.center.x;
            pt[0].y=armors[i].rect1.center.y+rotRectHeight[armors[i].i];
            pt[1].x=armors[i].rect1.center.x;
            pt[1].y=armors[i].rect1.center.y-rotRectHeight[armors[i].i];
            pt[2].x=armors[i].rect2.center.x;
            pt[2].y=armors[i].rect2.center.y-rotRectHeight[armors[i].j];
            pt[3].x=armors[i].rect2.center.x;
            pt[3].y=armors[i].rect2.center.y+rotRectHeight[armors[i].j];
        }
        else if(armors[i].rect1.center.x>armors[i].rect2.center.x){
            pt[0].x=armors[i].rect2.center.x;
            pt[0].y=armors[i].rect2.center.y+rotRectHeight[armors[i].i];
            pt[1].x=armors[i].rect2.center.x;
            pt[1].y=armors[i].rect2.center.y-rotRectHeight[armors[i].i];
            pt[2].x=armors[i].rect1.center.x;
            pt[2].y=armors[i].rect1.center.y-rotRectHeight[armors[i].j];
            pt[3].x=armors[i].rect1.center.x;
            pt[3].y=armors[i].rect1.center.y+rotRectHeight[armors[i].j];
        }
        dst[0].x=0;
        dst[0].y=1.25*(rotRectHeight[armors[i].i]+rotRectHeight[armors[i].j]);
        dst[1].x=0;
        dst[1].y=0;
        dst[2].x=std::abs((armors[i].rect1.center.x-armors[i].rect2.center.x));
        dst[2].y=0;
        dst[3].x=std::abs((armors[i].rect1.center.x-armors[i].rect2.center.x));
        dst[3].y=1.25*(rotRectHeight[armors[i].i]+rotRectHeight[armors[i].j]);

        cv::Mat warp_mat=cv::getPerspectiveTransform(pt,dst);
        cv::Mat dstImage(std::abs((armors[i].rect1.center.x-armors[i].rect2.center.x)),
                         1.35*(rotRectHeight[armors[i].i]+rotRectHeight[armors[i].j]),CV_8UC4);
        cv::warpPerspective(src,dstImage,warp_mat,dstImage.size());
        cv::resize(dstImage,dstImage,cv::Size(36,28));
        dstImage=10*dstImage;
        cv::namedWindow("dstImage",cv::WINDOW_NORMAL);
        cv::imshow("dstImage",dstImage);
        for(int i=0;i<4;++i){
            std::cout<<pt[i]<<std::endl;
        }
        rotArmors.push_back(armorNum);
        for(int i=0;i<4;++i){
            cv::line(src,static_cast<cv::Point>(pt[i]),
                     static_cast<cv::Point>(pt[(i+1)%4]),cv::Scalar(0,0,255),2);
        }
    }
}
