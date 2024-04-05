 # Dining Philosophers Problem with Process Synchronization



To manage the access of philosophers to the forks, this program implements a solution using processes, semaphores, and inter-process communication (IPC). Here's a breakdown of the components:

# Components:

 Philosophers (`philosopher.c`):
- Each philosopher is represented as a separate process.
- Philosophers go through a cycle of thinking and eating.
- To eat, a philosopher needs to acquire both forks (semaphores).
- After eating, they release the forks and go back to thinking.

# Waiter (`waiter.c`):
- The waiter oversees the dining process, ensuring that no more than four philosophers can simultaneously eat.
- It acts as a process controller, managing access to the dining table.

 # Utility (`clean_tmp_fifo.c`):
- This utility cleans up temporary FIFOs created during execution.

# Main (`main.c`):
- Orchestrates the initialization of the system.
- Creates FIFOs for communication between philosophers and the waiter.
- Spawns processes for the waiter and the philosophers.
- Implements error handling and cleanup routines.

 # Instructions:

1. Compile the program using `gcc main.c -o dining_philosophers`.
2. Run the program with `./dining_philosophers`.
3. Watch as the philosophers engage in their dining ritual, managed by the waiter.
4. Upon completion, the program automatically cleans up temporary resources.

 # Notes:

- The program demonstrates the use of processes, FIFOs, and semaphores for synchronization.
- It provides insights into process coordination and resource management in Unix-like systems.
- Feel free to explore and modify the code for learning or experimentation purposes.



