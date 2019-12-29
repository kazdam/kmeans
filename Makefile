CXX = g++
CC = g++
CXXFLAGS = -std=c++11

kmeans: kmeans_main.o kmeans.o read.o
kmeans.o: kmeans.cpp kmeans.hpp
kmeans_main.o: kmeans_main.cpp kmeans.hpp
read.o: read.cpp kmeans.hpp


