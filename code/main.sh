g++ -c -g -Wall `root-config --cflags` plot.cpp
g++ -c -g -Wall `root-config --cflags` plot_jets.cpp
g++ -o plot_jets.exe `root-config --glibs` plot.o plot_jets.o
./plot_jets.exe
