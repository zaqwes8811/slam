PROJ_ROOT=/home/zaqwes/vt_vfr_only/utils/play

CXXFLAGS=-O3 -g -I/opt/eign/include/eigen3 \
	-I/tmp/mathfu-1.0.3/include \
	-I$(PROJ_ROOT)/../../libphotoradar \
	-I$(PROJ_ROOT)/../../libasynctools \
	-I$(PROJ_ROOT)/../../sensorapp \
	-I$(PROJ_ROOT)

SOURCES= $(wildcard *.cpp)

OBJECTS=$(patsubst %.cpp, %.o, $(SOURCES))

TARGET=$(lastword $(subst /, ,$(realpath .)))

LINKS= -lglut -lGL -lGLU -lboost_system -lboost_chrono

CXX=g++



all: $(TARGET)
	echo ImpulseEngine built

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -c $(PROJ_ROOT)/TrafficModel.cpp \
		$(PROJ_ROOT)/TrafficModelHelper.cpp \
		$(PROJ_ROOT)/../../libasynctools/Transform.cpp \
		$(PROJ_ROOT)/../../libasynctools/Log.cpp \
		$(PROJ_ROOT)/../../libasynctools/ICONVConverter.cpp \
		$(PROJ_ROOT)/../../sensorapp/Calc.cpp \
		$(PROJ_ROOT)/../../libphotoradar/ImageZoneDescription.cpp
	$(CXX) $(CXXFLAGS) -o impulseengine.bin $(OBJECTS) \
		TrafficModel.o TrafficModelHelper.o Calc.o Transform.o \
		Log.o ICONVConverter.o ImageZoneDescription.o \
		$(LINKS)

clean:
	rm -rf $(OBJECTS) $(TARGET) *.bin build/*.o *.o
