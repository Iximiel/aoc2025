all: run

.PHONY:run clean

run: puzzle4.x
	@./puzzle4.x


%.x:%.cpp
	$(CXX) -O3 -g $< -o $@
clean:
	@rm -rf *.x
