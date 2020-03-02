#include "tsh.h"

using namespace std;

void simple_shell::parse_command(char *cmd, char **cmdTokens)
{
  // TODO: tokenize the command string into arguments
  *cmdTokens = strtok(cmd, " \n");
  char ** p = cmdTokens;
  p++;
  for (int i = 1; i < 25; ++i)
  {
    if ((*p = strtok(NULL, " \n")) != NULL)
    {
      p++;
    }
    else
    {
      break;
    }
  }
}

void simple_shell::exec_command(char **argv)
{
  // TODO: fork a child process to execute the command.
  // parent process should wait for the child process to complete and reap it
  if (argv[0][0] == '.' || argv[0][0] == '/' || argv[0][0] == '~')
  {
    if (fork() == 0)
    {
      if (execv(argv[0], argv) == -1)
      {
        perror("execl error");
        exit(1);
      }
    }
    else
    {
      wait(NULL);
    }
  }
  else
  {
    if (fork() == 0)
    {
      if (execvp(argv[0], argv) == -1)
      {
        cout << argv[0];
        perror("execvp error");
        exit(1);
      }
    }
    else
    {
      wait(NULL);
    }
  }
}

bool simple_shell::isQuit(char *cmd)
{
  // TODO: check for the command "quit" that terminates the shell
  return cmd[0] == 'q' && cmd[1] == 'u' && cmd[2] == 'i' && cmd[3] == 't';
}
