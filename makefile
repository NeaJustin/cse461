main: main.cpp Sdisk.o Filesys.o Shell.o
	g++ main.cpp Sdisk.o Filesys.o Shell.o -o main

Shell.o: Shell.cpp
	g++ -c Shell.cpp

Sdisk.o: Sdisk.cpp
	g++ -c Sdisk.cpp

Filesys.o: Filesys.cpp
	g++ -c Filesys.cpp

clean:
	rm *.o
