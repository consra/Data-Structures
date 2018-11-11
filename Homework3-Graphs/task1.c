#include "task1.h"

//task1
void task1(FILE *f,FILE **g)
{
	char *buff = (char*)malloc(sizeof(char)*100);
	int noduri;
	fgets(buff,10,f);
	sscanf(buff,"%d",&noduri);
	fgets(buff,1000,f);

	//determin ferma cu care sa incep
  int poz_ferma = find_ferma(buff) -1;

	//creez matricea de adiacenta
	int muchii,i,poz_x,poz_y,cost;
	fgets(buff,1000,f);
	graf *gf = init(noduri);
	sscanf(buff,"%d",&muchii);
	for(i = 0;i < muchii;i++)
	{
		fgets(buff,1000,f);
		sscanf(buff,"%d %d %d",&poz_x,&poz_y,&cost);
		add_con(gf,poz_x,poz_y,cost);
	}

	int dest,sum=0;
	fgets(buff,1000,f);
	while(fgets(buff,1000,f) != NULL)
	{
		//tipul fermei nu ma intereseaza asa ca citesc doar nodul
		sscanf(buff,"%*d %d",&dest);

		/*in Dijkstra calculez distanta de la comanda curenta
			la ferma si apoi de la ferma la comanda urmatoare */
		sum = sum + Dijkstra(gf,poz_ferma,dest,g)
							+ Dijkstra(gf,dest,poz_ferma,g);
	}

	fprintf(*g,"%d\n",poz_ferma);
	fprintf(*g,"%d\n",sum);

	for(i= 0 ; i < gf->dim;i++)
		free(gf->mat[i]);
	free(gf->mat);
	free(gf);
	free(buff);
}

//imi gaseste ferma de inceput
int find_ferma(char *v)
{
	int i;
	int nr_poz = 0;
	for(i = 0; i < strlen(v);i++)
	{
		if(v[i] == ' ') continue;
		else
		{
			if(v[i] == '0')	nr_poz++;
			else
			{
				nr_poz++;
				break;
			}
		}
	}
	return nr_poz;
}
//imi adauga in matricea de adiacenta un cost pentru 2 noduri
void add_con (graf *g, int i, int j, int cost)
{
	g->mat[i][j] = cost;
}

//imi initializeza graful cu o matrice cu toate elementele 0
graf *init (int n)
{
	graf * graph = (graf *)malloc(sizeof(graf));
	graph->dim= n;
	graph->mat = (int **)malloc(n * sizeof(int *));
	int i;
	for (i = 0; i < n; i++)
		graph->mat[i] = (int *)calloc(n,sizeof(int));
	return graph;
}

//imi printeaza path ul in fisierul de iesire
void print_path2(int *parent, int v,const int end,FILE **g)
{
	/*pentru ca nodurile ce imi trebuie sunt in ordine inversa
	 	ma folosesc de o stiva sa le inversez ordinea */
	Queue *node = NULL;
	int iter = parent[end];

	//le introduc in stiva
	while (-1 != iter)
	{
		node = push(node,iter);
		iter = parent[iter];
	}

	// le afisez
	while(node != NULL)
	{
		fprintf(*g,"%d ", node->data);
		node = pop(node);
	}
}

//determina pozitia nodului cu valoarea din dist cea mai mica
int min_dist (int *dist, int *visited, int n)
{
	int min = INT_MAX, pos;
	int i;
	for (i = 0; i < n; i++)
	{
		if (visited[i] == 0 && dist[i] <= min)
		{
			min = dist[i];
			pos = i;
		}
	}
	return pos;
}

//algoritmul implementat la laborator
int Dijkstra (graf *graph, int src,int end,FILE **g)
{
	int n = graph->dim;
	int *visited = (int *) malloc (n * sizeof(int));
	int *dist = (int *) malloc (n * sizeof(int));
	int *prev = (int *) malloc (n * sizeof(int));
	int i;
	for (i = 0; i < n; i++)
	{
		visited[i] = 0;
		dist[i] = INT_MAX;
	}
	dist[src] = 0;
	prev[src] = -1;
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

				//ma folosesc de ideea pseudocodului de la aborator
				prev[v] = u;
			}
		}
	}

	//afisez path de la sursa(src) la destinatie(end)
	print_path2(prev,end,end,g);
	int result = dist[end];
	free(visited);
	free(dist);
	free(prev);
	return result;
}

//operatia pentru inserea in stiva
Queue *push (Queue *top, int data)
{
	Queue *new;
	new = (Queue*)malloc (sizeof(Queue));
	new->data = data;
	if (top == NULL)
	{
		new->next = NULL;
		top = new;
		return top;
	}
	else
	{
		new->next = top;
		top = new;
	}
	return top;
}

//scoate din stiva
Queue *pop(Queue *q)
{
	Queue *new;
	new = q;
	if (q == NULL)
		return NULL;
	q = new->next;
	free(new);
	return q;
}
