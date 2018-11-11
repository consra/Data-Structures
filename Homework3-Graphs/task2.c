#include "task2.h"

void task2(FILE *f,FILE **g,int start_ferma)
{
	char *buff = (char*)malloc(sizeof(char)*100);
	int noduri;
	fgets(buff,10,f);
	sscanf(buff,"%d",&noduri);
	fgets(buff,1000,f);

	//retin linia cu vectorul de caractere intr-un char *
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
	int dest,sum=0;
	fgets(buff,1000,f);
	int v[noduri];
	int nr = 0;

	/* retin toate comenzile intr-un vector pentru
		 ca am nevoie si de comanda  urmatoare */
	while(fgets(buff,1000,f) != NULL && strlen(buff) > 3)
	{
		sscanf(buff,"%*d %d",&dest);
		v[nr++] = dest;
	}

	//calculeaza distanta de la comanda curenta la cea mai aproapiata ferma
	//si apoi de la ferma aleasa la urmatoare comanda
	for(i = 0; i < nr-1;i++)
	{
		sum += ModDijkstra(gf,&start_ferma,&v[i],v[i+1],vector_ferme,g)
				 + ModDijkstra(gf,&v[i],&start_ferma,v[i+1],vector_ferme,g);
	}
	sum += ModDijkstra(gf,&start_ferma,&v[nr-1],INT_MAX,vector_ferme,g)+
		 		 ModDijkstra(gf,&v[nr-1],&start_ferma,INT_MAX,vector_ferme,g);

	fprintf(*g,"%d\n",start_ferma);
	fprintf(*g,"%d\n",sum);
	for(i= 0 ; i < gf->dim;i++)
		free(gf->mat[i]);
	free(gf->mat);
	free(gf);
	free(vector_ferme);
	free(buff);
}
//distanta minima pana la urmatoare ferma
int min_dist_next_ferma(int *dist,int n,char *buff)
{
	int min = INT_MAX, position;
	int i;
	for (i = 0; i < n; i++)
	{
		if (is_ferma(buff,i) == 1 && dist[i] <= min)
		{
			min = dist[i];
			position = i;
		}
	}
	return position;
}

//gaseste ferma care se gaseste pe drumul cel mai scurt din scr si next
int Best_ferm(graf *g,char *buff,int src,int next)
{
	//Este algoritmul lui Floyd_Warshall modificat pentru a gasi cea mai
	//avantajoasa ferma
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

	//gaseste cea mai avantajoasa ferma
	for(k = 0; k < n; k++)
	{
			sum = 0;
			if(is_ferma(buff,k) == 1 && dist[src][k] != 0
															 && dist[k][next] != 0)
			{
				sum = sum + dist[src][k] + dist[k][next];
				if(sum < min)
				{
					min = sum;
					poz = k;
				}
			}
	}
	for (i = 0; i < n; i++)
		free(dist[i]);
	free(dist);
	return poz;
}

//verifica daca nodul primit ca parametru este o ferma
int is_ferma(char *buff,int possible_ferm)
{
		int i,nr = 0;
		if(possible_ferm > (strlen(buff)-1)/2 ) return 0;

		for(i = 0;i < strlen(buff); i++)
		{
			if(buff[i] == ' ') continue;
			else
			{
				if(buff[i] == '0' ) nr++;
				else
				{
					if(nr == possible_ferm)
					 	return 1;
					nr++;
				}
			}
		}
		return 0;
}

//algoritmul lui Dijkstra modificat
int ModDijkstra (graf *graph, int *src,int *end,int next,char *vector_ferme,FILE **g)
{
	int n = graph->dim;
	int *visited = (int *) malloc (n * sizeof(int));
	int *dist = (int *) malloc (n * sizeof(int));

	//vectorul unde retin nodurile anterioare
	int *prev = (int *) malloc (n * sizeof(int));
	int i;
	for (i = 0; i < n; i++)
	{
		visited[i] = 0;
		dist[i] = INT_MAX;
	}
	dist[*src] = 0;
	prev[*src] = -1;
	for (i = 0; i < n-1; i++)
	{
		int u = min_dist(dist, visited, n);
		visited[u] = 1;
		int v;
		for (v = 0; v < n; v++)
		{
			if ((visited[v] == 0) && (dist[u] != INT_MAX)
			&& (dist[u] + graph->mat[u][v] < dist[v])
			&& graph-> mat[u][v])
			{
				dist[v] = dist[u] + graph->mat[u][v];
				prev[v] = u;
			}
		}
	}
	int result;

	//daca locul de un pleaca este ferma afisez direct distanta
	if(is_ferma(vector_ferme,*src) == 1)
	{
		print_path2(prev,*end,*end,g);
		result = dist[*end];
	}
	else
	{
		//cazul cand src este defapt ultima comanda
		if(next == INT_MAX)
			{
				*end = min_dist_next_ferma(dist,n,vector_ferme);
				print_path2(prev,*end,*end,g);
				result = dist[*end];
			}

		//cazul cand src nu este ultima comanda
			else
			{
				*end =
							Best_ferm(graph,vector_ferme,*src,next);
				print_path2(prev,*end,*end,g);
			  result = dist[*end];
			}
	}
	free(visited);
	free(dist);
	free(prev);
	return result;
}
