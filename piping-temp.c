
        /* piping - first process */
        else if (strcmp(input[i], "|") == 0) {
          printf("i found a pipe");
          first_cycle = 1;
          already_executed = 1;

          dup2(pipefd[1], 1);
          close(pipefd[0]);
          close(pipefd[1]);
          /* execute the program */
          //execvp(input[i-1], input);
          exit(1);
        }

        /* piping - second process */
        else if (first_cycle == 1) {
          if (strcmp(input[i-1], "|") == 0) {
            printf("\nprogram after pipe\n");
            already_executed = 1;

            dup2(pipefd[0], 0);
            close(pipefd[0]);
            /* execute the program */
            //execvp(input[i], input);
            exit(1);
          }
        }
