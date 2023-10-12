g++ -c -g -Wall `root-config --cflags` plot.cpp
g++ -c -g -Wall `root-config --cflags` plot_weights.cpp
g++ -o plot_weights.exe `root-config --glibs` plot.o plot_weights.o
./plot_weights.exe
