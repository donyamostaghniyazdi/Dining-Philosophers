all: DiningPhilosophers Waiter Philosopher
DiningPhilosophers: dining_philosophers.t Waiter Philosopher
	gcc $< -g -o $@
Waiter: Waiter.t
	gcc $< -g -o $@	
Philosopher: Philosopher.t
	gcc $< -g -o $@	
DiningPhilosophers.t: DiningPhilosophers.c CleanTmpFifo
	gcc -c $<	
waiter.o: Waiter.c
	gcc -c $<
Philosopher.t: Philosopher.c
	gcc -c $<
.PHONY: clean
clean:
	rm -f DiningPhilosophers.t
	rm -f Waiter.t
	rm -f Philosopher.t