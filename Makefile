LIB_FILES = target/linked-list.o target/mark.o target/group.o

test: $(LIB_FILES)
	gcc -o target/test $(LIB_FILES) test/all.c
	./target/test

target/%.o: src/%.c
	gcc -c -o $@ $<

clean:
	rm -f target/*

