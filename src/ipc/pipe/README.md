# What are pipes?

Reference: pipe(7) page.

**A Pipe is a unidirectional interprocess communication channel which has a read end and write end. It is created using pipe(2) function.**

## Function Definition:

    #include <unistd.h>
    int pipe(int pipefd[2]);

Here pipefd[0] refers to the read end and pipefd[1] refers to the write end. Data written to the write end of the pipe is buffered by the kernel until it is read from the read end of the pipe. pipe(2) conforms to POSIX.1-2001, POSIX.1-2008.
On success, zero is returned.  On error, -1 is returned, errno is set to indicate the error, and pipefd is left unchanged.

## Behaviour:
By default, if a process attempts to read from an empty pipe, then read(2) will block until data is available.  If a process attempts to write to a full pipe (see below), then write(2) blocks until sufficient data has been read from the pipe to allow the write to complete. However, this behaviour can be altered to Nonblocking I/O by setting the flag 0_NONBLOCK.
A pipe is a data stream, which implies that there is no message boundary. This means that we can read a block of data of any size, irrespective of size of data written to the pipe. It is also not possible to randomly access the data using lseek().
The blocking behavior can be utilized to implement a simple signalling/synchronization mechanism (sleep/wakeup) between 2 related processed.
 
read(2):

    #include <unistd.h>
    ssize_t read(int fd, void *buf, size_t count);

write(2):

    #include <unistd.h>
    ssize_t write(int fd, const void *buf, size_t count);

## Pipe Capacity:
A pipe has limited capacity. If the pipe is full, then a write(2) will block or fail, depending on whether the O_NONBLOCK flag is set. Applications should not rely on a particular capacity: an application should be designed so that a reading process consumes data as soon as it is available, so that a writing process does not remain blocked.
The capacity can be queried and set using the fcntl(2) F_GETPIPE_SZ and F_SETPIPE_SZ operations.

## PIPE_BUF:
As per Posix  writes of less than PIPE_BUF bytes must be atomic: the output data is written to the pipe as a contiguous sequence.  Writes of more than PIPE_BUF bytes may be nonatomic: the kernel may interleave the data with data written by other processes.  POSIX.1 requires PIPE_BUF to be at least 512 bytes. On linux it is 4096 bytes.
