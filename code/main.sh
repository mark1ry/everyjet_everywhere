g++ -c -g -Wall `root-config --cflags` histogram.cpp
g++ -c -g -Wall `root-config --cflags` plot_flavours_truth.cpp
g++ -o plot_flavours_truth.exe `root-config --glibs` histogram.o plot_flavours_truth.o 
./plot_flavours_truth.exe
