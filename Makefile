TARGET = tp2
#PROCESS_COUNT = 2
GCC_FLAGS = -Wall -g -fopenmp

RUN_OPTIONS = world.txt

RUN_ARGS =

FILES := $(shell find . -type f -name '*.c' -printf '%P\n')


# Shell commands
GCC = gcc
RUN = ./tp2 ${RUN_OPTIONS}

run: ${TARGET}
	${RUN}

${TARGET}: ${OBJS}
	${GCC} ${FILES} -o ${TARGET} ${GCC_FLAGS}

clean:
	${RM} $(shell find . -type f -name '*\.o')
	${RM} ${TARGET}