all: run

.PHONY:run clean

run: puzzle3.x
	@./puzzle3.x


%.x:%.cpp
	$(CXX) -O3 -g $< -o $@
clean:
	@rm -rf *.x
