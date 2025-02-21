1. Can you think of why we use `fork/execvp` instead of just calling `execvp` directly? What value do you think the `fork` provides?

    > **Answer**:  `execvp` replaces the current process image with the new program. If we were to call it directly, the shell itself would be replaced by the executed command, and the shell would terminate. This is no good. `fork` creates a new process that is a copy of the shell, allowing the shell to execute further commands after the child process completes (the `execvp` call replaces the child process's image with the requested program).

2. What happens if the fork() system call fails? How does your implementation handle this scenario?

    > **Answer**:  If `fork()` fails, it returns a negative value. In my implementation, the parent process checks the return value of `fork()`: if it's less than 0, the code calls `perror("fork")` to print an error message to stderr.

3. How does execvp() find the command to execute? What system environment variable plays a role in this process?

    > **Answer**: It finds the command to execute by searching the directories listed in the PATH environment variable, which contains a list of directories that the shell searches when looking for an executable file.

4. What is the purpose of calling wait() in the parent process after forking? What would happen if we didn’t call it?

    > **Answer**: The purpose this is to wait for the child process to complete its execution. If we didn't call wait(), the parent process would continue executing without waiting for the child to finish. This could lead to several problems, including incorrect output or a process becoming stuck.

5. In the referenced demo code we used WEXITSTATUS(). What information does this provide, and why is it important?

    > **Answer**:  `WEXITSTATUS(status)` extracts the exit status of a child process from the passed-in status variable. It is important because it provides information about whether the child process executed successfully or encountered an error, which we can then handle appropriately.

6. Describe how your implementation of build_cmd_buff() handles quoted arguments. Why is this necessary?

    > **Answer**:  My implementation handles quoted arguments by iterating through the string and taking note of where a quote starts and ends, so we can keep the contents as one string. This is necessary because quoted arguments need to be kept exactly as specified, and regular tokenization via spaces does not respect that.

7. What changes did you make to your parsing logic compared to the previous assignment? Were there any unexpected challenges in refactoring your old code?

    > **Answer**:  In this assignment, the parsing logic was refactored to handle only a single command at a time and store it in a command buffer. This involved removing the code that split the input by pipes and re-making the tokenization logic to work directly with the command buffer. The unexpected challenges included the proper handling of quotes and spaces which was frankly very annoying to deal with.

8. For this quesiton, you need to do some research on Linux signals. You can use [this google search](https://www.google.com/search?q=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&oq=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&gs_lcrp=EgZjaHJvbWUyBggAEEUYOdIBBzc2MGowajeoAgCwAgA&sourceid=chrome&ie=UTF-8) to get started.

- What is the purpose of signals in a Linux system, and how do they differ from other forms of interprocess communication (IPC)?

    > **Answer**:  Signals are a form of IPC used to notify a process of an external event. They are a limited form of IPC, as they carry just the signal number. Signals are typically used for asynchronous notifcations of an event, such as interrupts, errors, or user actions. Other forms of IPC are often used for general-purpose data exchange, but signals are asynchronous and used for process control. They also don't hold much data.

- Find and describe three commonly used signals (e.g., SIGKILL, SIGTERM, SIGINT). What are their typical use cases?

    > **Answer**:  SIGINT (Interrupt): This signal is typically sent when the user presses Ctrl+C. It is a nice request (not forceful) to the process to terminate. The process can catch and handle this signal, or it can ignore it entirely.  SIGTERM (Terminate): This signal is the default signal sent by the kill command, requesting to the process to terminate. Like SIGINT, the process can handle this signal, but cannot be ignored. SIGKILL (Kill): This signal is a forceful termination to the process. It cannot be caught and handled or ignored. When a process receives SIGKILL, it is immediately terminated.

- What happens when a process receives SIGSTOP? Can it be caught or ignored like SIGINT? Why or why not?

    > **Answer**:  When a process receives SIGSTOP, it is immediately paused and remains that way until it receives a SIGCONT signal. SIGSTOP cannot be caught or ignored by a process because SIGSTOP (and SIGKILL) are designed to provide a reliable way to control processes - if we could catch or ignore them, it would defeat the purpose.