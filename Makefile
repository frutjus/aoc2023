flgs = -static
inc = -Iraylib-5.0_win64_mingw-w64\include
lnk = -Lraylib-5.0_win64_mingw-w64\lib -lraylib -lopengl32 -lgdi32 -lwinmm

hello.exe: hello.c
	gcc hello.c -o hello.exe $(flgs) $(inc) $(lnk)

run: hello.exe
	./hello.exe
