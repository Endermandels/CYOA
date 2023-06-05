EXEC = cyoa
OBJS = main.o exitManager.o fileManager.o userInput.o prompts.o game.o
CCFLAGS = gcc -Wall -std=gnu11 --pedantic
CACHE = ../.cache

${EXEC}: ${OBJS}
	${CCFLAGS} -o ${EXEC} ${OBJS}
	make cache

# if the cache does not exist, create it
cache: | ${CACHE}
	mv ${OBJS} ${CACHE}
	mv ${EXEC} ..

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

userInput.o: userInput.c
	${CCFLAGS} -c userInput.c

prompts.o: prompts.c
	${CCFLAGS} -c prompts.c

game.o: game.c
	${CCFLAGS} -c game.c

run: ${EXEC}
	../${EXEC}

runv: ${EXEC}
	valgrind ../${EXEC}

runvs: ${EXEC}
	valgrind -s ../${EXEC}