build_deug: DEBUG=-g -D DEBUG
build_release: DEBUG=

build_deug: clean ttt
build_release: clean ttt

ttt: main.o render.o matrix4f.o debug.o
	gcc -o ttt main.o render.o matrix4f.o debug.o -lSDL2 -lGL -lGLU

clean:
	rm -rf ttt *.o

main.o: main.c
	gcc -c main.c -o main.o $(DEBUG)

render.o: render.c
	gcc -c render.c -o render.o $(DEBUG)

matrix4f.o: matrix4f.c
	gcc -c matrix4f.c -o matrix4f.o $(DEBUG)

debug.o: debug.c
	gcc -c debug.c -o debug.o $(DEBUG)
