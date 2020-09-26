CC = g++
CFLAGS = -g -Wall
SRCS = main.cpp dataframe.h dataCollector.h
PROG = collect

OPENCV = `pkg-config opencv --cflags --libs`
LIBS = $(OPENCV)

$(PROG):$(SRCS)
	$(CC) $(CFLAGS) -o $(PROG) $(SRCS) $(LIBS)




#collect: main.cpp dataCollector.cpp dataframe.cpp
#	g++ -o collect main.cpp dataCollector.cpp dataframe.cpp `pkginfo openCV` -I.



