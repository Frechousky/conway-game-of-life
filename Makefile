run: gameoflife
	./gameoflife

gameoflife:
	gcc gameoflife.c -o gameoflife

clean:
	rm -f gameoflife
