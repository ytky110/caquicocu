bin/caquicocu: src/caquicocu.cxx
	mkdir -p bin
	c++ $^ -o $@

.PHONY: clean

clean:
	rm -f bin/*
