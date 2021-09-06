lambda:
		bison -d lambda.y
		flex lambda.l
		gcc `pkg-config --cflags glib-2.0` lambda.tab.c lex.yy.c lambda.c term.c util.c -lfl -lglib-2.0 -o lambda
        
flex:
		flex lambda.l

bison:
		bison -d lambda.y

debug:
		gcc lambda.tab.c lex.yy.c lambda.c term.c util.c -lfl -lglib-2.0 -g -o lambda.debug
        
bison-debug:
		bison -d lambda.y -v

clean:
		rm -f lambda lambda.debug lambda.output lambda.tab.* lex.yy.c