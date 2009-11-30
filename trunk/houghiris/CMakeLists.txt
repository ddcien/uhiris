PROJECT(uhiris)

SET(iris_SRCS
	src/iris_main.cxx
)

SET(iris_HDRS
)

ADD_EXECUTABLE(iris ${iris_SRCS} ${iris_HDRS})
TARGET_LINK_LIBRARIES(iris ${OpenCV_LIBS})