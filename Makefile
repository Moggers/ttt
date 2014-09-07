build_deug: DEBUG=-g -D DEBUG
build_release: DEBUG=
build_fas: DEBUG=-g -D DEBUG -fsanitize=address

build_deug: clean ttt
build_release: clean ttt
build_fas: clean ttt

LINKS=-lSDL2 -lGL -lGLU -lm -lpthread

ttt: main.o render.o matrix4f.o vec3f.o vec4f.o hullbox.o stack.o ts_queue.o camera.o debug.o tank.o
	gcc -o ttt main.o render.o matrix4f.o vec3f.o vec4f.o hullbox.o stack.o ts_queue.o camera.o debug.o tank.o $(LINKS) $(DEBUG)

clean:
	rm -rf ttt *.o

main.o: main.c
	gcc -c main.c -o main.o $(DEBUG)

render.o: render.c
	gcc -c render.c -o render.o $(DEBUG)

matrix4f.o: matrix4f.c
	gcc -c matrix4f.c -o matrix4f.o $(DEBUG)

vec4f.o: vec4f.c
	gcc -c vec4f.c -o vec4f.o $(DEBUG)

vec3f.o: vec3f.c
	gcc -c vec3f.c -o vec3f.o $(DEBUG)

hullbox.o: hullbox.c
	gcc -c hullbox.c -o hullbox.o $(DEBUG)

stack.o: stack.c
	gcc -c stack.c -o stack.o $(DEBUG)

ts_queue.o: ts_queue.c
	gcc -c ts_queue.c -o ts_queue.o $(DEBUG)

camera.o: camera.c
	gcc -c camera.c -o camera.o $(DEBUG)

debug.o: debug.c
	gcc -c debug.c -o debug.o $(DEBUG)

tank.o: tank.c
	gcc -c tank.c -o tank.o $(DEBUG)
