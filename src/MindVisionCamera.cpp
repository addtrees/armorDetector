//
// Created by aver on 18-11-20.
//

#include <CameraDefine.h>
#include "MindVisionCamera.h"
MindVisionCamera::MindVisionCamera(){
    int camera_counts=1;
    CameraSdkInit(1);
    int status= CameraEnumerateDevice(&tCameraEnumList,&camera_counts);
    if(status){
        std::cout<<"camera enum fail,error code="<<status<<std::endl;
        exit(-1);
    }
    if(!camera_counts){
        std::cout<<"camera_disconnect"<<std::endl;
        exit(-2);
    }
    status=CameraInit(&tCameraEnumList,-1,-1,&h_camera);
    if(status){
        std::cout<<"camera init fail"<<std::endl;
        exit(-3);
    }
    CameraGetCapability(h_camera,&tCapability);
    g_pRgbBuffer = (unsigned char*)malloc(tCapability.sResolutionRange.iHeightMax*tCapability.sResolutionRange.iWidthMax*3);
    CameraGetImageResolution(h_camera,&tImageResolution);
    tImageResolution.iIndex=0xff;
    tImageResolution.iWidth=640;
    tImageResolution.iHeight=480;
    tImageResolution.iWidthFOV=640;
    tImageResolution.iHeightFOV=480;
    CameraSetImageResolution(h_camera,&tImageResolution);
    CameraPlay(h_camera);
    CameraSetIspOutFormat(h_camera,CAMERA_MEDIA_TYPE_BGR8);
    CameraSetAeState(h_camera,FALSE);
    CameraSetFrameSpeed(h_camera,1);
}
MindVisionCamera::~MindVisionCamera(){
    CameraUnInit(h_camera);
    free(g_pRgbBuffer);
}
void MindVisionCamera::SetExposureTime( double          ExposureTime_ms){
    CameraSetExposureTime(h_camera,ExposureTime_ms*1000);
}
bool MindVisionCamera::ReadFrame(cv::Mat &frame) {
    IplImage *iplImage = NULL;
    tSdkFrameHead           sFrameInfo;
    BYTE*			        pbyBuffer;
    if(CameraGetImageBuffer(h_camera,&sFrameInfo,&pbyBuffer,1000) == CAMERA_STATUS_SUCCESS) {
        CameraImageProcess(h_camera, pbyBuffer, g_pRgbBuffer, &sFrameInfo);
        if (iplImage) {
            cvReleaseImageHeader(&iplImage);
        }
        iplImage = cvCreateImageHeader(cvSize(sFrameInfo.iWidth, sFrameInfo.iHeight), IPL_DEPTH_8U, 3);
        cvSetData(iplImage, g_pRgbBuffer, sFrameInfo.iWidth * 3);

        frame = cv::cvarrToMat(iplImage);
        CameraReleaseImageBuffer(h_camera, pbyBuffer);
	return true;
    }

}
