all: run

.PHONY:run clean

run: puzzle.x
	@./puzzle.x


%.x:%.cpp
	$(CXX) -std=c++20 -O3 -g $< -o $@
clean:
	@rm -rf *.x
