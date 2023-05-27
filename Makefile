EXEC = cyoa
OBJS = main.o exitManager.o fileManager.o prompts.o
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

fileManager.o: fileManager.c
	${CCFLAGS} -c fileManager.c

exitManager.o: exitManager.c
	${CCFLAGS} -c exitManager.c

prompts.o: prompts.c
	${CCFLAGS} -c prompts.c

run: ${EXEC}
	${CACHE}/${EXEC}

runv: ${EXEC}
	valgrind ${CACHE}/${EXEC}

runvs: ${EXEC}
	valgrind -s ${CACHE}/${EXEC}