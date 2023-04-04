SOURCES=gameoflife.c
BIN=gameoflife

$(BIN): $(SOURCES)
	gcc $< -o $(BIN)

run: $(BIN)
	./gameoflife

valgrind: $(BIN)
	valgrind ./$(BIN) --leak-check=full

clean:
	rm -f $(BIN)
