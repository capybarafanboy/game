windows:
	g++ -std=c++20 main.cpp -I.\x86_64-w64-mingw32\include -L.\x86_64-w64-mingw32\lib -Wall -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -O2 -o main
linux:
	g++ -std=c++20 main.cpp -Wall -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -O2 -o main