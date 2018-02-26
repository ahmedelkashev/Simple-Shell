all:
	flex lex.l
	cc myshell.c lex.yy.c -lfl -o myshell
clear:
	rm lex.yy.c
	rm myshell
