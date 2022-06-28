# gnl
Repository for the get next line project of the 42 Core Curriculum

### Goal: Write a function, which returns a full next line of a file given its file descriptor.

This project is a really handy one, since it will allow us to learn about static local variables and the file descriptors workings. The usecases are endless in which this program can be useful, as files often
need to be read one line at a time. Here we need to be able to read a file line by line disregarding its length. Difficulty is increased by setting a variable called BUFFER_SIZE which limits the amount of bytes that 
are allowed to be read at one time. This means, that with one read of the file, we might not have a whole line in the buffersize, or several lines at once. The ouput needs to be the same: one line per function call.
With repeated function calls, the entire file should be readable.
