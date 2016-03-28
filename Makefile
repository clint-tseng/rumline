LIB_FILES = target/linked-list.o target/mark.o target/group.o target/strtod.o

test: $(LIB_FILES)
	gcc -o target/test $(LIB_FILES) test/all.c
	./target/test

target/strtod.o: src/ext/strtod.c
	gcc -c -o target/strtod.o src/ext/strtod.c

target/%.o: src/%.c
	gcc -c -o $@ $<

clean:
	rm -f target/*

