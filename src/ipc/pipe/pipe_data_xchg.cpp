#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/wait.h>
#include <iostream>

#define READ_END 0
#define WRITE_END 1

/*
 * Demo program to demonstrate data exchange between parent and child.
 */
template<typename T> 
void basic_dataxchange(T value) {
  int fd[2];
  T result;
  assert(0 == pipe(fd));
  pid_t cPid;
  cPid = fork();
  if (cPid == 0) {  // child
    close(fd[READ_END]);
    write(fd[WRITE_END], &value, sizeof(T)); // write value of type T
    close(fd[WRITE_END]);
    exit(0);
  } else if (cPid > 0) {  // parent
    close(fd[WRITE_END]);
    read(fd[READ_END], &result, sizeof(T)); // read value of type T
    close(fd[READ_END]);
    std::cout << "result = " << result << std::endl;
    std::cout << "value = " << value << std::endl;  
    assert(result == value);
    wait(NULL);
  } else {
    std::cout << "fork() failed" << std::endl;
    return;
  }
  return;
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
    basic_dataxchange<int>(12345);
    basic_dataxchange<float>(12345.5);
    basic_dataxchange<char>('a');
    basic_dataxchange<double>(12345.5);
    basic_dataxchange<bool>(true);
  } else {
  }
  std::cout << "Tests completed" << std::endl;
  return 0;
}
