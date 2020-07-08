FORMAT=clang-format
FORMATFLAGS=-i -style="{IndentWidth: 4,TabWidth: 4}"

CXX=clang++
CXXFLAGS=-Wall -Wno-unknown-pragmas -pedantic -g

filter_test: filter_test.cpp filter.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^
	./filter_test

.PHONY: format
format: filter_test.cpp filter.cpp filter.h
	$(FORMAT) $(FORMATFLAGS) $^

.PHONY: clean
clean:
	rm -f filter_test
