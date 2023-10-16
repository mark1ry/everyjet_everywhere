g++ -c -g -Wall `root-config --cflags` histogram.cpp
g++ -c -g -Wall `root-config --cflags` plot_single_hist.cpp
g++ -o plot_single_hist.exe `root-config --glibs` histogram.o plot_single_hist.o
./plot_single_hist.exe
