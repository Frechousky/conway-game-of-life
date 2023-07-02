SOURCES=gameoflife.c cmdline.c cmdline.h
BIN=gameoflife

$(BIN): $(SOURCES)
	gcc -o $(BIN) $^

debug: $(SOURCES)
	gcc -g -o $(BIN) $^

run: $(BIN)
	./gameoflife

clean:
	rm -f $(BIN)

valgrind: $(BIN)
	valgrind ./$(BIN) --leak-check=full

gengetopt: gengetopt.conf
	gengetopt < $<
