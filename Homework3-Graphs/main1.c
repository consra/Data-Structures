#include "task1.h"
#include "task2.h"
#include "task3.h"

int main(int argc , char **argv)
{
	FILE *input,*output;
	input = fopen(argv[1],"r");
	output = fopen(argv[2],"w");
	if(!input) return -1;

	char buff[3];
	int alg;
	fgets(buff,10,input);

	//citesc valoare ce determina ce task trebuie facut
	sscanf(buff,"%d",&alg);

	//taskul1
	if(alg == 1)
	{
		task1(input,&output);
	}

	//taskul2
	else if(alg == 2)
	{
		int start_ferm;
		char *buffer = (char*) malloc(sizeof(char)*100);

		//parcurg tot fisierul pentru a determina valoarea fermei de start
		while(fgets(buffer,100,input) != NULL)
		{
				sscanf(buffer,"%d%*s",&start_ferm);
		}

		//mut cursorul la inceputul fisierului
		rewind(input);
		fgets(buffer,100,input);
		task2(input,&output,start_ferm);
		free(buffer);
	}

	//taskul3
	else if(alg == 3)
	{
			//aplic acceasi ideea ca la 2 pentru determinarea fermei de start
			int start_ferm;
			char *buffer = (char*) malloc(sizeof(char)*100);
			while(fgets(buffer,100,input) != NULL)
			{
					sscanf(buffer,"%d%*s",&start_ferm);
			}
			rewind(input);
			fgets(buffer,100,input);
			task3(input,&output,start_ferm);
			free(buffer);
	}
	return 0;
}
