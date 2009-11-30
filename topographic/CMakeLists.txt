PROJECT(topographic)

FILE(GLOB SRCS "src/*.c" "src/*.cpp")
FILE(GLOB HDRS "include/*.h")

ADD_EXECUTABLE(topographic ${SRCS} ${HDRS})

TARGET_LINK_LIBRARIES(topographic ${OpenCV_LIBS})

