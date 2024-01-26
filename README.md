Building the game:
Building is done through Make, with commands: make windows or make linux
Make expects SDL2 to be set up already with SDL2_IMG and SDL2_Mixer extensions installed. For windows it is statically linked to a folder inside the project titled "x86_64-w64-mingw32", for linux, it attempts to use the system installation of SDL2.
