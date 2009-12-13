cmake_minimum_required(VERSION 2.6)

project(dc_DIP_HW2)
FIND_PACKAGE(OpenCV REQUIRED)
FIND_PACKAGE(FFTW REQUIRED)

FILE(GLOB SRCS src/*.cpp )
FILE(GLOB HDRS include/*.h )

INCLUDE_DIRECTORIES(include)

ADD_LIBRARY(DCHW2Common ${SRCS} ${HDRS})
ADD_EXECUTABLE(HistogramShaping src/main1.cxx)
ADD_EXECUTABLE(ToeplitzStripe src/main2.cxx)
ADD_EXECUTABLE(LogPolar src/main3.cxx)
ADD_EXECUTABLE(RetinalImage src/main4.cxx)

TARGET_LINK_LIBRARIES(DCHW2Common ${OpenCV_LIBS})

foreach(__target_executable HistogramShaping ToeplitzStripe LogPolar RetinalImage)
	TARGET_LINK_LIBRARIES(${__target_executable} DCHW2Common ${OpenCV_LIBS}) 
endforeach(__target_executable)

TARGET_LINK_LIBRARIES(ToeplitzStripe fftw3)
