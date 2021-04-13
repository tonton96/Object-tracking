QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    objecttracking.cpp

HEADERS += \
    mainwindow.h \
    objecttracking.h

FORMS += \
    mainwindow.ui

INCLUDEPATH += C:\opencv\opencv\release\install\include
LIBS += C:\opencv\opencv\release\install\x64\mingw\bin\libopencv_calib3d452.dll
LIBS += C:\opencv\opencv\release\install\x64\mingw\bin\libopencv_core452.dll
LIBS += C:\opencv\opencv\release\\install\x64\mingw\bin\libopencv_dnn452.dll
LIBS += C:\opencv\opencv\release\install\x64\mingw\bin\libopencv_features2d452.dll
LIBS += C:\opencv\opencv\release\install\x64\mingw\bin\libopencv_flann452.dll
LIBS += C:\opencv\opencv\release\install\x64\mingw\bin\libopencv_gapi452.dll
LIBS += C:\opencv\opencv\release\install\x64\mingw\bin\libopencv_highgui452.dll
LIBS += C:\opencv\opencv\release\install\x64\mingw\bin\libopencv_imgcodecs452.dll
LIBS += C:\opencv\opencv\release\\install\x64\mingw\bin\libopencv_imgproc452.dll
LIBS += C:\opencv\opencv\release\install\x64\mingw\bin\libopencv_ml452.dll
LIBS += C:\opencv\opencv\release\install\x64\mingw\bin\libopencv_objdetect452.dll
LIBS += C:\opencv\opencv\release\install\x64\mingw\bin\libopencv_photo452.dll
LIBS += C:\opencv\opencv\release\install\x64\mingw\bin\libopencv_stitching452.dll
LIBS += C:\opencv\opencv\release\install\x64\mingw\bin\libopencv_video452.dll
LIBS += C:\opencv\opencv\release\install\x64\mingw\bin\libopencv_videoio452.dll
LIBS += C:\opencv\opencv\release\install\x64\mingw\bin\libopencv_tracking452.dll

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    Video test/cars.mp4 \
    Video test/cars.mp4 \
    Video test/test.mp4 \
    Video test/test.mp4
