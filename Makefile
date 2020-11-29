CC = g++
CFLAGS = -g -Wall

OPENCV = `pkg-config opencv --cflags --libs`
LIBS = $(OPENCV)


collect: main.o dataCollector.o dataframe.o
	g++ -g -o collect main.o dataCollector.o dataframe.o $(LIBS)

main.o: main.cpp dataCollector.h
	g++ -g -c main.cpp dataCollector.h $(LIBS)

dataCollector.o: dataCollector.cpp dataCollector.h dataframe.cpp dataframe.h
	g++ -g -c dataCollector.cpp dataCollector.h dataframe.cpp dataframe.h $(LIBS)

dataframe.o: dataframe.cpp dataframe.h
	g++ -g -c dataframe.cpp dataframe.h 

clean:
	rm -f *.o && rm -f *.gch && rm collect


