CC = gcc
CFLAGS = -g -Wall -std=gnu99
LIBS = -lm

OBJ_FILES = raytracer.o image_utils.o vector.o main.o
EXE_FILE = ray3

${EXE_FILE}: ${OBJ_FILES}
	${CC} ${CFLAGS} -o ${EXE_FILE} ${OBJ_FILES} ${LIBS}

raytracer.o: raytracer.c
	${CC} ${CFLAGS} -c $< -o $@

image_utils.o: image_utils.c
	${CC} ${CFLAGS} -c $< -o $@

vector.o: vector.c
	${CC} ${CFLAGS} -c $< -o $@

main.o: main.c
	${CC} ${CFLAGS} -c $< -o $@

clean:
	rm -f ${OBJ_FILES} ${EXE_FILE}