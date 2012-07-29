

run: clean main
	./main.exe

main: dll
	g++ -o main main.cpp libwndcls.a -mwindows

dll: mousehook.cpp
	g++ -shared -o mousehook.dll mousehook.cpp

clean:
	rm -rf main.exe main.o mousehook.dll