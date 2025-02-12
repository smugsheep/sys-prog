1. In this assignment I suggested you use `fgets()` to get user input in the main while loop. Why is `fgets()` a good choice for this application?

    > **Answer**:  `fgets()` is a good choice because it reads user input line by line, stopping at newline/EOF. It allows you to specify a specific buffer length so that there will be no unwanted overflow problems. 

2. You needed to use `malloc()` to allocte memory for `cmd_buff` in `dsh_cli.c`. Can you explain why you needed to do that, instead of allocating a fixed-size array?

    > **Answer**:  `malloc()` allows for dynamic memory allocation so that it can accomodate for different input lengths. We don't want to use a fixed sized array because it could lead to overflow or wasted memory.


3. In `dshlib.c`, the function `build_cmd_list()` must trim leading and trailing spaces from each command before storing it. Why is this necessary? If we didn't trim spaces, what kind of issues might arise when executing commands in our shell?

    > **Answer**:  If we didn't trim spaces, it could lead to parsing errors like incorrect argument splitting or failing to match the command name. Trimming spaces ensures that the commands are processed correctly.

4. For this question you need to do some research on STDIN, STDOUT, and STDERR in Linux. We've learned this week that shells are "robust brokers of input and output". Google _"linux shell stdin stdout stderr explained"_ to get started.

- One topic you should have found information on is "redirection". Please provide at least 3 redirection examples that we should implement in our custom shell, and explain what challenges we might have implementing them.

    > **Answer**:  Redirection examples include `cmd > file`, which redirects STDOUT to a file, `cmd < file`, which redirects STDIN from a file, and `cmd 2> file`, which redirects STDERR to a file. Challeneges implementing them might include handling the opening, read/write, and closing of a file properly. We also must ensure that the proper streams are redirected.

- You should have also learned about "pipes". Redirection and piping both involve controlling input and output in the shell, but they serve different purposes. Explain the key differences between redirection and piping.

    > **Answer**: Redirection is used to control the input and output and where they come/go, but piping directly connects the output of one command to the input of other. We use pipes to continously process data between commands, and redirection to deal with moving data from/to externally.

- STDERR is often used for error messages, while STDOUT is for regular output. Why is it important to keep these separate in a shell?

    > **Answer**:  Seperating these streams are important to ensure that regular output and error messages do not get mixed up. It also makes it easier for scripts and users to capture and handle errors easily, seperate of regular output.

- How should our custom shell handle errors from commands that fail? Consider cases where a command outputs both STDOUT and STDERR. Should we provide a way to merge them, and if so, how?

    > **Answer**:  We want to handle errors from failed commands by providing a useful error message and error return code. If we output to both STDOUT and STDERR, we should probably keep them seperate to give better control and handling. However, we might want to merge them if we have some sort of error log, where we would then redirect both to let the user debug themselves.  