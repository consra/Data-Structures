#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//structura folosita pentru exercitiul 1 si 2
//retin aici copiii nodului si matricea corespunzatoare
typedef struct Tree
{
	int nivel;
	char eticheta;
	char **tabla;
	int nr_cop;
	struct Tree** v;
}Tree;

//cele 2 structuri sunt folosite pentru taskul3
//in v retin din nou copiii nodului
typedef struct Minimax_Tree
{
	int nivel;
	int val;
	int nr_cop;
	struct Minimax_Tree** v;
}Minimax_Tree;

typedef struct Qnode{
	Minimax_Tree *data;
	struct Qnode *next;
}Qnode;

Tree *init_tree(Tree *,char **,char ,int);
void print(Tree *,int,FILE **);
void print_et(Tree *,int,FILE **);
int poz_libere(char **);
Tree* insert (Tree *, char **,char,int ,int,const char);
void afisare(Tree *node,int,FILE **);
void afisare_et(Tree *node,int,FILE **);
int test_cond(char **);
int test_cond_2(char **,char);
void dealoc(Tree**);
int move(char **v);
void add_et(Tree *node,int);
Qnode *enqueue(Qnode *q, Minimax_Tree* data);
void minimax(Minimax_Tree *node);
Qnode *dequeue(Qnode *q);
Minimax_Tree *init_tre(int nivel,int nr_cop,int val );
Qnode *get_cop(char *,int ); 
void afisare_val(Minimax_Tree **node,int ref,FILE **out);
void print_val(Minimax_Tree *nod,int ref,FILE **out);
Qnode *enqueue_last(Qnode *q, Minimax_Tree* data);
void task1_2(FILE *f,FILE *g,char *);
void task3_4(FILE *f,FILE *g,char *);


int main(int argc,char **argv)
{
	FILE *f,*g;
	f = fopen(argv[2],"r");
	g = fopen(argv[3], "w");
	if(f == NULL) printf("aici");
	//in functie de argv[1] apelez taskul necesar 
	if(strcmp(argv[1],"-c1") == 0)
		task1_2(f,g,argv[1]);
	else if(strcmp(argv[1],"-c2") == 0)
		task1_2(f,g,argv[1]);
	else if(strcmp(argv[1],"-c3") == 0)
		task3_4(f,g,argv[1]);
	else if(strcmp(argv[1],"-c4") == 0)
		task3_4(f,g,argv[1]);
	fclose(g);
	fclose(f);
	return 0;
}

void task1_2(FILE *f,FILE *g,char *task)
{
	char buff[20];
	int i = 0;
	char **tabla = (char**)malloc(sizeof(char*)*20);
	fgets(buff,100,f);
	char alg = buff[0];
	const char alg2 = alg;
	//copiez intr-o matrice matricea primita ca input 
	while(fgets(buff,100,f) != NULL)
	{
		if(strlen(buff) > 2)
		{
			tabla[i] = (char*)malloc(sizeof(char)*20);
			sprintf(tabla[i++],"%s",buff);
		}
	}
	//pozitia de referinta pentru nivele 
	// ma folosesc de casutele libere ca sa determin nivele arborului
	int ref = 9 - poz_libere(tabla);
	Tree *node = NULL;
	node = insert(node,tabla,alg,-1,ref,alg2);
	if(strcmp("-c1",task) == 0)
		afisare(node,ref,&g);
	else if(strcmp("-c2",task) == 0)
		afisare_et(node,ref,&g);
	//eliberez structura arborului si matricea primita ca input
	for(i = 0; i < 3 ;i++)
        {
		free(tabla[i]); 
	}
	dealoc(&node);
	free(tabla);
}
void task3_4(FILE *f,FILE *g,char *task)
{
	//retin fiecare linie in buff
	char *buff = (char*)calloc(500000,sizeof(char));
	//citesc manual primul nod ca sa-l pun in coada
	fgets(buff,1000,f);	
        int niv_max,cop;
	sscanf(buff,"%d",&niv_max);
	fgets(buff,1000,f);
	sscanf(buff,"(%d)",&cop);
	Minimax_Tree *node = NULL;
	//initializez cozile si pun primul element in coada anterior
	Qnode *anterior = NULL;
        Qnode *prezent = NULL;
	Qnode *aux ;
	node = init_tre(0,cop,0);
	anterior = enqueue(anterior,node);
	Qnode *dealo = NULL;
	int niv = 0;
	int nr;
	//citesc linie cu linie
	while(fgets(buff,1000000,f) != NULL )
	{
		niv++;
		// transform linia intr -o coada cu arbori
		prezent = get_cop(buff,niv);
		aux = prezent;
		while(anterior != NULL)
		{
			nr = 0;
			while(nr < anterior->data->nr_cop && prezent != NULL)
			{
				//bag toate nodurile din noua coada(prezent) in vectorii fiilor cozii anterioare 
				anterior->data->v[nr++] = prezent->data;
				prezent = prezent->next; 
			}
			dealo = anterior;
			anterior = anterior->next;
			//dealoc nodul dupa ce copii au fost adaugati
			free(dealo);
			dealo = NULL;
		}
		anterior = aux;	

	}
	//dealoc ultima coada
	while(aux != NULL)
	{
		dealo = aux;
		aux=aux->next;
		free(dealo);
	}
	free(buff);
		minimax(node);
		afisare_val(&node,0,&g);

}
//functia ajutatoare pentru taskul 2 ce imi adauga eticheta corespunzatoare
void add_et(Tree *node,int ref)
{
	if(node == NULL) return;
	if(ref == 0) 
		ref = 2;
	int i,nr;
	// ma folosesc de paritatea nivelului pentru a determina
	//tipul de nod
	if(ref % 2 == node->nivel % 2)
		nr = 1;
	else 
		nr = 0;
	//nod de tipul sau
	if(nr == 1)
	{
		node->eticheta = 'F';
		for(i = 0 ; i < node->nr_cop; i++)
		{
			if(node->v[i] != NULL && node->v[i]->eticheta == 'T')
			{
				node->eticheta = 'T';
				break;
			}
		}
		
	}
	//node de tipul si 
	else
	{
		node->eticheta = 'T';	
		for(i = 0 ; i < node->nr_cop; i++)
		{
			if( node->v[i] != NULL && node->v[i]->eticheta == 'F')
			{
				node->eticheta = 'F';
				break;
			}
		}
	}
}
//conditiile de determinare a etichetei 
int test_cond_2(char **tabla,const char alg)
{

	if(tabla[0][0] == tabla[1][0] && tabla[1][0] == alg &&
	   tabla[1][0] == tabla[2][0] && tabla[2][0] != '-')
	   return 1;
        else if (tabla[0][0] == tabla[0][2] && tabla[0][0] == alg &&
	   tabla[0][2] == tabla[0][4] && tabla[0][4] != '-')
	   return 1;
	else if (tabla[1][0] == tabla[1][2] && tabla[1][2] == alg &&
	   tabla[1][2] == tabla[1][4] && tabla[1][2] != '-')
	   return 1;
	 else if (tabla[0][0] == tabla[1][2] && tabla[1][2] == alg &&
	   tabla[1][2] == tabla[2][4]  &&  tabla[2][4] != '-')
	   return 1;
	 else if (tabla[0][2] == tabla[1][2] && tabla[1][2] == alg &&
	   tabla[1][2] == tabla[2][2]  &&  tabla[1][2] != '-')
	   return 1; 
	 else if (tabla[0][4] == tabla[1][2] && tabla[1][2] == alg &&
	   tabla[1][2] == tabla[2][0] &&  tabla[1][2] != '-')
	   return 1;
	 else if (tabla[0][4] == tabla[1][4] && tabla[1][4] == alg && 
	   tabla[1][4] == tabla[2][4] &&  tabla[0][4] != '-')
	   return 1;
	 else if (tabla[2][0] == tabla[2][2] && tabla[2][2] == alg &&
	   tabla[2][4] == tabla[2][2] &&  tabla[2][2] != '-')
	   return 1;

	return 0;
}
//conditiile de oprire a construirii matricei
int test_cond(char **tabla)
{
	if(tabla[0][0] == tabla[1][0] && 
	   tabla[1][0] == tabla[2][0] && tabla[2][0] != '-')
	   return 1;
        else if (tabla[0][0] == tabla[0][2] &&
	   tabla[0][2] == tabla[0][4] && tabla[0][4] != '-')
	   return 1;
	else if (tabla[1][0] == tabla[1][2] &&
	   tabla[1][2] == tabla[1][4] && tabla[1][2] != '-')
	   return 1;
	 else if (tabla[0][0] == tabla[1][2] &&
	   tabla[1][2] == tabla[2][4]  &&  tabla[2][4] != '-')
	   return 1;
	 else if (tabla[0][2] == tabla[1][2] &&
	   tabla[1][2] == tabla[2][2]  &&  tabla[1][2] != '-')
	   return 1; 
	 else if (tabla[0][4] == tabla[1][2] &&
	   tabla[1][2] == tabla[2][0] &&  tabla[1][2] != '-')
	   return 1;
	 else if (tabla[0][4] == tabla[1][4] &&
	   tabla[1][4] == tabla[2][4] &&  tabla[0][4] != '-')
	   return 1;
	 else if (tabla[2][0] == tabla[2][2] &&
	   tabla[2][4] == tabla[2][2] &&  tabla[2][2] != '-')
	   return 1;

	return 0;
}
//dealocare structurii arborescente 
void dealoc(Tree** node)
{
	if(*node == NULL)
		return;	
	int i;
	Tree *aux;
	for(i = 0; i < (*node)->nr_cop;i++)
	{
		aux = (*node)->v[i];
		dealoc(&aux);
	}

	free((*node)->tabla[0]); (*node)->tabla[0] = NULL;
	free((*node)->tabla[1]); (*node)->tabla[1] = NULL;
	free((*node)->tabla[2]); (*node)->tabla[2] = NULL;
	free((*node)->tabla);
	free((*node)->v);
	free((*node));
}
//functia ce creeaza arborele si determina matricele pentru fiecare nod 
Tree* insert (Tree *new, char **data,char alg,int nr,int ref,const char co)
{
	int i;
	//daca mai o singura pozitie libera 
	if(poz_libere(data) == 1)
	{
		Tree * node = NULL;
		node = init_tree(node,data,alg,nr);
		if(test_cond_2(node->tabla,co) == 1)
			node->eticheta = 'T';
		else 
			node->eticheta = 'F';
		return node;	
	}
	else if(poz_libere(data) > 1)
	{
		new = init_tree(new,data,alg,nr);
		//daca nu este apelul initial 
		if(nr != -1)
		{
			if(alg == 'O')
				alg = 'X';
			else
				alg = 'O';
		}
		//conditiile de castigare sa nu fie deja indeplinite 
		if(test_cond(new->tabla) != 1)
		{	
			for(i = 0; i < poz_libere(new->tabla);i++)
			{
				new->v[i] = insert(new->v[i],new->tabla,alg,i,ref,co);	
			}	
			add_et(new,ref);		
		}
		else 
		{
			if(test_cond_2(new->tabla,co) == 1)
				new->eticheta = 'T';
			else 
				new->eticheta = 'F';
		}
		
        }	
	return new;
}
//functia ce-mi creaza un nou node de tipul Tree * si initializeaza matricea lui
// in functie de alg(caracterul primit) si matricea primita ca parametru
Tree *init_tree(Tree *nod,char **matrice,char alg,int nr)
{
	if(matrice == NULL) return NULL;
	nod  = (Tree*)calloc(1,sizeof(Tree));
	nod->v = (Tree**)calloc(8,sizeof(Tree*));
	nod->nr_cop = poz_libere(matrice);
	if(nr == -1)
		nod->nivel = 9 - nod->nr_cop;
	else 
		nod->nivel = 9 - nod->nr_cop+1;
	nod->tabla = (char**)malloc(sizeof(char*)*10);
	int i,j,contor = 0;
	for(i = 0; i < 8; i++)
	{
		nod->v[i] = NULL;
	}
	//alocare unei noi matrici
	for(i = 0 ;i < 3 ; i++)
	{
		nod->tabla[i] = (char*)calloc(20,sizeof(char));
		sprintf(nod->tabla[i],"%s",matrice[i]);
	}
	if(nr == -1)
	 return nod;
	for (i = 0; i < 3 ;i++)
	{
		for(j = 0; j < 5 ;j ++)
		{
			if(nod->tabla[i][j] == '-')
			{
				if(contor == nr)
				{
					nod->tabla[i][j] = alg;
					return nod;
				}
				contor++;
			}
		}
	}

	return nod;
}
//afiseaza pentru taskul 1 unul matricea fiecarui nod 
void afisare(Tree *node,int ref,FILE **out)
{
	if(node == NULL)
		return;	
	int i;
        print(node,ref,out);
	for(i = 0; i < node->nr_cop;i++)
	{
		afisare(node->v[i],ref,out);
	}

}
//afiseaza pentru taskul 2 matricea fiecarui nod 
void afisare_et(Tree *node,int ref,FILE **out)
{
	if(node == NULL)
		return;	
	int i;
        print_et(node,ref,out);
	for(i = 0; i < node->nr_cop;i++)
	{
		afisare_et(node->v[i],ref,out);
	}
}
//functie ajutatoare pentru afisare 
void print(Tree *nod,int ref,FILE **out)
{
	int i,j;
	if(nod == NULL) return;
	
	for(i = 0 ; i < 3;i++)
	{
		for(j = ref; j < nod->nivel; j++)
			fprintf(*out,"\t");
		fprintf(*out,"%s",nod->tabla[i]);	
	}
	fprintf(*out,"\n");
}
void print_et(Tree *nod,int ref,FILE **out)
{
	int j;
	if(nod == NULL) return;
	for(j = ref; j < nod->nivel; j++)
		fprintf(*out,"\t");
	fprintf(*out,"%c",nod->eticheta);	
	fprintf(*out,"\n");
}

int poz_libere(char **tabla)
{
	int nr = 0,i,j;
	if(tabla == NULL) return -1;
	for(i = 0 ; i < 3 ;i++)
	{
		for(j = 0;j< 5; j++)
		{
			if(tabla[i][j] == '-')
				nr++; 
		}	
	}
	return nr;
}
//functii pentru taskul 3
//adauga la capatul listei cu pointer la sfarsitul ei 
Qnode *enqueue_last(Qnode *q, Minimax_Tree* data)
{
	Qnode *new,*aux;
	new = (Qnode*)malloc(sizeof(Qnode));
	new->data = data;
	new->next = NULL;
	if (q == NULL)
		return new;
	aux = q;
	aux->next = new;
	return new;
}
Qnode *enqueue(Qnode *q, Minimax_Tree* data)
{
	Qnode *new,*aux;
	new = (Qnode*)malloc(sizeof(Qnode));
	new->data = data;
	new->next = NULL;
	if (q == NULL)
		return new;
	aux = q;
	while(aux->next)
		aux = aux->next;
	aux->next = new;
	return q;
}
Qnode *dequeue(Qnode *q)
{
	Qnode *new;
	new = q;
	if (q == NULL)
		return NULL;
	q = new->next;
	free(new);
	return q;
}
//transforma linia primita intr-o coada cu noduri de tip Minimax_tree 
Qnode *get_cop(char *buff,int niv)
{
 	// primeste o linie intreaga si o transforma intr-o coada pe care o trimite in main
	int cop;
	Minimax_Tree *node = NULL;
	Qnode *vec = (Qnode*)malloc(sizeof(Qnode));
	Qnode *aux;
	Qnode *last = vec;
	char *p;
	while( (sscanf(buff,"(%d) %*s",&cop) == 1 ||
	      	sscanf(buff,"[%d] %*s",&cop) == 1 ))
	{
		//ma deplasez cu p prin buffer 
		p = strchr(buff,' ');
		if(p == NULL)
			p = strchr(buff,'\n');
		if(buff[0] == '(')
		{
			node = init_tre(niv,cop,0);	
		}
		else if (buff[0] == '[')
		{
			node = init_tre(niv,0,cop);
		}
		//pune mereu la sfarsitul cozii
		last = enqueue_last(last,node);
		node = NULL;
		if(strlen(buff) < 5 )
			break;
		buff = p+1;
	}
	aux = vec;
	vec = vec->next;
	free(aux);
	return vec;

}

//functia ce calculeaz
void minimax(Minimax_Tree *node)
{
	if(node == NULL || node->v[0] == NULL) return;
	int min = 1000;
	int max = -1000;
	int i,nr;
	//In functie de paritatea nivelului functia imi determina tipul nodului
	if(node->nivel % 2 == 0)
		nr = 1;
	else
		nr = 2;
	//cazul in care nodul este max
	if(nr == 1)
	{
		for(i = 0; i < node->nr_cop;i++)
		{
			minimax(node->v[i]);
		}
		for(i = 0; i < node->nr_cop;i++)
		{
			if(node->v[i]  != NULL && node->v[i]->val > max)
				max = node->v[i]->val;
		}
		node->val = max;
	}
	//nodul este min
	else
        {
                for(i = 0; i < node->nr_cop;i++)
                {
                        minimax(node->v[i]);
                }
                for(i = 0; i < node->nr_cop;i++)
                {
                        if(node->v[i] != NULL && node->v[i]->val < min)
                                min = node->v[i]->val;
                }
                node->val = min;
        }
	
}
//aici initializez un node de arbore in functie de nr de copii
Minimax_Tree *init_tre(int nivel,int nr_cop,int val)
{
	Minimax_Tree *nod = (Minimax_Tree*)malloc(sizeof(Minimax_Tree));
	nod->nivel = nivel;
	nod->nr_cop = nr_cop;
	nod->val = val;
	//aloc memorie pentru vectorul de copii de arbore
	if(nr_cop != 0)
		nod->v = (Minimax_Tree**)malloc(sizeof(Minimax_Tree*)*nr_cop);
	else 
		nod->v = (Minimax_Tree**)malloc(sizeof(Minimax_Tree*));
	int i;
	for(i = 0; i < nr_cop; i++)
	{
		nod->v[i] = NULL;
	}
	if(nr_cop == 0)
		nod->v[0] = NULL;	
	return nod;
}
//afiseaza recursiz arborele
void afisare_val(Minimax_Tree **node,int ref,FILE **out)
{
	if(node == NULL)
		return;	
	int i;
        print_val(*node,ref,out);
	Minimax_Tree *aux = *node;
	for(i = 0; i < aux->nr_cop;i++)
	{
		afisare_val(&(aux->v[i]),ref,out);
	}
	//dealoc imediat dupa afisare pentru mai multa rapiditate
	free((*node)->v);
	free((*node));
}
//printeza valoare nodului
void print_val(Minimax_Tree *nod,int ref,FILE **out)
{
	int j;
	if(nod == NULL) return;
	for(j = ref; j < nod->nivel; j++)
		fprintf(*out,"\t");
	fprintf(*out,"%d",nod->val);	
	fprintf(*out,"\n");
}
