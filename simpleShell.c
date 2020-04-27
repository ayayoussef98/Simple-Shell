#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<signal.h>

char * arg[100];
int size;

//function that writes to log file
void handlingSig(int noof_signal)
{
FILE * f;
f= fopen("signal.log","a+");
fprintf(f,"\nChild process was terminated.\n");
fprintf(f,"Signal Number:%d\n",noof_signal);
fclose(f);
}

//function to split the input taken from user
void takeinput()
{size=0;
char line[100];
int i=0;
char *a;
gets(line);
a=strtok(line," ");
while(a != NULL)
{size++;
arg[i]=a;
a=strtok(NULL," ");
i++; }
if(strcmp(arg[0],"exit")==0)
exit(0);
}

//function to see whether there's an "&" at the end of the taken input or no
int parentandchild()
{
int x;
int flag=0;
x=strcmp(arg[size-1],"&");
if(x==0) //parent with child(parent doesnt wait)(& present)
{
flag=1;
strcpy(arg[size-1],"NULL");
}

else //parent wait(no &)
flag=0;  

return flag;
}


void runShell()
{

int y=strcmp(arg[0],"cd");
if(y!=0){
int result=parentandchild(); //to store flag returned to know whether it has & or no
pid_t id=fork();

if(id<0)
{printf("Failed process");
 return;}

else if(id==0)
execvp(arg[0],arg); //runs the argument

else
{ 
   if(result==0) //if yes, parent should wait for the child to complete its process
{
  signal(SIGCHLD, handlingSig); 
  wait(NULL);
     return;
}
else
signal(SIGCHLD, handlingSig);
  

}}
}

//sees if the enterred command is cd or not, if yes then the directory goes to the given argument(arg[1]) if its present, but if the argument is not cd then 
//the process continues and gets executed by the "execvp"
void changeDirectory()
{
int result;
if(strcmp(arg[0],"cd")==0)
{
result=chdir(arg[1]);
if(result==-1)
printf("\nNo such directory present\n");
}
else {

}
}


int main()
{
  while(1)
{
 takeinput();
 changeDirectory();
 runShell();
}
return 0;
}
