bin/caquicocu: src/*.cxx include/*
	mkdir -p bin
	c++ src/*.cxx -o $@ -I include

.PHONY: clean

clean:
	rm -f bin/*
