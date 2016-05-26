#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#include <pwd.h>
#include <string.h>

void prt(char** c)
{
	int i=0;
	while(c[i]!=NULL)
	{
		printf("%s\n",c[i]);
		i++;
	}
}


char* lireCommande()
{
	int i=0;
	char* buff=malloc(sizeof(char)*100);
	if(fgets(buff, 100, stdin)==NULL){
		return NULL;
	}
	while(buff[i]!='\n'){
		i++;
	}
	buff[i]='\0';
	return buff;
}


char** splitCommande(char*c)
{
	char** tab=malloc(sizeof(char*)*10);
	
	int i, f=0;
	int j=0;
	int k=0;
	for(i=0; i<10; i++){
		tab[i]=malloc(sizeof(char)*100);
	}
	i=0;
	while(c[i]!='\0')
	{
		if(c[i]!=' ')
		{
			f=0;
			tab[j][k]=c[i];
			k++;
		}
		i++;
		if(c[i]==' '&&f!=1)
		{
			tab[j][k]='\0';
			j++;
			k=0;
			f=1;
		}
		
		
	}
	tab[j][k]='\0';
	for(i=j+1; i<10;i++)
	{
		tab[i]=(char*)NULL;
	}
	return tab;
}

void ouvrir(char** c)
{
	int i,j=1,k;
	char dir[50];
	pid_t pid =fork();
	if(pid == 0)
	{
			
		if(execvp(c[0],c)==-1)
		{
			exit(2);
		}
	}
	else
	{
		if(!strcmp(c[0], "cd")){
			getcwd(dir, 50);
			i=strlen(dir);
			if(!strcmp(c[j],".."))
			{
				
				do
				{
					dir[i]='\0';
					i--;
				}while(dir[i]!='/');
				j++;
			}else
			{
				strcat(dir, "/");
				strcat(dir, c[j]);
			}
			if(chdir(dir))
				printf("impossible de cd\n");
		}
		wait(NULL);
	}
}




int main(char* argv[])
{
	int i, j;
	char* c;
	char** tabc;
	char k[30], user[150],dir[50];
	int a=-1;
	struct passwd *p= getpwuid(getuid());
	gethostname(k, 30);
	strcpy(user, p->pw_name);
	strcat(user, "@");
	strcat(user, k);
	strcat(user, ": ");
	while(a!=0)
	{
		getcwd(dir, 50);
		i=strlen(dir);
		while(dir[i]!='/') i--;
		j=i;
		do{
			j++;
			dir[j-i-1]=dir[j];
		}while(dir[j]!='\0');
		printf("%s%s> My shell:", user, dir);
		c=lireCommande();
		if(c==NULL) {printf("\n"); a=0;}
		else {tabc=splitCommande(c);
		ouvrir(tabc);
		
		free(c);
		for(i=0;i<10;i++)
			free(tabc[i]);
		free(tabc);
		}
	}
}
