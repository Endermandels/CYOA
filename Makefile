EXEC = cyoa
OBJS = main.o
CCFLAGS = gcc -Wall -std=c99 --pedantic
GCACHE = ./GameCache

${EXEC}: ${OBJS}
	${CCFLAGS} -o ${EXEC} ${OBJS}
	make gcache

# if the game cache does not exist, create it
gcache: | ${GCACHE}
	mv ${OBJS} ${EXEC} ${GCACHE}

# $@ refers to the name of this command, ${GCACHE}
# -p creates the directory if it doesn't exist
#	and does not return an error if it does exist
${GCACHE}:
	mkdir -p $@

main.o: main.c
	${CCFLAGS} -c main.c

run: ${EXEC}
	${GCACHE}/${EXEC}

runv: ${EXEC}
	valgrind ${GCACHE}/${EXEC}