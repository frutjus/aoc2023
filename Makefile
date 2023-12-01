DAY = hello

flgs = -static
inc = -Iraylib-5.0_win64_mingw-w64\include -Isrc
lnk = -Lraylib-5.0_win64_mingw-w64\lib -lraylib -lopengl32 -lgdi32 -lwinmm

run: exe/$(DAY).exe
	./exe/$(DAY).exe

exe/$(DAY).exe: src/$(DAY).c
	gcc src/$(DAY).c -o exe/$(DAY).exe $(flgs) $(inc) $(lnk)
