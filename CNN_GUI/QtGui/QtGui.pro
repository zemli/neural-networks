# ----------------------------------------------------
# This file is generated by the Qt Visual Studio Tools.
# ------------------------------------------------------

TEMPLATE = app
TARGET = QtGui
DESTDIR = ../Win32/Debug
QT += core widgets gui
CONFIG += debug
DEFINES += _SCL_SECURE_NO_WARNINGS WIN64 QT_DLL QT_WIDGETS_LIB
INCLUDEPATH += ../../../../../../../../opencv/build/include \
    ./GeneratedFiles \
    . \
    ./GeneratedFiles/Debug
LIBS += -L"../../../../../../../../opencv/build/x86/vc14/lib" \
    -lopencv_calib3d2413d \
    -lopencv_contrib2413d \
    -lopencv_core2413d \
    -lopencv_features2d2413d \
    -lopencv_flann2413d \
    -lopencv_gpu2413d \
    -lopencv_highgui2413d \
    -lopencv_imgproc2413d \
    -lopencv_legacy2413d \
    -lopencv_ml2413d \
    -lopencv_nonfree2413d \
    -lopencv_objdetect2413d \
    -lopencv_photo2413d \
    -lopencv_stitching2413d \
    -lopencv_ts2413d \
    -lopencv_video2413d \
    -lopencv_ocl2413d \
    -lopencv_superres2413d
DEPENDPATH += .
MOC_DIR += ./GeneratedFiles/debug
OBJECTS_DIR += debug
UI_DIR += ./GeneratedFiles
RCC_DIR += ./GeneratedFiles
include(QtGui.pri)