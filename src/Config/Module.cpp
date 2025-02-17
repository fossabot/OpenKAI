/*
 * Module.cpp
 *
 *  Created on: Nov 22, 2016
 *      Author: yankai
 */

#include "Module.h"

namespace kai
{

	Module::Module()
	{
	}

	Module::~Module()
	{
	}

	BASE *Module::createInstance(Kiss *pK)
	{
		IF_N(!pK);

		ADD_MODULE(_ThreadCtrl);
		ADD_MODULE(_SharedMem);
		ADD_MODULE(Destimator);
		ADD_MODULE(_Console);

#ifdef WITH_3D && USE_OPEN3D
		ADD_MODULE(_MeshStream);
		ADD_MODULE(_PCstream);
		ADD_MODULE(_PCframe);
		ADD_MODULE(_PClattice);
		ADD_MODULE(_PCfile);
		ADD_MODULE(_PCmerge);
		ADD_MODULE(_PCsend);
		ADD_MODULE(_PCrecv);
		ADD_MODULE(_PCtransform);
		ADD_MODULE(_PCcrop);
		ADD_MODULE(_PCremove);
		ADD_MODULE(_PCdownSample);
		ADD_MODULE(_PCregistCol);
		ADD_MODULE(_PCregistICP);
		ADD_MODULE(_PCregistGlobal);
#ifdef USE_GUI
		ADD_MODULE(_GeometryViewer);
#endif
#endif

#ifdef WITH_ACTUATOR
		ADD_MODULE(_ArduServo);
		ADD_MODULE(_ActuatorSync);
		ADD_MODULE(_Feetech);
		ADD_MODULE(_OrientalMotor);
		ADD_MODULE(_HYMCU_RS485);
		ADD_MODULE(_S6H4D);
		ADD_MODULE(_StepperGripper);
		ADD_MODULE(_SkydroidGimbal);
		ADD_MODULE(_ZLAC8015);
		ADD_MODULE(_ZLAC8015D);
		ADD_MODULE(_ZDmotor);
		ADD_MODULE(_DDSM);
#ifdef USE_XARM
		ADD_MODULE(_xArm);
#endif
#endif

#ifdef WITH_ARDUPILOT
		ADD_MODULE(_AP_base);
		ADD_MODULE(_AP_actuator);
		ADD_MODULE(_AP_move);
		ADD_MODULE(_AP_RTH);
		ADD_MODULE(_AP_relay);
		ADD_MODULE(_AP_servo);
		ADD_MODULE(_AP_takeoff);
		ADD_MODULE(_AP_goto);
		ADD_MODULE(_AP_link);
		ADD_MODULE(_AProver_drive);
#ifdef WITH_SWARM
		ADD_MODULE(_AP_swarm);
#endif
#ifdef WITH_SENSOR
		ADD_MODULE(_AP_distLidar);
#endif
#ifdef USE_OPENCV
		ADD_MODULE(_AP_avoid);
		ADD_MODULE(_AP_depthVision);
		ADD_MODULE(_AP_gcs);
		ADD_MODULE(_AP_land);
		ADD_MODULE(_AP_mission);
		ADD_MODULE(_AP_follow);
		ADD_MODULE(_AP_videoStream);
		ADD_MODULE(_AP_video);
		ADD_MODULE(_APcopter_photo);
#ifdef WITH_APP_ROBOTARM
		ADD_MODULE(_AProver_picking);
#endif
#endif // opencv
#ifdef WITH_NAVIGATION
#ifdef USE_REALSENSE
		ADD_MODULE(_AP_GPS);
		ADD_MODULE(_AP_visionEstimate);
#endif
#endif
#endif

#ifdef WITH_AUTOPILOT_DRIVE
		ADD_MODULE(_Drive);
#endif

#ifdef WITH_APP_3DSCAN && WITH_3D && USE_OPEN3D
		ADD_MODULE(_3DScanCalibCam);
		ADD_MODULE(_3DScanCalibOfs);
		ADD_MODULE(_PCscan);
		ADD_MODULE(_PCcalib);
#endif

#ifdef WITH_APP_CAMCALIB
		ADD_MODULE(_CamCalib);
#endif

#ifdef WITH_APP_DRONEBOX
		ADD_MODULE(_DroneBox);
		ADD_MODULE(_DroneBoxJSON);
		ADD_MODULE(_AP_droneBoxJSON);
#endif

#ifdef WITH_APP_GSV
		ADD_MODULE(_GSVdetect);
		ADD_MODULE(_GSValarm);
#endif

#ifdef WITH_APP_LIVOXSCANNER && USE_LIVOX && WITH_3D && USE_OPEN3D
		ADD_MODULE(_LivoxAutoScan);
		ADD_MODULE(_LivoxScanner);
#endif

#ifdef WITH_APP_MEASUREMENT
		ADD_MODULE(_ARmeasure);
		ADD_MODULE(_ARmeasureVertex);
		ADD_MODULE(_ARmeasureFree);
		ADD_MODULE(_ARmeasureCalibCam);
		ADD_MODULE(_ARmeasureCalibDofs);
		ADD_MODULE(_RaspiWSbattery);
#endif

//#ifdef WITH_APP_ROPEWAYSCAN && USE_VZENSE && USE_LIVOX && WITH_3D && USE_OPEN3D
#ifdef WITH_APP_ROPEWAYSCAN
		ADD_MODULE(_RopewayScan);
//		ADD_MODULE(_RopewayScanVz);
//		ADD_MODULE(_RopewayScanLivox);
#endif

#ifdef WITH_APP_ROBOTARM
		ADD_MODULE(_Sequencer);
#ifdef USE_OPENCV
		ADD_MODULE(_PickingArm);
#endif
#endif

#ifdef WITH_APP_ROVER
		ADD_MODULE(_PWMrover);
		ADD_MODULE(_SbusRover);
		ADD_MODULE(_AProver_BR);
		ADD_MODULE(_AProver_BRfollow);
		ADD_MODULE(_AProver_KU);
		ADD_MODULE(_AProver_KUfollowTag);
		ADD_MODULE(_AProver_followTag);
		ADD_MODULE(_AProver_tag);
		ADD_MODULE(_AProver_WB);
		ADD_MODULE(_AProver_WBnav);
		ADD_MODULE(_UTprArmL);
#ifdef USE_OPENCV
#endif
#endif

#ifdef WITH_APP_SWARMSEARCH
		ADD_MODULE(_SwarmSearchCtrl);
		ADD_MODULE(_SwarmSearchCtrlUI);
#endif

#ifdef WITH_APP_VZSCAN && USE_VZENSE && WITH_3D && USE_OPEN3D &&USE_GUI
		ADD_MODULE(_VzScan);
		ADD_MODULE(_VzScanAuto);
		ADD_MODULE(_VzScanCalib);
#endif

#ifdef WITH_COMPUTE
#ifdef USE_OPENCL
		ADD_MODULE(clBase);
#endif
#endif

#ifdef WITH_CONTROL
		ADD_MODULE(PID);
#endif

#ifdef WITH_FILE
#ifdef USE_OPENCV
		ADD_MODULE(_PhotoTake);
		ADD_MODULE(_GPhotoTake);
		ADD_MODULE(_GDimgUploader);
		ADD_MODULE(_BBoxCutOut);
		ADD_MODULE(_CutOut);
		ADD_MODULE(_FrameCutOut);
#endif
#endif

#ifdef WITH_DETECTOR
#ifdef USE_OPENCV
		ADD_MODULE(_DNNclassifier);
		ADD_MODULE(_YOLOv3);
		ADD_MODULE(_DNNtext);
		ADD_MODULE(_DepthSegment);
		ADD_MODULE(_IRLock);
		ADD_MODULE(_OpenPose);
		ADD_MODULE(_HandKey);
		ADD_MODULE(_Lane);
		ADD_MODULE(_Line);
		ADD_MODULE(_SSD);
		ADD_MODULE(_Thermal);
#ifdef USE_OPENCV_CONTRIB
		ADD_MODULE(_ArUco);
		ADD_MODULE(_MotionDetector);
		ADD_MODULE(_SingleTracker);
#endif
#ifdef USE_CUDA
		ADD_MODULE(_Bullseye);
		ADD_MODULE(_Cascade);
#endif
#ifdef USE_CHILITAGS
		ADD_MODULE(_Chilitags);
#endif
#endif //USE_OPENCV
#endif

#ifdef WITH_DNN
#ifdef USE_OPENCV
#ifdef USE_JETSON_INFERENCE
		//	ADD_MODULE(_ImageNet);
		ADD_MODULE(_DetectNet);
#endif
#ifdef USE_TF-LITE
		ADD_MODULE(_TFmobileNet);
#endif
#endif
#endif

#ifdef WITH_FILTER
#endif

#ifdef WITH_GCS
#endif

#ifdef WITH_IO
		ADD_MODULE(_ADIO_EBYTE);
		ADD_MODULE(_SerialPort);
		ADD_MODULE(_TCPserver);
		ADD_MODULE(_TCPclient);
		ADD_MODULE(_UDP);
		ADD_MODULE(_WebSocket);
#endif

#ifdef WITH_NAVIGATION
		ADD_MODULE(_RTCM3);
		ADD_MODULE(_GPS);
		ADD_MODULE(GeoGrid);
#ifdef USE_REALSENSE
		ADD_MODULE(_RStracking);
#endif
#endif

#ifdef WITH_NET
		ADD_MODULE(_Curl);
#endif

#ifdef WITH_PROTOCOL
		ADD_MODULE(_Canbus);
		ADD_MODULE(_Mavlink);
		ADD_MODULE(_Modbus);
		ADD_MODULE(_SBus);
		ADD_MODULE(_PWMio);
		ADD_MODULE(_ProtocolBase);
		ADD_MODULE(_Xbee);
#endif

#ifdef WITH_SCIENCE
#ifdef USE_MATHGL
		//		ADD_MODULE(_Solver);
		ADD_MODULE(_SolverBase);
		ADD_MODULE(_FourierSeries);
#endif
#endif

#ifdef WITH_SENSOR
		ADD_MODULE(_BenewakeTF);
		ADD_MODULE(_TOFsense);
		ADD_MODULE(_LeddarVu);
#ifdef USE_LIVOX
		ADD_MODULE(_Livox);
		ADD_MODULE(LivoxLidar);
#endif
#ifdef USE_LIVOX2
		ADD_MODULE(_Livox2);
		ADD_MODULE(LivoxLidar2);
#endif
#endif

#ifdef WITH_SLAM
#ifdef USE_OPENCV
#ifdef USE_ORB_SLAM
		ADD_MODULE(_ORB_SLAM);
#endif
#endif
#endif

#ifdef WITH_STATE
		ADD_MODULE(_StateControl);
#endif

#ifdef WITH_SWARM
		ADD_MODULE(_SwarmBase);
		ADD_MODULE(_SwarmSearch);
#endif

#ifdef WITH_UI
#ifdef USE_OPENCV
		ADD_MODULE(_GstOutput);
		ADD_MODULE(_WindowCV);
#endif
#ifdef USE_GUI
#endif
#endif

#ifdef WITH_UNIVERSE
		ADD_MODULE(_Object);
		ADD_MODULE(_Universe);
#endif

#ifdef WITH_VISION
#ifdef USE_REALSENSE
		ADD_MODULE(_RealSense);
#endif
#ifdef USE_VZENSE
		ADD_MODULE(_Vzense);
#endif
#ifdef USE_XDYNAMICS
		ADD_MODULE(_XDynamics);
#endif
#ifdef USE_OPENCV
		ADD_MODULE(_Camera);
		ADD_MODULE(_Contrast);
		ADD_MODULE(_DepthProj);
		ADD_MODULE(_Crop);
		ADD_MODULE(_Depth2Gray);
		ADD_MODULE(_DepthShow);
		ADD_MODULE(_Erode);
		ADD_MODULE(_GPhoto);
		ADD_MODULE(_GStreamer);
		ADD_MODULE(_Grayscale);
		ADD_MODULE(_HistEqualize);
		ADD_MODULE(_Invert);
		ADD_MODULE(_InRange);
		ADD_MODULE(_ImgFile);
		ADD_MODULE(_Mask);
		ADD_MODULE(_Morphology);
		ADD_MODULE(_Resize);
		ADD_MODULE(_Remap);
		ADD_MODULE(_Rotate);
		ADD_MODULE(_SharedMemImg);
		ADD_MODULE(_Threshold);
		ADD_MODULE(_VideoFile);
#ifdef USE_INFIRAY
		ADD_MODULE(_InfiRay);
#endif
#ifdef USE_CUDA
		ADD_MODULE(_DenseFlow);
#endif
#endif	//opencv
#endif	//vision

		return NULL;
	}

	template <typename T>
	BASE *Module::createInst(Kiss *pKiss)
	{
		IF_N(!pKiss);

		T *pInst = new T();
		return pInst;
	}

}
