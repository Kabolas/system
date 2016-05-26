#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>


int cpy ( char* argv, char* argv2)
{
	int f1,f2;
	int test;
	struct stat temp;
	char c[1];
	f1=open(argv,O_RDONLY);
	if(f1==-1)
	{
		printf("erreur fichier lu");
		return -1;
	}

	f2=open(argv2,O_WRONLY);
		if (f2 == -1)
		{
			f2=open(argv2,O_WRONLY|O_CREAT);
		}		
		if (f2 == -1)
			{
				printf("error open\n");
				return -1;
			}
		test=read(f1,c,1);
		while(test>0)
		{		
			test=write(f2,c,1);
			if(test < 0)
			{
				printf("error write\n");
				return -1;
			}
			test=read(f1,c,1);
		}
	fstat(f1,&temp);
	fchmod(f2, temp.st_mode);
	close(f1);
	close(f2);
	printf("tout est ok");
}





int cpyd (char* argv, char*argv2)
{
	DIR* or;
	DIR* des;
	struct stat s,d; 
	char riri[500],fifi[500];
	char newname[50];
	int i=0,j=0;
	or = opendir(argv);
	des = opendir(argv2);
	if(or==NULL)
	{
		strcpy(riri,argv);
		strcpy(fifi,argv2);
		if(des!=NULL)
		{
			while(riri[i]!='\0')
			{				
				i++;
			}
			while(riri[i]!='/')
			{
				i--;
			}
			i++;
			while(riri[i]!='\0')
			{
				newname[j]=riri[i];
				j++;
				i++;
			}
			newname[j]='\0';
			strcat(fifi,"/");
			strcat(fifi,newname);
		}
			cpy(riri,fifi);

	}
	else
	{
	
		if(des==NULL)
		{	
			stat(argv,&d);
			mkdir(argv2,d.st_mode);
		}
		des = opendir(argv2);
		if(des==NULL)
		printf("error des\n");
		struct dirent* read;
		read=readdir(or);
		read=readdir(or);/*les deux fois dans le vide*/
		read=readdir(or);
		while(read!=NULL)
		{
			strcpy(riri,argv);
			strcpy(fifi,argv2);
			strcat(riri,"/");
			strcat(fifi,"/");
			strcat(riri,read->d_name);
			strcat(fifi,read->d_name);
			stat(riri,&s);
			switch(s.st_mode & S_IFMT)
			{
			case S_IFDIR :
				cpyd(riri,fifi);
				break;
			case S_IFREG :
				cpy(riri,fifi);
				break;
			}
	
	
			read=readdir(or);
		}	
	}
	closedir(or);
	closedir(des);
return 0;
}


int main (int argc, char *argv[])
{
	printf("\n");
	cpyd(argv[1],argv[2]);
	printf("\n");
	return 0;
}




