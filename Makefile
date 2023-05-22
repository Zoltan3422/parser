target: output

output: main.o toojpeg.o
	g++ main.o toojpeg.o -o parser -std=c++11

main.o: main.cpp
	g++ -c main.cpp -std=c++11

toojpeg.o: toojpeg.cpp toojpeg.h
	g++ -c toojpeg.cpp -std=c++11

clean:
	rm *.o parser