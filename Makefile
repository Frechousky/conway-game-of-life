run: gameoflife
	./gameoflife

gameoflife: gameoflife.c
	gcc gameoflife.c -o gameoflife

clean:
	rm -f gameoflife
