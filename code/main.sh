g++ -c -g -Wall `root-config --cflags` plot.cpp
g++ -c -g -Wall `root-config --cflags` plot_leptons.cpp
g++ -o plot_leptons.exe `root-config --glibs` plot.o plot_leptons.o
./plot_leptons.exe
