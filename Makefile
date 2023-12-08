DAY = hello

flgs = -static -Wall
inc = -Isrc
lnk = 
hdrs = src/common.h src/array.h
ext = 

ifeq ($(OS),Windows_NT)
	inc += -Iraylib-5.0_win64_mingw-w64\include
	lnk += -Lraylib-5.0_win64_mingw-w64\lib -lraylib -lopengl32 -lgdi32 -lwinmm
	ext += .exe
endif

run: exe/$(DAY).$(ext)
	./exe/$(DAY).$(ext)

exe/$(DAY).$(ext): src/$(DAY).c Makefile $(hdrs)
	gcc src/$(DAY).c -o exe/$(DAY).$(ext) $(flgs) $(inc) $(lnk)
