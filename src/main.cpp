#include <algorithm>
#include <cstdio>
#include <iostream>
#include <map>
#include <sched.h>
#include <signal.h>
#include <string>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>
#include "headers/utils.hpp"
#include "headers/builtin.hpp"
#include "headers/KillProcess.hpp"

int main(int argc, char *argv[]) {
  std::string input;
  auto CurrentDir = PWDFunc();
  while (true) {
    std::cout << CurrentDir << ">>> ";
    std::getline(std::cin, input);
    std::vector<std::string> full_command = split_args(input);
    std::vector<char *> command_char;
    //Pointer pointing to KillProcess function(for later use in signal())
    void (*KPPointer)(int);
    KPPointer = &KillProcess;
    //Check if the vector is empty
    if(full_command.empty()){
        continue;
    }
    //Check if the inserted command is a builtin function(e.g: cd)
    if (IsBCommand(full_command.at(0)) == true) {
        auto& CommandList = BCommands;
        switch (CommandList[full_command.at(0)]) {
            case 1:
                chdir(full_command.at(1).c_str());
                CurrentDir = PWDFunc();
                continue;
            case 2:
                std::cout << PWDFunc() << "\n";
                continue;
            case 3:
                exit(0);
        }
    }
    // Converts full_command into a char * vector
    std::transform(full_command.begin(), full_command.end(),
                   std::back_inserter(command_char), convert);
    full_command.clear();
    //Push command_char a nullptr to make it a null terminated string
    command_char.push_back(nullptr);
    pid_t command = fork();
    //If fork() fails
    if (command < 0) {
      std::cout << "fork() fail \n";
      for (char *ptr : command_char) {
        delete[] ptr;
      }
      command_char.clear();
      continue;
    }
    // Child Process
    else if (command == 0) {
      execvp(command_char.at(0), command_char.data());
      perror("MihShell");
      _exit(1);
    }
    // Parent Process
    else {
      ChildPid = command;
      signal(SIGINT, KPPointer);
      wait(NULL);
    }
    //Deallocate all pointers
    for (char *ptr : command_char) {
      delete[] ptr;
    }
    command_char.clear();
  }
  return 0;
}
