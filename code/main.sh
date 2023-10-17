g++ -c -g -Wall `root-config --cflags` histogram.cpp
g++ -c -g -Wall `root-config --cflags` plot_flavour_hist.cpp
g++ -o plot_flavour_hist.exe `root-config --glibs` histogram.o plot_flavour_hist.o
./plot_flavour_hist.exe
