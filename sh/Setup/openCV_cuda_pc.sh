#----------------------------------------------------
# (Optional) OpenCV
git clone --branch 4.8.1 --depth 1 https://github.com/opencv/opencv.git
git clone --branch 4.8.1 --depth 1 https://github.com/opencv/opencv_contrib.git
cd opencv
mkdir build
cd build

# OpenCV without CUDA
cmake -DBUILD_CUDA_STUBS=OFF \
      -DBUILD_DOCS=OFF \
      -DBUILD_EXAMPLES=OFF \
      -DBUILD_IPP_IW=ON \
      -DBUILD_ITT=ON \
      -DBUILD_JASPER=OFF \
      -DBUILD_JAVA=OFF \
      -DBUILD_JPEG=OFF \
      -DBUILD_OPENEXR=OFF \
      -DBUILD_PACKAGE=ON \
      -DBUILD_PERF_TESTS=OFF \
      -DBUILD_PNG=OFF \
      -DBUILD_PROTOBUF=ON \
      -DBUILD_SHARED_LIBS=ON \
      -DBUILD_TBB=OFF \
      -DBUILD_TESTS=OFF \
      -DBUILD_TIFF=OFF \
      -DBUILD_WEBP=OFF \
      -DBUILD_WITH_DEBUG_INFO=OFF \
      -DBUILD_WITH_DYNAMIC_IPP=OFF \
      -DBUILD_ZLIB=OFF \
      -DBUILD_opencv_alphamat=ON \
      -DBUILD_opencv_apps=ON \
      -DBUILD_opencv_aruco=ON \
      -DBUILD_opencv_barcode=ON \
      -DBUILD_opencv_bgsegm=ON \
      -DBUILD_opencv_bioinspired=ON \
      -DBUILD_opencv_calib3d=ON \
      -DBUILD_opencv_ccalib=ON \
      -DBUILD_opencv_core=ON \
      -DBUILD_opencv_cudev=ON \
      -DBUILD_opencv_datasets=ON \
      -DBUILD_opencv_dnn=ON \
      -DBUILD_opencv_dnn_objdetect=ON \
      -DBUILD_opencv_dnn_superres=ON \
      -DBUILD_opencv_dpm=ON \
      -DBUILD_opencv_face=ON \
      -DBUILD_opencv_features2d=ON \
      -DBUILD_opencv_flann=ON \
      -DBUILD_opencv_fuzzy=ON \
      -DBUILD_opencv_gapi=ON \
      -DBUILD_opencv_hdf=OFF \
      -DBUILD_opencv_hfs=ON \
      -DBUILD_opencv_highgui=ON \
      -DBUILD_opencv_img_hash=ON \
      -DBUILD_opencv_imgcodecs=ON \
      -DBUILD_opencv_imgproc=ON \
      -DBUILD_opencv_java_bindings_generator=OFF \
      -DBUILD_opencv_js=OFF \
      -DBUILD_opencv_js_bindings_generator=OFF \
      -DBUILD_opencv_line_descriptor=ON \
      -DBUILD_opencv_ml=ON \
      -DBUILD_opencv_objc_bindings_generator=OFF \
      -DBUILD_opencv_objdetect=ON \
      -DBUILD_opencv_optflow=ON \
      -DBUILD_opencv_phase_unwrapping=ON \
      -DBUILD_opencv_photo=ON \
      -DBUILD_opencv_plot=ON \
      -DBUILD_opencv_python3=OFF \
      -DBUILD_opencv_python_bindings_generator=OFF \
      -DBUILD_opencv_python_tests=OFF \
      -DBUILD_opencv_reg=ON \
      -DBUILD_opencv_rgbd=ON \
      -DBUILD_opencv_saliency=ON \
      -DBUILD_opencv_sfm=ON \
      -DBUILD_opencv_shape=ON \
      -DBUILD_opencv_stereo=ON \
      -DBUILD_opencv_stitching=ON \
      -DBUILD_opencv_structured_light=ON \
      -DBUILD_opencv_superres=ON \
      -DBUILD_opencv_surface_matching=ON \
      -DBUILD_opencv_text=ON \
      -DBUILD_opencv_tracking=ON \
      -DBUILD_opencv_ts=ON \
      -DBUILD_opencv_video=ON \
      -DBUILD_opencv_videoio=ON \
      -DBUILD_opencv_videostab=ON \
      -DBUILD_opencv_world=OFF \
      -DBUILD_opencv_xfeatures2d=ON \
      -DBUILD_opencv_ximgproc=ON \
      -DBUILD_opencv_xobjdetect=ON \
      -DBUILD_opencv_xphoto=ON \
      -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_CONFIGURATION_TYPES=Release \
      -DCMAKE_INSTALL_PREFIX=/usr/local \
      -DENABLE_BUILD_HARDENING=OFF \
      -DENABLE_FAST_MATH=ON \
      -DENABLE_PRECOMPILED_HEADERS=OFF \
      -DOPENCV_ENABLE_NONFREE=OFF \
      -DOPENCV_EXTRA_MODULES_PATH=../../opencv_contrib/modules \
      -DOPENCV_DNN_OPENVINO=OFF \
      -DOPENCV_GAPI_WITH_OPENVINO=OFF \
      -DOPJ_USE_THREAD=ON \
      -DWITH_1394=OFF \
      -DWITH_ARAVIS=OFF \
      -DWITH_CAROTENE=ON \
      -DWITH_CLP=OFF \
      -DWITH_CUDA=OFF \
      -DWITH_EIGEN=ON \
      -DWITH_FFMPEG=ON \
      -DWITH_FREETYPE=ON \
      -DWITH_GDAL=OFF \
      -DWITH_GDCM=OFF \
      -DWITH_GPHOTO2=ON \
      -DWITH_GSTREAMER=ON \
      -DWITH_GTK=ON \
      -DWITH_GTK_2_X=OFF \
      -DWITH_HALIDE=OFF \
      -DWITH_HPX=OFF \
      -DWITH_INF_ENGINE=ON \
      -DWITH_IPP=ON \
      -DWITH_ITT=ON \
      -DWITH_JASPER=ON \
      -DWITH_JPEG=ON \
      -DWITH_LAPACK=ON \
      -DWITH_LIBREALSENSE=OFF \
      -DWITH_MATLAB=OFF \
      -DWITH_MFX=OFF \
      -DWITH_OPENCL=ON \
      -DWITH_OPENCLAMDBLAS=ON \
      -DWITH_OPENCLAMDFFT=ON \
      -DWITH_OPENCL_SVM=OFF \
      -DWITH_OPENEXR=ON \
      -DWITH_OPENGL=ON \
      -DWITH_OPENMP=OFF \
      -DWITH_OPENNI=OFF \
      -DWITH_OPENNI2=OFF \
      -DWITH_OPENVINO=OFF \
      -DWITH_OPENVX=OFF \
      -DWITH_PNG=ON \
      -DWITH_PROTOBUF=ON \
      -DWITH_PTHREADS_PF=ON \
      -DWITH_PVAPI=OFF \
      -DWITH_QT=OFF \
      -DWITH_TBB=ON \
      -DWITH_TIFF=ON \
      -DWITH_V4L=ON \
      -DWITH_VA=OFF \
      -DWITH_VA_INTEL=OFF \
      -DWITH_VTK=ON \
      -DWITH_WEBP=ON \
      -DWITH_XIMEA=OFF \
      -DWITH_XINE=OFF \
      ../

make all -j$(nproc)
sudo make install
