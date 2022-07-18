#include <unistd.h>
#include <sys/wait.h>
#include <iostream>

#define READ_END 0
#define WRITE_END 1

int main() {
  int fd[2];
  int result_dummy = 0;
  pipe(fd);
  pid_t cPid;
  cPid = fork();
  if (cPid == 0) {  // child
    close(fd[READ_END]);
    std::cout << "Hello From Child" << std::endl;
    write(fd[WRITE_END], &result_dummy, sizeof(result_dummy));
    close(fd[WRITE_END]);
    exit(0);
  } else if (cPid > 0) {  // parent
    close(fd[WRITE_END]);
    read(fd[READ_END], &result_dummy, sizeof(result_dummy));
    close(fd[READ_END]);
    std::cout << "Hello From Parent" << std::endl;
    wait(NULL);
  } else {
    printf("fork() failed\n");
    return -1;
  }
  return 0;
}
