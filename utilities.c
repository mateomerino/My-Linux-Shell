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

int pipe_test(char* buffer)
{
    int pipe= 0;
    for(int i=0;i<strlen(buffer);i++){
            if(buffer[i]=='|'){
                pipe=1;
                break;
                }
        }
    return pipe;
}

int background_test(char* buffer)
{
    int background=0;
     for(int i=0;i<strlen(buffer);i++)
     {
        if(buffer[i]=='&')
        {
            background=1;
            break;
        }
    }
    return background;
}
/**
 * @brief Esta función se encarga de quitar los
 *        espacios en los extremos de un string.
 *
 * @details Recorremos el string desde el comienzo hasta
 *          encontrar una letra. A partir de allí,
 *          recorremos el string desde el final hasta
 *          encontrar una letra. Antes de esta letra
 *          final, se coloca el caracter de fin de string.
 *          Si el string está completamente vacío, se lo
 *          retorna tal y como vino.
 *
 * @param str String a modificar.
 *
 * @returns El string sin espacios ni tabulaciones
 *          ni al comienzo ni al final.
 */
char *strtrim(char *str)
{
    while ((*str == ' ') || (*str == '\t'))
        str++;

    if (*str == 0)
        return str;

    char *end = str + strlen(str) - 1;

    while ((end > str) && ((*end == ' ') || (*end == '\t')))
        end--;

    end[1] = '\0';

    return str;
}
/**
 * @brief Esta función se encarga de quitar el simbolo
 *        el simbolo pipe "|" del comienzo de un string
 *
 * @details Recorremos el string desde el comienzo y vamos
 *          "desplazando" en uno cada letra del string
 *          eliminando así el simbolo pipe del comienzo. 
 *
 * @param str String a modificar.
 *
 * @returns El string sin espacios ni tabulaciones
 *          ni al comienzo ni al final.
 */
char *strpipe(char *str)
{
    for (int i = 0; i < strlen(str); i++)
    {
        str[i] = str[i + 1];
        if (i == strlen(str) - 1)
        {
            break;
        }
    }
    return str;
}
/**
 * @brief Esta función crea un array con argumentos
 *        para poder utilizarlos al ejecutar un proceso
 *
 * @details Comenzamos colocando el primer elemento en 
 *          el array y vamos recortando con strtok el resto
 *          de los elementos y agregandolos a nuestro array
 *
 * @param token Los argumentos "recortados".
 * @param args  El array a llenar.
 *
 * @returns Nuestro array de argumentos listo para utilizar.
 */
char **createargs(char *token, char **args)
{
    int i = 0;
    while (token != NULL)
    {
        args[i] = token;
        //printf("%s\n", args[i]);
        //int len = strlen(args[i]);
        //printf("%d\n", len);
        i++;
        token = strtok(NULL, " ");
    }
    strtok(args[i - 1], "\n");
    args[i] = NULL;
    return args;
}
