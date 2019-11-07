######################################################################
# Automatically generated by qmake (3.1) Wed Oct 16 09:05:58 2019
######################################################################

TEMPLATE = app
TARGET = dettusSDR
INCLUDEPATH += . \
		threads/	\
		gui/		\
		signalprocessing/	\
		tuners/		\
		tuners/dummy	\
		tuners/rtltcp	\
		demod/	\
		demod/am	\
		demod/fm	\
		shared

CONFIG += debug
CONFIG += c++11

# The following define makes your compiler warn you if you use any
# feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Input
HEADERS +=	threads/TunerMain.h	\
		threads/CentralMain.h	\
		threads/DemodMain.h	\
		threads/AudioMain.h	\
		threads/AudioIODevice.h	\
		tuners/Tuners.h		\
		tuners/TunerDialog.h	\
		tuners/dummy/TDummy.h	\
		tuners/rtltcp/TRtlTcp.h	\
		signalprocessing/Downsampler.h	\
		signalprocessing/SimpleFft.h	\
		signalprocessing/SimpleShifter.h	\
		signalprocessing/Filter.h	\
		demod/DemodWidget.h	\
		demod/Demod.h	\
		demod/am/DemodAM.h	\
		demod/fm/DemodFM.h	\
		gui/WSpectrum2.h		\
		shared/DataTypes.h

SOURCES +=	main.cpp \
		threads/TunerMain.cpp	\
		threads/CentralMain.cpp	\
		threads/DemodMain.cpp	\
		threads/AudioMain.cpp	\
		threads/AudioIODevice.cpp	\
		tuners/TunerDialog.cpp	\
		tuners/dummy/TDummy.cpp	\
		tuners/rtltcp/TRtlTcp.cpp	\
		gui/WSpectrum2.cpp		\
		signalprocessing/Downsampler.cpp	\
		signalprocessing/Filter.cpp	\
		signalprocessing/SimpleFft.cpp	\
		signalprocessing/SimpleShifter.cpp	\
		demod/DemodWidget.cpp	\
		demod/am/DemodAM.cpp	\
		demod/fm/DemodFM.cpp	
         
           
QT += multimedia widgets
