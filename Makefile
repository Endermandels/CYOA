EXEC = cyoa
OBJS = main.o readStory.o
CCFLAGS = gcc -Wall -std=c99 --pedantic
CACHE = ./.cache

${EXEC}: ${OBJS}
	${CCFLAGS} -o ${EXEC} ${OBJS}
	make cache

# if the game cache does not exist, create it
cache: | ${CACHE}
	mv ${OBJS} ${EXEC} ${CACHE}

# $@ refers to the name of this command, ${CACHE}
# -p creates the directory if it doesn't exist
#	and does not return an error if it does exist
${CACHE}:
	mkdir -p $@

main.o: main.c
	${CCFLAGS} -c main.c

readStory.o: readStory.c
	${CCFLAGS} -c readStory.c

run: ${EXEC}
	${CACHE}/${EXEC}

runv: ${EXEC}
	valgrind ${CACHE}/${EXEC}