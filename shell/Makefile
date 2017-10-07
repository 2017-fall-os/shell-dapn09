all: shell

shell: shell.c myTok.c myTok.h helpers.c helpers.h
	gcc -o shell shell.c myTok.c myTok.h helpers.c helpers.h

clean:
	rm shell testLog.txt

test: shell testShell.sh
	./testShell.sh ./shell
