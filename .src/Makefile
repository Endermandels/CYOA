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

fileManager.o: Util/fileManager.c
	${CCFLAGS} -c Util/fileManager.c

exitManager.o: Util/exitManager.c
	${CCFLAGS} -c Util/exitManager.c

userInput.o: GUI/userInput.c
	${CCFLAGS} -c GUI/userInput.c

prompts.o: Game/prompts.c
	${CCFLAGS} -c Game/prompts.c

game.o: Game/game.c
	${CCFLAGS} -c Game/game.c

run: ${EXEC}
	clear
	../${EXEC}

runv: ${EXEC}
	valgrind ../${EXEC}

runvc: ${EXEC}
	valgrind --leak-check=full ../${EXEC}

runvs: ${EXEC}
	valgrind -s ../${EXEC}