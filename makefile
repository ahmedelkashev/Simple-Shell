all:
	flex lex.l
	cc myshell.c lex.yy.c -lfl -o myshell
	cc cwd.c -o cwd
clear:
	rm lex.yy.c
	rm myshell
	rm cwd
