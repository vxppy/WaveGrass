@echo off
g++ -c main.cpp
g++ -c wglexer.cpp
g++ -c wgast.cpp
g++ -c wgparser.cpp
g++ -c wgenvironment.cpp
g++ -c wgclasses.cpp
g++ main.o wglexer.o wgparser.o wgast.o wgenvironment.o wgclasses.o -o main.exe
main.exe
@REM del main.exe