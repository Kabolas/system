#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>
#include <pwd.h>
#include <string.h>
#include <utime.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <time.h>


void prt(char** c)
{
	int i=0;
	while(c[i]!=NULL)
	{
		printf("%s\n",c[i]);
		i++;
	}
}

void touch(char** c){
	int cpt;
	char dir[50];
	struct stat buf;
	time_t t;
	struct utimbuf* u=malloc(sizeof(struct utimbuf));
	int k=1,i, flagm=0;
	if(c[k][0]=='-'){
		flagm=1;
		k++;
	}
	cpt=open(c[k],O_WRONLY|O_CREAT);
	if(!flagm){
		utime (c[k], NULL);
	}
	else {
	getcwd(dir, 50);
	strcat(dir,"/");
	strcat(dir, c[k]);
	stat(dir,&buf);
	time(&t);
	u->actime=buf.st_atime;
	u->modtime=t;
	utime (c[k], u);
	}
	free(u);
	close(cpt);
}

void cat(char** c)
{
	int cpt, k=1,flag=0,j=1,bob=1;
	char b;
	while(c[j]!=NULL)
	{
		if(!strcmp(c[j],"-n"))
		flag=1;
		j++;
	}
	while(c[k]!=NULL)
	{
		if(!(flag && c[k][0]=='-'))
		{	
			if(flag)	
			printf("%d ",bob);
			cpt=open(c[k],O_RDONLY);
			while(read(cpt, &b, 1)>0)
			{
				printf("%c", b);
				if(flag && b=='\n')
				{
					printf("%d ",bob);
					bob++;
				}
			}
			close(cpt);
			k++;
		}
		else
		k++;
		printf("\n");
	}
}

void history(char** c,int boby)
{
	int bob=0,k=0;
	int flag = 0;
	int cpt=open("history.txt",O_RDONLY);
	while(c[k]!=NULL)
	{
		if(!strcmp(c[k],"-n"))
		flag=1;
		k++;
	}
	char* buf=malloc(sizeof(char));
	if(flag)
	{
		while(read(cpt, buf, 1)>0 && bob!=boby)
		{
			if(*buf == '\n')
			{
				bob++;
			}
		}
		while(read(cpt, buf, 1)>0 && *buf != '\n'){}
		bob++;
		printf("%d ",bob);
	}
	while(read(cpt, buf, 1)>0)
	{
		printf("%s", buf);
		if(*buf == '\n')
		{
			bob++;
			printf("%d ", bob);
		}
	}
	printf("\n");
	free(buf);
	
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

void ouvrir(char** c,int bob)
{
	int i,j=1,k;
	char dir[50];
	if(!strcmp(c[0], "history")){
		history(c,bob);
	}
	else if(!strcmp(c[0], "cat")){
		cat(c);
	}
	else if(!strcmp(c[0], "touch")){
		touch(c);
	}
	else if(!strcmp(c[0], "cpy")){
		char** k=malloc(sizeof(char*)*4);
		int f;
		for(f=0; f<4; f++){
			k[f]=malloc(sizeof(char)*100);
		}
		getcwd(k[1],60);
		strcat(k[1],"/");
		strcat(k[1],c[1]);
		getcwd(k[2],60);
		strcat(k[2],"/");
		strcat(k[2],c[2]);
		pid_t pid =fork();
		if(pid == 0)
		{
			if(execv("cpy",k)==-1)
			{
				printf("erreur exécution cpy\n");
			}
			exit(2);
		}
		else
		{
			wait(NULL);
		}
		for(f=0; f<4; f++){
			free(k[f]);
		}
		free(k);
	}
	else{
		char *path = getenv("PATH");
		int nb2p=0, i=0;
		while(path[i]!='\0'){
			if(path[i]==':')
				nb2p++;
			i++;
		}
		char **parse= malloc((nb2p+1)*sizeof(char*));
		for(i=0; i<nb2p+1; i++)
		{
			parse[i]=malloc(100*sizeof(char));
		}
		i=0;
		int b=0, q=0;
		int exist=0;
		while(path[i]!='\0'){
			if(path[i]==':'){
				parse[b][q]='/';
				b++;
				q=0;
				i++;
			}else{
				parse[b][q]=path[i];
				i++;
				q++;	
			}
		}
		for(q=0; q<nb2p && !exist; q++){
			if(!access(strcat(parse[q], c[0]), F_OK))
				exist = 1;
		}
		pid_t pid =fork();
		if(pid == 0)
		{
			i=0;
			int encoreunpipe=0;
			char ** bUf=malloc(sizeof(char*)*50);
			int o;
			for(o=0;o<50;o++)
			{
				bUf[o]=malloc(sizeof(char)*100);		
			}
			while(c[i]!=NULL)
			{
				do{
					if(c[i][0]=='>')
					{
						char* fichier=malloc(sizeof(char)*50);
						strcpy(fichier,c[i+1]);
 						int redir=open(fichier,O_RDWR | O_CREAT);
						fchmod(redir, S_IRWXU | S_IRWXG | S_IROTH | S_IWOTH);
						dup2(redir, 1);
						c[i]=NULL;
						c[i+1]=NULL;
					}
					else if(c[i][0]=='|')
					{
						// on copie ce que sort la console dans un ficher, on lance ensuite le programme avec pour arguments le texte enregistré sur le fichier
						int redir=open("temp.txt",O_WRONLY | O_CREAT|O_TRUNC);
						fchmod(redir, S_IRWXU | S_IRWXG | S_IROTH | S_IWOTH);
						char** red =malloc(sizeof(char*)*50);
						if(!encoreunpipe)
						{
							for(j=0;j<10;j++)
								{
								red[j]=malloc(sizeof(char)*50);
								}
							for(k=0;k<i;k++)
								{
									red[k]=c[k];
								}
							while(k<10)
							{
								red[k]=NULL;
								k++;
							}
						}
						pid_t pid2 =fork();
						if(pid2 == 0)
						{	
							if(redir!=-1)
								dup2(redir,1);
							if(!encoreunpipe)
							{
								execvp(red[0],red);
							}
							else
							{
								encoreunpipe=0;
								execvp(c[i+1],bUf);
							}
							exit(2);
						}
						else{
							wait(NULL);
						}
						FILE* reredir=fopen("temp.txt", "r");
						for(o=0;o<50;o++)
						{
							//if(red[o]!=NULL)
							//	free(red[o]);
							if(bUf[o]!=NULL)
								{free(bUf[o]);
							bUf[o]=malloc(sizeof(char)*100);}
						}
						//if(red!=NULL)
						//	free(red);
						o=1;
						int p=0;
						if(reredir != NULL)
						{
							while(bUf[o]!=NULL)
							{
								bUf[o][p]=fgetc(reredir);
								if(feof(reredir))
								{
									break;
								}								
     		 						else if(bUf[o][p] == '\n')
								{
									bUf[o][p] = '\0';
									 o++;
									p=0;
								}
								else
									{p++;}
							}
						fclose(reredir);
						}
						bUf[o]=NULL;
						char* truc= malloc(sizeof(char) * 50);
						int v;
						for(v = i+1; c[v]!=NULL&&v<10;v++)
						{
							if(c[v][0]=='|')
								{encoreunpipe =1;
								break;}	
							else	encoreunpipe =0;
						}
						if(!encoreunpipe){
							strcpy(truc,c[i+1]);
							execvp(truc,bUf);
							exit(2);
						}
					}
					if(encoreunpipe)
						i++;
				}while(encoreunpipe);
				i++;	
			}
			for(o=0;o<50;o++)
			{
				if(bUf[o]!=NULL)
				free(bUf[o]);
			}
			free(bUf);
			execv(strcat(parse[q], c[0]),c);
			exit(2);
		}
		else
		{
			//if(redir!=0)
			//	close(redir);
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
			if(!strcmp(c[0], "exit"))
			{
				if(c[1]!=NULL)
				k=c[1][0]-'0';
				else k=2;
				exit(k);
			}
			wait(NULL);
		}
	}
}




int main(char* argv[])
{
	int i, j;
	char* c;
	char** tabc;
	char k[30], user[150],dir[50];
	int a=-1;
	int cpt;
	char b;
int bob=0;
	struct passwd *p= getpwuid(getuid());
	gethostname(k, 30);
	strcpy(user, p->pw_name);
	strcat(user, "@");
	strcat(user, k);
	strcat(user, ": ");
	cpt=open("history.txt",O_RDWR|O_CREAT);
	while(read(cpt, &b, 1)>0){
	if(b =='\n')
		bob++;
	}
	fchmod(cpt, S_IRWXU | S_IRWXG | S_IROTH | S_IWOTH);
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
		if(c!=NULL){
		write(cpt,"\n",1);
		write(cpt,c,strlen(c));
		}
		if(c==NULL)
			{printf("\n"); a=0;}
		else
			{tabc=splitCommande(c);
		ouvrir(tabc,bob);
		free(c);
		for(i=0;i<10;i++)
			free(tabc[i]);
		free(tabc);
		}
	}
	close(cpt);
}
