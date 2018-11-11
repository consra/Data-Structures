#include "task3.h"

void task3(FILE *f,FILE **g,int start_ferma)
{
	char *buff = (char*)malloc(sizeof(char)*100);
	int noduri;
	fgets(buff,10,f);
	sscanf(buff,"%d",&noduri);
	fgets(buff,1000,f);

	//retin linia cu ferme
	char *vector_ferme = strdup(buff);
	int muchii,i,poz_x,poz_y,cost;
	fgets(buff,1000,f);
	graf *gf = init(noduri);
	sscanf(buff,"%d",&muchii);

	//creare matrice de adiacenta
	for(i = 0;i < muchii;i++)
	{
		fgets(buff,1000,f);
		sscanf(buff,"%d %d %d",&poz_x,&poz_y,&cost);
		add_con(gf,poz_x,poz_y,cost);
	}
	fgets(buff,1000,f);
	int nr_comenzi;

	//citesc nr de comenzile
	sscanf(buff,"%d",&nr_comenzi);
	Vector v[nr_comenzi];
	int nr = 0;

	//citesc comenzile intr-o structura Vector
	while(fgets(buff,1000,f) != NULL && strlen(buff) > 3)
	{
		sscanf(buff,"%d %d",&(v[nr].ferma),&(v[nr].node));
		v[nr++].vazut = 0;
	}
	int dist,best = 100000;

	//ca sa mai reduc din operatii memorez matricea cu distante
	int ** distance = Simple_Floyd(gf);

	//in aux retin mereu drumurile ce se formeaza
	char *aux = (char*)malloc(sizeof(char)*10000);

	//in path retin mereu variantele finale ale drumurilor
	char *path = (char*)malloc(sizeof(char)*10000);
	int next_ferma;

	for(i = 0 ; i < nr ; i++)
	{
			/*retin cea mai buna ferma de start si calculez
			distanta de la ferma de inceput la ferma de start si apoi la
			prima comanda */
			next_ferma = Floyd_Warshall_task3(gf,vector_ferme,start_ferma,v[i]);
			dist = distance[start_ferma][next_ferma] +
						 distance[next_ferma][v[i].node];
			int diff = distance[start_ferma][v[i].node];

			//pentru cazul in care ferma de start e si cea mai avantajoasa ferma
			if(is_that_ferma(vector_ferme,start_ferma,v[i].ferma) == 1 && diff < dist)
			{
				dist = diff;
			}
			else
			{
				if(start_ferma != next_ferma)
					sprintf(aux,"%d ",next_ferma);
			}

		//generez toate drumurile posibile pentru a vedea cea mai buna solutie
		task3_Floyd(gf,distance,vector_ferme,v,nr,&v[i],dist,&best,&aux,&path);

		//sterg tot ce e in aux
		aux[0] = '\0';
	}
	//pun la inceputului lui ferma de start si drumul cel mai avantajos
	sprintf(aux,"%d %s",start_ferma,path);
	int current ,next = 0,s = 0;
	char * p = strchr(aux,' ');

	//afisez pentru drumul cel mai avantajos intreaga cale
	while(p != NULL && strlen(aux) > 1 && sscanf(aux,"%d %d",&current,&next) == 2)
	{
		p = strchr(aux,' ');
		s += Dijkstra(gf, current ,next,g);
		aux = p + 1;
	}
	fprintf(*g,"%d\n",next);
	fprintf(*g,"%d\n",best);
	fclose(*g);
	free(buff);
	free(path);
	//free(aux);
	free(vector_ferme);
	for(i = 0 ; i < gf->dim; i++)
		free(distance[i]);
	free(distance);
}
//determina daca solutia a ajuns la capat
int is_ready(Vector v[],int dim)
{
	int i,nr = 0;
	for(i = 0; i < dim; i++)
	{
		if(v[i].vazut == 0)
			nr++;
	}
	return nr;
}

//generarea tuturor solutiilor posibile si gasirea celui mai bun
void task3_Floyd(graf *g,int ** mat,
	char *buff,Vector v[],int dim,Vector
		*inc,int dis,int *min,char **vec,char **path)
{
	//cazul cand solutia a ajuns la final
	if(is_ready(v,dim) == 1)
	{
		  Vector aux;
			aux.node = -1;
			aux.ferma = -1;
			int next_ferma = Floyd_Warshall_task3(g,buff,inc->node,aux);
			int last = mat[inc->node][next_ferma];
			dis = dis + last;

			//testez daca solutia prezenta e mai buna decat cea retinuta
			if(dis < *min)
			{
				*min = dis;
				strcpy(*path,*vec);
				sprintf(*path,"%s%d ",*path,inc->node);
				sprintf(*path,"%s%d ",*path,next_ferma);
			}
			return;
	}
	int i,numb = dis;

	//daca solutia un curs de creare are dist mai mare decat distanta solutie
	//optime ma opresc
	if(dis > *min) return;
	char *string = (char*)calloc(1000,sizeof(char));
	sprintf(*vec,"%s%d ",*vec,inc->node);

	inc->vazut = 1;
	for(i = 0; i < dim ; i++)
	{
		if(v[i].vazut == 0 )
		{
			//gasesc cea mai avantajoasa ferma dintre inc->node si v[i]
			int next_ferma = Floyd_Warshall_task3(g,buff,inc->node,v[i]);
			int a = mat[inc->node][next_ferma];
			int b = mat[next_ferma][v[i].node];
			dis = dis + a + b;

			sprintf(string,"%s%d ",*vec,next_ferma);
			task3_Floyd(g,mat,buff,v,dim,&v[i],dis,min,&string,path);
			v[i].vazut = 0;
			strcpy(string,*vec);
			dis = numb;
		}
	}
	free(string);
	//este necesara aceasta atribuire pentru generarea tuturor permutarilor
	inc->vazut = 0;
}

//verifica daca posibile_ferm este o ferma de tipul needed_ferm
int is_that_ferma(char *buff,int possible_ferm,int needed_ferm)
{
		int i,nr = 0;
		if(buff == NULL) return -1;
		if(possible_ferm > (strlen(buff)-1)/2 ) return 0;
		char *aux = buff;
		for(i = 0;i < strlen(buff); i++)
		{
			if(buff[i] == ' ')
			{
				aux++;
				continue;
			}
			else
			{
				if(buff[i] == '0' )
				{
					nr++;
					aux++;
				}
				else
				{
					if(nr == possible_ferm)
					{
						char *p = strchr(aux,' ');
						if(p == NULL)
							p = strchr(aux,'\n');
						char *q = (char*)calloc(20,sizeof(char));
						strncpy(q,aux,p-aux);
						int ferm = 0;
						if(sscanf(q,"%d",&ferm) != 1)
							puts("aici");
						free(q);
						if(ferm == needed_ferm)
					 		return 1;
						return 0;
					}
					aux++;
					nr++;
				}
			}
		}
		return 0;
}

//imi determina cea mai avantajoasa ferma pe drumul dintr src si next
int Floyd_Warshall_task3(graf *g,char *buff,int src,Vector next)
{
	//un floyd normal
	int n = g->dim;
	int i, j, k;
	int **dist = (int **) malloc (n * sizeof(int *));
	for (i = 0; i < n; i++)
		dist[i] = (int *) malloc (n * sizeof(int));
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
		{
			if(g->mat[i][j] == 0)
				dist[i][j] = INT_MAX;
			else
				dist[i][j] = g->mat[i][j];
		}
	}
	for (k = 0; k < n; k++)
	{
		for (i = 0; i < n; i++)
		{
			for (j = 0; j < n; j++)
			{
				if (dist[i][j] > (dist[i][k] + dist[k][j])
				&& (dist[i][k] != INT_MAX) && (dist[k][j] != INT_MAX))
					dist[i][j] = dist[i][k] + dist[k][j];
			}
		}
	}
	int min = INT_MAX;
	int sum ,poz;
	if(next.node != -1)
	{
		for(k = 0; k < n; k++)
		{
				sum = 0;

				if(is_that_ferma(buff,k,next.ferma) == 1
					&& dist[src][k] != 0 && dist[src][k] != INT_MAX
				  && dist[k][next.node] != 0 && dist[k][next.node] != INT_MAX)
				{
					sum = sum + dist[src][k] + dist[k][next.node];
					if(sum < min)
					{
						min = sum;
						poz = k;
					}
				}
		}
	}
	else
	{
		for(k = 0; k < n; k++)
		{
				sum = 0;
				if(is_ferma(buff,k) == 1 && dist[src][k] != 0)
				{
					sum = sum + dist[src][k];
					if(sum < min)
					{
						min = sum;
						poz = k;
					}
				}
		}
	}
	for (i = 0; i < n; i++)
		free(dist[i]);
	free(dist);
	return poz;
}

//floydul acesta imi returneaza matricea cu distantele grafului
//pentru a reduce numarul de operatii
int **Simple_Floyd(graf *g)
{
	int n = g->dim;
	int i, j, k;
	int **dist = (int **) malloc (n * sizeof(int *));
	for (i = 0; i < n; i++)
		dist[i] = (int *) malloc (n * sizeof(int));
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
		{
			if(g->mat[i][j] == 0)
				dist[i][j] = INT_MAX;
			else
				dist[i][j] = g->mat[i][j];
		}
	}
	for (k = 0; k < n; k++)
	{
		for (i = 0; i < n; i++)
		{
			for (j = 0; j < n; j++)
			{
				if (dist[i][j] > (dist[i][k] + dist[k][j])
				&& (dist[i][k] != INT_MAX) && (dist[k][j] != INT_MAX))
					dist[i][j] = dist[i][k] + dist[k][j];
			}
		}
	}
	return dist;
}
