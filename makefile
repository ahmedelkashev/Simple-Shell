all:
	flex lex.l
	cc myshell.c lex.yy.c -lfl -o myshell
	cc cwd.c -o cwd
	cc ls.c -o ls
clear:
	rm lex.yy.c
	rm myshell
	rm cwd
	rm ls
