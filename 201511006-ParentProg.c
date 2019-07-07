#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>

int frequency1[27] = {0}; // frequency array for child process 1 all elements 0
int frequency2[27] = {0}; // frequency array for child process 2 all elements 0

//function for dividing the string
//which came from child process with stdout
//with delimiter ','
void function(char *text, int arr[])
{

    int i = -1;
    const char s[2] = ",";
    char *token;
    /* get the first token */
    token = strtok(text, s);

    /* walk through other tokens */
    while (token != NULL)
    {
        ++i;
        arr[i] = arr[i] + atoi(token);
        token = strtok(NULL, s);
    }
}

//function for printing the table of frequencies
void table()
{
    printf("-----------------------------------------------------------------------------------------\n");
    for (int i = 0; i < 26; i = i + 2)
    {
        char c = i + 65;
        char c1 = i + 66;
        printf(" %c: %d    ||  %c: %d       \r\n\r\r", c, frequency2[i] + frequency1[i], c1, frequency2[i + 1] + frequency1[i + 1]);
    }
    printf("-----------------------------------------------------------------------------------------\n");
    printf("Good Bye!!\n");
}

int main()
{

    system("cc 201511006-ChildProg.c -o 201511006-ChildProg");
    pid_t child_pid1, child_pid2; // process ids of child processes
    FILE *fp;
    char text[80], file_name1[20], file_name2[20]; //text for reading from childs with stdout
    char file_name[] = "file.txt";
    int P_to_C_1[2];   //parent to child1 parent writes child reads
    int C_to_P_1[2];   //child1 to parent child writes parent reads
    int stdout_backup; //to restore stdout after child process works
    int status;
    char *const envp[] = {NULL};
    // P_to_C_1[0] child1 read
    // P_to_C_1[1] parent write
    pipe(P_to_C_1); // convert variable to "pipe" it must be done before fork()

    // C_to_P_1[0] parent read
    // C_to_P_1[1] child1 write
    pipe(C_to_P_1); // convert variable to "pipe"

    child_pid1 = fork();

    if (child_pid1 == -1)
    {
        perror("fork");
        return -1;
    }

    //P0 parent process
    if (child_pid1 > 0)
    {
        fp = fopen(file_name, "w");
        fprintf(fp, "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.");

        fclose(fp);
        int P_to_C_2[2]; //parent to child2 parent writes child reads
        int C_to_P_2[2]; //child2 to parent child writes parent reads

        // P_to_C_1[0] child2 read
        // P_to_C_1[1] parent write
        pipe(P_to_C_2); // convert variable to "pipe"

        // C_to_P_1[0] parent read
        // C_to_P_1[1] child2 write
        pipe(C_to_P_2); // convert variable to "pipe"

        close(P_to_C_1[0]); //child1 read close
        close(C_to_P_1[1]); // child1 write close

        child_pid2 = fork();

        if (child_pid2 == -1)
        {
            perror("fork");
            return -1;
        }

        //P0  parent process continue..
        if (child_pid2 > 0)
        {

            close(P_to_C_2[0]); //parent to child read end close
            close(P_to_C_1[0]); //parent to child read end close

            write(P_to_C_1[1], file_name, strlen(file_name) + 1); //write file name to pipe for child1
            write(P_to_C_2[1], file_name, strlen(file_name) + 1); //write file name to pipe for child2

            waitpid(child_pid2, &status, 0); //wait until child2 executes
            read(C_to_P_2[0], text, 81);     // read from read-end of pipe
            function(text, frequency2);

            waitpid(child_pid1, &status, 0); //wait until child1 executes
            read(C_to_P_1[0], text, 81);     // read from read-end of pipe
            function(text, frequency1);
            table();
        }

        //P2 child2
        if (child_pid2 == 0)
        {
            //close every pipe between child1 and parent because we don't gonna use them
            close(P_to_C_1[0]);
            close(P_to_C_1[1]);
            close(C_to_P_1[0]);
            close(C_to_P_1[1]);

            close(P_to_C_2[1]); // parent to child write end close
            close(C_to_P_2[0]); //child to parent read close

            read(P_to_C_2[0], file_name2, strlen(file_name) + 1); // read from read-end of the parent to child pipe
                                                                  //store stdout
            stdout_backup = dup(1);
            // What used to be stdout will now go to the pipe.
            dup2(C_to_P_2[1], 1);

            char *argv2[] = {"./201511006-ChildProg", file_name2, "p", "z", NULL};
            execvp("./201511006-ChildProg", argv2);
            perror("exec");

            sleep(50);
            //Restore stdout
            dup2(stdout_backup, 1);
            close(stdout_backup);

            //}
        }
    }

    //P1 child1
    if (child_pid1 == 0)
    {

        close(P_to_C_1[1]);                                   // parent to child write end close
        close(C_to_P_1[0]);                                   //child to parent read close
        read(P_to_C_1[0], file_name1, strlen(file_name) + 1); // read from read-end of the parent to child pipe

        //store stdout
        stdout_backup = dup(1);
        // What used to be stdout will now go to the pipe.
        dup2(C_to_P_1[1], 1);
        char *argv1[] = {"./201511006-ChildProg", file_name1, "a", "o", NULL};
        execvp("./201511006-ChildProg", argv1);
        perror("exec");

        sleep(50);
        //Restore stdout
        dup2(stdout_backup, 1);
        close(stdout_backup);
    }

    return 0;
}
