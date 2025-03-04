1. Your shell forks multiple child processes when executing piped commands. How does your implementation ensure that all child processes complete before the shell continues accepting user input? What would happen if you forgot to call waitpid() on all child processes?

_My shell implementation ensures this by using `waitpid()` in a loop, which pauses the shell until each child process is complete. If you don't use `waitpid()`, then it can create zombie processes that run even though the next shell prompt has already been displayed. This can lead to unpredictable output and resource issues, which is not good._

2. The dup2() function is used to redirect input and output file descriptors. Explain why it is necessary to close unused pipe ends after calling dup2(). What could go wrong if you leave pipes open?

_If you don't close the all the unsued pipes, the shell may think there is still some process using it. This can cause the program to get stuck waiting for a signal that will never come, which may eat resources or lead to other problems._

3. Your shell recognizes built-in commands (cd, exit, dragon). Unlike external commands, built-in commands do not require execvp(). Why is cd implemented as a built-in rather than an external command? What challenges would arise if cd were implemented as an external process?

_`cd` is implemented as a built-in because it modifies the current working directory of the shell process. If it were an external command, it would only change the working directory of the child process executing cd and not the shell's, which is a challenge to work around._

4. Currently, your shell supports a fixed number of piped commands (CMD_MAX). How would you modify your implementation to allow an arbitrary number of piped commands while still handling memory allocation efficiently? What trade-offs would you need to consider?

_I would change my implementation of `command_list_t` to have a dynamically allocated array for `commands`. Then, I would incrementally reallocate memory as more commands are parsed (i.e. start with 4, double each time we cap out). The trade-off with this strategy is how much and how often we are reallocating since reallocation of memory (I assume) can be expensive on the system depending on the size._