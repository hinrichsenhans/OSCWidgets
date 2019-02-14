# Created by and for Qt Creator. This file was created for editing the project sources only.
# You may attempt to use it for building too, by modifying this file here.

#TARGET = OSCWidgets
QT += widgets
QT += network
CONFIG += c++11

HEADERS = \
   OSCWidgets/EditPanel.h \
   OSCWidgets/EosPlatform.h \
   OSCWidgets/FadeButton.h \
   OSCWidgets/LogFile.h \
   OSCWidgets/LogWidget.h \
   OSCWidgets/MainWindow.h \
   OSCWidgets/NetworkThreads.h \
   OSCWidgets/QtInclude.h \
   OSCWidgets/resource.h \
   OSCWidgets/SettingsPanel.h \
   OSCWidgets/Toy.h \
   OSCWidgets/ToyActivity.h \
   OSCWidgets/ToyButton.h \
   OSCWidgets/ToyCmd.h \
   OSCWidgets/ToyEncoder.h \
   OSCWidgets/ToyFlicker.h \
   OSCWidgets/ToyGrid.h \
   OSCWidgets/ToyLabel.h \
   OSCWidgets/ToyMath.h \
   OSCWidgets/ToyMetro.h \
   OSCWidgets/ToyPedal.h \
   OSCWidgets/Toys.h \
   OSCWidgets/ToySine.h \
   OSCWidgets/ToySlider.h \
   OSCWidgets/ToyWidget.h \
   OSCWidgets/ToyWindow.h \
   OSCWidgets/ToyXY.h \
   OSCWidgets/Utils.h \
   EosSyncLib/EosSyncLib/EosLog.h \
   EosSyncLib/EosSyncLib/EosOsc.h \
   EosSyncLib/EosSyncLib/EosSyncLib.h \
   EosSyncLib/EosSyncLib/EosTcp.h \
   EosSyncLib/EosSyncLib/EosTimer.h \
   EosSyncLib/EosSyncLib/EosUdp.h \
   EosSyncLib/EosSyncLib/OSCParser.h

SOURCES = \
   OSCWidgets/EditPanel.cpp \
   OSCWidgets/EosPlatform.cpp \
   OSCWidgets/FadeButton.cpp \
   OSCWidgets/LogFile.cpp \
   OSCWidgets/LogWidget.cpp \
   OSCWidgets/main.cpp \
   OSCWidgets/MainWindow.cpp \
   OSCWidgets/NetworkThreads.cpp \
   OSCWidgets/SettingsPanel.cpp \
   OSCWidgets/Toy.cpp \
   OSCWidgets/ToyActivity.cpp \
   OSCWidgets/ToyButton.cpp \
   OSCWidgets/ToyCmd.cpp \
   OSCWidgets/ToyEncoder.cpp \
   OSCWidgets/ToyFlicker.cpp \
   OSCWidgets/ToyGrid.cpp \
   OSCWidgets/ToyLabel.cpp \
   OSCWidgets/ToyMetro.cpp \
   OSCWidgets/ToyPedal.cpp \
   OSCWidgets/Toys.cpp \
   OSCWidgets/ToySine.cpp \
   OSCWidgets/ToySlider.cpp \
   OSCWidgets/ToyWidget.cpp \
   OSCWidgets/ToyWindow.cpp \
   OSCWidgets/ToyXY.cpp \
   OSCWidgets/Utils.cpp \
   EosSyncLib/EosSyncLib/EosLog.cpp \
   EosSyncLib/EosSyncLib/EosOsc.cpp \
   EosSyncLib/EosSyncLib/EosSyncLib.cpp \
   EosSyncLib/EosSyncLib/EosTcp.cpp \
   EosSyncLib/EosSyncLib/EosTimer.cpp \
   EosSyncLib/EosSyncLib/EosUdp.cpp \
   #EosSyncLib/EosSyncLib/main.cpp \
   EosSyncLib/EosSyncLib/OSCParser.cpp

RESOURCES     = OSCWidgets/OSCWidgets.qrc

OBJECTIVE_SOURCES += OSCWidgets/EosPlatform_Mac_Native.mm
unix {
    HEADERS += \
        EosSyncLib/EosSyncLib/EosTcp_Mac.h \
        EosSyncLib/EosSyncLib/EosUdp_Mac.h


    SOURCES += \
        EosSyncLib/EosSyncLib/EosTcp_Mac.cpp \
        EosSyncLib/EosSyncLib/EosUdp_Mac.cpp
}
mac {
    HEADERS += \
        OSCWidgets/EosPlatform_Mac.h \
        OSCWidgets/EosPlatform_Mac_Bridge.h \
        OSCWidgets/EosPlatform_Mac_Native.h
    SOURCES += \
        OSCWidgets/EosPlatform_Mac.cpp

    LIBS += -framework Foundation
}

win32 {
    HEADERS += \
        EosSyncLib/EosSyncLib/EosTcp_Win.h \
        EosSyncLib/EosSyncLib/EosUdp_Win.h

    SOURCES += \
        EosSyncLib/EosSyncLib/EosTcp_Win.cpp \
        EosSyncLib/EosSyncLib/EosUdp_Win.cpp
    LIBS += -luser32 -lWs2_32
    QT += winextras
}

INCLUDEPATH = \
    OSCWidgets/. \
    EosSyncLib/EosSyncLib/

#ICON = eos.icns
#QMAKE_INFO_PLIST = Info.plist

#DEFINES = 

