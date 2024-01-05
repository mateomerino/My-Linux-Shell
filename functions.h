
extern void type_prompt();
extern void execute_clear();
extern void read_command(char* comando);
extern void execute_echo(char* comando,int background);
extern void execute_cd(char* comando,int background);
extern void execute_process(char* comando,int backgound);
extern void execute_pipe(char* comando,int background);

//SIGNALS

#ifndef SIGNALS_H
#define SIGNALS_H
void signals_off(void);
void signals_on(void);
#endif
