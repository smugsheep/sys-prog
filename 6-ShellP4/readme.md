1. How does the remote client determine when a command's output is fully received from the server, and what techniques can be used to handle partial reads or ensure complete message transmission?

_The client looks for EOF character to mark the end of the output. A loop reads data until this character is received, handling partial reads by continuously appending to the buffer._

2. This week's lecture on TCP explains that it is a reliable stream protocol rather than a message-oriented one. Since TCP does not preserve message boundaries, how should a networked shell protocol define and detect the beginning and end of a command sent over a TCP connection? What challenges arise if this is not handled correctly?

_Techniques for this can include length-prefixing, delimiters, or fixed-size messages. All of these give some boundary the shell can look for when recieving TCP data. Incorrect handling can lead to data corruption, deadlocks due to incomplete data, and potential security vulnerabilities within the system, which are all things you do not want sending user data._

3. Describe the general differences between stateful and stateless protocols.

_Stateful protocols maintain the session state (the information) on the server. This leads to more complex interaction. Stateless protocols treat each request independently, not having session state persist between each message. This simplifies the server but requires each request to be self-contained._

4. Our lecture this week stated that UDP is "unreliable". If that is the case, why would we ever use it?

_UDP is faster than TCP. It is often used for applications prioritizing speed and low latency rather than 100% reliability, like video streaming or gaming, where occasional packet loss can be tolerated or handled._

5. What interface/abstraction is provided by the operating system to enable applications to use network communications?

_Like in the assignment, the Socket API provides a set of system calls (`socket()`, `bind()`, `send()`, `recv()`, etc.) that enables this communication._