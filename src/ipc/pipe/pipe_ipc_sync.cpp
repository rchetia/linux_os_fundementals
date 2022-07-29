#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/wait.h>
#include <iostream>

#define READ_END 0
#define WRITE_END 1

#define SLEEP_AT_PARENT 0
#define SLEEP_AT_CHILD  1

/*
 * Demo program to demonstrate signalling between parent and child.
 * Child (do some operation) -> Signal Parent about completion of operation->
 * parent wakes up and exits.
 */
int signal_between_parentchild(int where_to_sleep) {
  int fd[2];
  int result_dummy = 0;
  assert(0 == pipe(fd));
  pid_t cPid;
  cPid = fork();
  if (cPid == 0) {  // child
    close(fd[READ_END]);
    if (where_to_sleep == SLEEP_AT_CHILD) {
      sleep(3); // Sleep as placeholder for some operations
    }
    std::cout << "Hello From Child" << std::endl;
    write(fd[WRITE_END], &result_dummy, sizeof(result_dummy)); // write blocks till all data is consumed @read end
    close(fd[WRITE_END]);
    exit(0);
  } else if (cPid > 0) {  // parent
    if (where_to_sleep == SLEEP_AT_PARENT) {
      sleep(3); // Sleep as placeholder for some operations
    }
    close(fd[WRITE_END]);
    read(fd[READ_END], &result_dummy, sizeof(result_dummy)); // read blocks till data is available @write end
    close(fd[READ_END]);
    std::cout << "Hello From Parent" << std::endl;
    wait(NULL);
  } else {
    std::cout << "fork() failed" << std::endl;
    return -1;
  }
  return 0;
}

/*
 * main test.
 */
int main(int argc, char *argv[]) {
  if (argc < 2) {
    return -1;
  }
  int myarg = atoi(argv[1]);
  if (0 == myarg) {
    return signal_between_parentchild(SLEEP_AT_CHILD);
  } else if (1 == myarg) {
    return signal_between_parentchild(SLEEP_AT_PARENT);
  } else {
  }
  return 0;
}
