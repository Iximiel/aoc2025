all: run

.PHONY:run clean

run: puzzle.x
	@./puzzle.x


%.x:%.cpp
	$(CXX) -O3 -g $< -o $@
clean:
	@rm -rf *.x
