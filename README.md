# Multiprocessing-pipe-in-Linux

A letter frequency counter program in Linux using pipes.

•	The child processes searching the file for the character in the interval received, computes their frequencies and return them through stdout to the parent process's pipe. 

•	The parent process computes the total number of characters in the file, and the rate of the appearance of the character frequencies received from the two child process, through separate pipes.

•	The parent process then form a table of alphabetic characters and their frequencies and print the table in a proper format on the screen.


