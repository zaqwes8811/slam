
INC=-I/usr/local/include/mrpt/slam/include \
	-I/usr/local/include/mrpt/maps/include \
	-I/usr/local/include/mrpt/math/include \
	-I/usr/local/include/mrpt/rtti/include \
	-I/usr/local/include/mrpt/core/include \
	-I/usr/local/include/mrpt/obs/include \
	-I/usr/local/include/mrpt/poses/include \
	-I/usr/local/include/mrpt/typemeta/include \
	-I/usr/local/include/mrpt/system/include \
	-I/usr/local/include/mrpt/bayes/include \
	-I/usr/local/include/mrpt/config/include \
	-I/usr/local/include/mrpt/containers/include \
	-I/usr/local/include/mrpt/opengl/include \
	-I/usr/local/include/mrpt/img/include \
	-I/usr/local/include/mrpt/tfest/include \
	-I/usr/local/include/mrpt/serialization/include \
	-I/usr/local/include/mrpt/nanoflann/include/ \
	-I/usr/local/include/mrpt/graphs/include \
	-I/usr/local/include/mrpt/io/include \
	-I/usr/local/include/mrpt/vision/include \
	-I/usr/local/include/mrpt/random/include \
	`pkg-config eigen3 --cflags`
all:
	g++ $(INC) -std=c++17 test.cpp -o mrpt