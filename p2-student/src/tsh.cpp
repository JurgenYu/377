#include "tsh.h"

using namespace std;

void simple_shell::parse_command(char *cmd, char **cmdTokens)
{
  // TODO: tokenize the command string into arguments
  cmdTokens[0] = strtok(cmd, " ");
  for (int i = 1; i < 25; ++i)
  {
    char *token = strtok(NULL, " ");
    if (token != NULL)
    {
      cmdTokens[i] = token;
    }
    else
    {
      cmdTokens[i] = NULL;
      break;
    }
  }
}

void simple_shell::exec_command(char **argv)
{
  // TODO: fork a child process to execute the command.
  // parent process should wait for the child process to complete and reap it
  char p[] = "ls";
  if (p[0] == '.' || p[0] == '/' || p[0] == '~')
  {
    if (fork() == 0)
    {
      if (execv(p, argv) == -1)
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
      if (execvp(p, argv) == -1)
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
