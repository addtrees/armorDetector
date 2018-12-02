
#ifndef MINDVISIONCAMERA_MINDVISIONCAMERA_H
#define MINDVISIONCAMERA_MINDVISIONCAMERA_H
#include"CameraApi.h"
#include<opencv2/opencv.hpp>

class MindVisionCamera {
private:
    tSdkCameraDevInfo       tCameraEnumList;
    tSdkCameraCapbility     tCapability;      //设备描述信息
    tSdkImageResolution    tImageResolution;
    unsigned char           * g_pRgbBuffer;
public:
    int h_camera;

    MindVisionCamera();

    ~MindVisionCamera();
    void SetExposureTime( double          fExposureTime);
    bool ReadFrame(cv::Mat &frame);

};


#endif //MINDVISIONCAMERA_MINDVISIONCAMERA_H
