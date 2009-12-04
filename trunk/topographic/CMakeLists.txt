PROJECT(topographic)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include)

FILE(GLOB SRCS "src/*.c" "src/*.cpp")
FILE(GLOB HDRS "include/*.h")

ADD_EXECUTABLE(topographic ${SRCS} ${HDRS})

TARGET_LINK_LIBRARIES(topographic ${OpenCV_LIBS})

