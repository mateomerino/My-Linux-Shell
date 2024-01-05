#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <getopt.h>
//#include<readline/readline.h>
//#include<readline/history.h>
#include <limits.h>
#include <signal.h>
#include "utilities.h"

int job = 0;
void handler(int sig);

void signals_off(void)
{
    signal(SIGINT, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
}
void signals_on(void)
{
    signal(SIGINT, SIG_DFL);
    signal(SIGTSTP, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
}

void execute_cd(char *buffer, int background)
{

    if (strncmp(buffer, "cd -", 4) == 0)
    {
        chdir("..");
    }
    else
    {
        strtok(buffer, " \t");
        char *aux = strtok(NULL, " \t\n");
        char *new_dir;
        new_dir = (char *)malloc(strlen(getcwd(NULL, 0)) + strlen(aux) + sizeof(NULL) + (sizeof(char) * 2));
        strcpy(new_dir, getcwd(NULL, 0));
        strcat(new_dir, "/");
        strcat(new_dir, aux);

        if (chdir(new_dir) != 0)
        {
            perror("Change current directory failed");
        }
        else
        {
            setenv("OLDPWD", getenv("PWD"), 1);
            setenv("PWD", getcwd(NULL, 0), 1);
        }
    }
}

void execute_pipe(char *buffer, int background)
{
    // DECLARO TODAS LAS VARIABLES QUE VOY A UTILIZAR
    pid_t pid;
    int fd[2];
    char *tokenentrada;
    char *tokensalida;
    int i = 0;
    int j = 0;
    char **entradargs;
    char **salidargs;
    char *salida = malloc(strlen(buffer) + sizeof(NULL) + (sizeof(char) * 2));
    char *entrada = malloc(strlen(buffer) + sizeof(NULL) + (sizeof(char) * 2));

    // SEPARO ENTRADA Y SALIDA COMO INDICA EL PIPE "|" Y LOS DEJO ACOMODADOS
    strcpy(salida, buffer);
    strcpy(entrada, buffer);
    strtok(entrada, "|");
    entrada = strtrim(entrada);
    //printf("ENTRADA: %s\n", entrada);
    salida = strstr(salida, "|");
    salida = strtrim(salida);
    strtok(salida, "\n");
    salida = strpipe(salida);
    salida = strtrim(salida);
    //printf("SALIDA: %s\n", salida);
    //int len = strlen(salida);
    //printf("%d\n", len);

    // DECLARO LOS ARREGLOS CORRESPONDIENTES A LOS ARGUMENTOS DE ENTRADA Y SALIDA
    entradargs = (char **)malloc(strlen(getenv("PATH")) + strlen(buffer) + sizeof(NULL) + (sizeof(char) * 2));
    salidargs = (char **)malloc(strlen(getenv("PATH")) + strlen(buffer) + sizeof(NULL) + (sizeof(char) * 2));

    // CREO LOS ARREGLOS DE ARGUMENTOS Y LOS ASIGNO
    tokenentrada = strtok(entrada, " ");
    entradargs = createargs(tokenentrada, entradargs);
    tokensalida = strtok(salida, " ");
    salidargs = createargs(tokensalida, salidargs);


    //Creo el pipe, y notifico en caso de error 
    if (pipe(fd) == -1)
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    //Hago el fork
    pid = fork();

    if(pid==-1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {
        dup2(fd[1], STDOUT_FILENO);
        close(fd[0]);
        close(fd[1]);
        execvp(tokenentrada, entradargs);
        // fprintf(stderr, "Failed to execute '%s'\n", entrada.elements[0]);
        exit(EXIT_SUCCESS);
    }
    else
    {
        pid = fork();
        if(pid==-1)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        else if (pid == 0)
        {
            dup2(fd[0], STDIN_FILENO);
            close(fd[1]);
            close(fd[0]);
            execvp(tokensalida, salidargs);
            // fprintf(stderr, "Failed to execute '%s'\n", salida.elements[0]);
            exit(EXIT_SUCCESS);
        }
        else
        {
            int status;
            close(fd[0]);
            close(fd[1]);
            waitpid(pid, &status, 0);
        }
    }
 
    free(entrada);
    //free(salida);
    free(entradargs);
    free(salidargs);
    return;
}
void execute_process(char *buffer, int background)
{
    char *tokenbuffer;
    int i = 0;
    char **args;
    args = (char **)malloc(strlen(getenv("PATH")) + strlen(buffer) + sizeof(NULL) + (sizeof(char) * 2));
    tokenbuffer = strtok(buffer, " ");
    args = createargs(tokenbuffer, args);
    // El ultimo argumento de mi arreglo es NULL
    // Construyo el path a ejecutar
    int pid = fork();
    if (pid == 0)
    {
        signals_on();
        execvp(tokenbuffer, args);
        exit(EXIT_SUCCESS);
        /*
        char *str = "/";
        char *path = getenv("PATH");
        char *aux;
        aux = (char *)malloc(strlen(getenv("PATH")) + strlen(buffer) + sizeof(NULL) + (sizeof(char) * 2));
        // args=(char **)realloc(args, strlen(getenv("PATH")));
        // printf("%s\n",path);
        char *token;
        token = strtok(path, ":");
        // Voy iterando con todas las posibilidades de rutas donde puede estar el programa a ejecutar
        while (token != NULL)
        {
            // printf("%s\n",token);
            strcpy(aux, token);
            strcat(aux, str);
            strcat(aux, buffer);
            // printf("%s\n",aux);
            execv(aux, args);
            token = strtok(NULL, ":");
        }*/
    }
    else if (pid == -1)
    {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }
    else
    {
        if (background)
        {
            char bg[20] = "";
            sprintf(bg, "%d\n", pid);
            write(1, bg, 20);
        }
        else
        {
            wait(0);
        }
    }
    free(args);
    return;
}

void execute_echo(char *buffer, int background)
{
    int pid = fork();
    if (pid == 0)
    {
        char *arr;
        arr = strstr(buffer, " ");
        arr=strtrim(arr);
        printf("%s\n", arr);
        exit(0);
    }
    else
    {
        if (background == 1)
        {
            job++;
            printf("%s%d%s%d\n", "[", job, "]", pid);
        }
        else
        {
            wait(0);
        }
    }
}

void type_prompt()
{
    static int first_time = 1;
    char hostname[HOST_NAME_MAX + 1];
    char cwd[PATH_MAX];
    if (first_time)
    {
        first_time = 0;
    }
    // Por ejemplo, para el home: _username@hostname:~$
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        gethostname(hostname, HOST_NAME_MAX + 1);
        printf("%s%s%s%s%s%s", getenv("USER"), "@", hostname, ":~", cwd, "$ ");
    }
    return;
}

void execute_clear()
{
    system("clear");
}

void read_command(char *buffer)
{
    int pipe_flag=pipe_test(buffer);
    int backround_flag=background_test(buffer);

    if(backround_flag){strtok(buffer,"&");}

    if (strncmp("clr", buffer, 3) == 0)
    {
        execute_clear();
    }
    else if (strncmp("echo", buffer, 4) == 0)
    {
        execute_echo(buffer, backround_flag);
    }
    else if (strncmp("cd", buffer, 2) == 0)
    {
        execute_cd(buffer, backround_flag);
    }
    else if (pipe_flag == 1)
    {
        execute_pipe(buffer, backround_flag);
    }
    else
    {
        execute_process(buffer, backround_flag);
    }
    return;
}
