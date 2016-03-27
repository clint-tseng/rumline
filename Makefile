LIB_FILES = target/mark.o target/linked-list.o

test: $(LIB_FILES)
	gcc -o target/test $(LIB_FILES) test/all.c
	./target/test

target/%.o: src/%.c
	gcc -c -o $@ $<

clean:
	rm -f target/*

