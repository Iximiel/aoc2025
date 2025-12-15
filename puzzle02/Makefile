all: puzzle2.x
.PHONY: clean

%.x:%.cpp
	$(CXX) -O3 --std=c++20 $< -o $@
clean:
	@rm -rf *.x
