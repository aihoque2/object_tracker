collect: main.cpp dataCollector.cpp dataframe.cpp
	g++ -o collect main.cpp dataCollector.cpp dataframe.cpp `pkginfo openCV` -I.
