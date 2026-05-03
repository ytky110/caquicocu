bin/caquicocu: src/caquicocu.cxx
	c++ $^ -o $@

.PHONY: clean

clean:
	rm -f bin/*
