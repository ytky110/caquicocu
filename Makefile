TARGET = bin/caquicocu

$(TARGET): src/*.cxx include/*
	mkdir -p bin
	c++ src/*.cxx -o $@ -I include

.PHONY: clean

clean:
	rm -fr bin/
	rm -fr pkg/
	.ypkg2/CLEANPKG

# For yports

.PHONY: installpkg2 buildpkg2

installpkg2: buildpkg2
	ypkg2 install pkg/*

buildpkg2: $(TARGET)
	mkdir -p pkg
	.ypkg2/MAKEPKG
