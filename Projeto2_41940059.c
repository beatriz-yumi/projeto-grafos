/*
Nome: Beatriz Yumi Kataoka Kamimura        TIA:41940059
*/

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#define MAX_FILA  100

#define NONE -1
#define BRANCO 0
#define CINZA 1
#define PRETO 2

typedef struct aresta{
    int nome;
    struct aresta *prox;
}Aresta;

typedef struct vertice{
    int nome;
    int cor;
    int predecessor;
    int dist;
    Aresta *prim;
}Vertice;

void imprimeGrafoDistancia(Vertice G[], int ordem);
void imprimeGrafo(Vertice G[], int ordem);
void criarGrafo(Vertice **G, int ordem);
int adicionarAresta(Vertice G[], int ordem, int v1, int v2);
int calculaTamanho(Vertice G[], int ordem);
void buscaLargura(Vertice G[], int vInicial, int ordem);
void buscaProfundidade(Vertice G[], int ordem);
void bProfVisitado(Vertice G[], int v);

typedef struct queue{
    int vector[MAX_FILA];
    int front;
    int back;
}Queue;

void initializeEmpty(Queue *f);
int isEmpty(Queue f);
void enqueue(Queue *f, int elem);
int dequeue(Queue *f);


//Inicia a FILA vazia
void initializeEmpty(Queue *Q){
    Q->front = Q->back= 0;
}

//Verifica se a FILA está vazia. (ponto onde a cauda e a cabeça se encontram)
int isEmpty(Queue Q){
    return Q.back == Q.front;
}

//adiciona um elemento a FILA
void enqueue(Queue *Q, int elem){ 
     Q->vector[Q->back] = elem;
     Q->back += 1;
     if(Q->back == MAX_FILA) {
        Q->back = 0;
     }
}

//retira elemento que está na cabeça da FILA
int dequeue(Queue *Q){
	int back;
	if(Q->front == Q->back) {
       exit(1);
    }
    back = Q->vector[Q->front];
    Q->front += 1;
    if(Q->front == MAX_FILA) {
       Q->front = 0;
    }
    return back;              
 }

//fim da implementação da FILA

void criarGrafo(Vertice **G, int ordem){
    int i;
    *G= (Vertice*) malloc(sizeof(Vertice)*ordem);
    
    for(i=0; i<ordem; i++){
        (*G)[i].nome= i;
        (*G)[i].cor= BRANCO;
        (*G)[i].predecessor= -1;
        (*G)[i].dist= 999;
        (*G)[i].prim= NULL;
    }
}
//Adicionando arestas no Grafo
int adicionarAresta(Vertice G[], int ordem, int v1, int v2){
	if (v1<0 || v1 >= ordem)
	   return 0;
	if (v2<0 || v2 >= ordem)
	   return 0;
	
	Aresta *A1= (Aresta *) malloc(sizeof(Aresta));
	A1->nome= v2;
	A1->prox= G[v1].prim;
	G[v1].prim= A1;

	Aresta *A2= (Aresta *) malloc(sizeof(Aresta));
	A2->nome= v1;
	A2->prox= G[v2].prim;
	G[v2].prim= A2;
}
//Calculando o tamanho do Grafo
int calculaTamanho(Vertice G[], int ordem){
	int i;
	int totalArestas=0;
	//Percorrendo o Grafo criado pelo metodo criarGrafo
	for (i=0; i<ordem; i++){
		int j;
		Aresta *aux= G[i].prim;
		for(j=0; aux != NULL; aux= aux->prox, j++);
		totalArestas += j;	
	}
	return totalArestas/2 + ordem;
}

//verificaConexo percorre todos os vertices verificando se todos foram visitados e estão marcados como "PRETO"
//Ou se algum vertice permaneceu "BRANCO" após a execução do método percorreGrafo
int verificaConexo(Vertice G[], int ordem){
	int i;  
	//Percorrendo todos os vertices 
	printf("\nVerificando se o grafo e conexo...\n");  
	for (i=0; i<ordem; i++){
		Aresta *aux= G[i].prim;
		for( ; aux != NULL; aux= aux->prox){	  	
		//Se o vertice não foi visitado e permaneceu "BRANCO" o Grafo não será conexo
		if(G[i].cor == 0){
			   printf("G[%d] n%co foi visitado e permaneceu BRANCO.\n", aux->nome, 198);	    	
	    	   printf("Grafo n%co conexo\n",198);
	    	   return 0;
			}else{		
		          printf("G[%d] foi visitado e marcado como PRETO.\n", aux->nome, 198);
			}	
		}
	}
   //Se todos os vertices foram visitados e marcados como "PRETO" o Grafo será conexo
	printf("\nGrafo Conexo\n");	
    return 1;
}

//buscaLargura é a implementaçãodo Algoritmo BFS(Busca em Largura) do livro Cormen página 595
void buscaLargura(Vertice G[], int vInicial, int ordem){			
	int i,j,u;	
	//Para armazenar o vertice foi utilizada uma fila "q"					
	Queue *q = (Queue*) malloc(sizeof(Queue));
	q->front = 0;	
    q->back = 0;
    //a variável "u" armazenará o vertice inicial para verificar a conectividade com os demais vertices do Grafo
	u = vInicial; 
	Aresta *aux= G[u].prim;
	printf("\n*** Busca em Largura ***");
    printf("\n___________________________________________________________________________\n");
    if (aux != NULL)
        printf("\nV%d:", aux->nome);
	//A estrutura de repetição a seguir marca os vertices não visitado como "BRANCO"    	
	for (i=0; i<ordem; i++){
		Aresta *aux2= G[i].prim;
		for( ; aux2 != NULL; aux2= aux2->prox){
		    G[i].cor = BRANCO;
		    G[i].dist=0;
        	G[i].predecessor=-1;
	     	}
	}  
	//Nesse momento o primeiro vertice visitado é marcado com "CINZA"
    G[u].cor = CINZA;
    //o vertice será armazenado na fila
    enqueue(q, G[u].nome);
    while(isEmpty(*q)!=1){
	//Enquanto populada, o algoritmo continuará marcando como visitados os vertices que ainda tiverem arestas a percorrer
        u = dequeue(q);
	   	for(j=0; aux != NULL; aux= aux->prox, j++){
    	    if (G[aux->nome].cor == BRANCO) {
    	    	printf("  ->  G[j] %d", G[aux->nome].nome);
    	        G[u].cor = CINZA;
	            G[u].dist=G[u].dist+1;
	            G[u].predecessor=u;
		    	enqueue(q, aux->nome);
	    	}
	    }
	    aux= G[u].prim;
		//Quando não houverem mais aretas a serem visitadas em um determinado vertice, ele será marcado como "PRETO"	        
        G[u].cor = PRETO;
    } 
    imprimeGrafoDistancia(G, ordem);	        
}

//verificaGrau calcula o grau de cada vertice e o classifica como par ou impar 
int verificaGrau(Vertice G[], int ordem){
	int i=0;
	int verticeGrau[ordem];	
	//Percorre o grafo e soma a quantidade de arestas de cada um dos vertices
	for(i=0; i<ordem; i++){
	    verticeGrau[i]=0;		
		Aresta *aux= G[i].prim;
	    for( ; aux != NULL; aux= aux->prox){
			verticeGrau[i]+=1;			
		}
	}	
	//Verifica de o grau de cada um dos vertices é par ou impar 
	printf("\nVerificando se o grafo apresenta grau Impar ou grau diferente de zero...\n"); 
	for(i=0; i<ordem; i++){
		if(verticeGrau[i]%2 != 0){
		   printf("V%d: Grau: %3d IMPAR\n", i, verticeGrau[i]);	
		   return 0;
		}
		if((verticeGrau[i]%2 == 0)&&(verticeGrau[i]!=0)){	
		    printf("V%d: Grau: %3d PAR\n", i, verticeGrau[i]);	
		    }	
    }
	return 1;	   
}
//Verifica o retorno dos metodos verificaConexo e verificaGraua para determinar se o grafo é Euleriano
int verificaEuleriano(Vertice G[], int ordem){
	if((verificaConexo(G, ordem) == 1)&&(verificaGrau(G, ordem) == 1))
		
		printf("\n***** %c um Grafo Euleriano *****\n",144);
	else
		printf("\n***** N%co %c um Grafo Euleriano *****\n",198,130);


}
//buscaLargura é a implementaçãodo Algoritmo DFS(Busca em Profundidade) do livro Cormen página 604
void buscaProfundidade(Vertice G[], int ordem){
    int i;
    printf("\n*** Busca em Profundidade ***");
    printf("\n___________________________________________________________________________\n");
  
    for (i=0; i<ordem; i++){
        G[i].cor = BRANCO;
        G[i].dist=0;
        G[i].predecessor = -1;
    }
    
    for (i=0; i<ordem; i++){
        if (G[i].cor == BRANCO) {
            bProfVisitado(G, i);
        }
    }
}
//Percorre o Grafo em busca de Profundidade marcando como visitado  
void bProfVisitado(Vertice G[], int v){
    int j;
    Aresta *aux = G[v].prim;
    if (aux != NULL)
        printf("\nV%d:", aux->nome);
    for(j=0; aux != NULL; aux= aux->prox, j++){
        G[v].cor = CINZA;
        if (G[aux->nome].cor == BRANCO) {
            printf("  ->  G[j] %d", G[aux->nome].nome);
            G[aux->nome].dist=G[aux->nome].dist+1;
            G[aux->nome].predecessor = v;
            bProfVisitado(G, j);
        }
    }
    G[v].cor = PRETO;
}
//Imprime o Grafo Inicial
void imprimeGrafo(Vertice G[], int ordem){
    int i;
    printf("\n*** Grafo ***");
    printf("\n___________________________________________________________________________\n");
    for (i=0; i<ordem; i++){
        printf("\nV%d: ", i);
        Aresta *aux= G[i].prim;
        for( ; aux != NULL; aux= aux->prox)
            printf("->%3d ", aux->nome);       
    }
    printf("\n\n");
}
//Imprime a Distancia percorrida do grafo
void imprimeGrafoDistancia(Vertice G[], int ordem){
    int i;
    printf("\n\n");
    printf("*** Distancia ***");
    printf("\n___________________________________________________________________________\n");
    printf("\nVert:");
    for(i=0; i<ordem; i++)
        printf("%3d ",G[i].nome);
    printf("\nDist:");
    for(i=0; i<ordem; i++)
        printf("%3d ",G[i].dist);
    printf("\nCor: ");
    for(i=0; i<ordem; i++)
        printf("%3d ",G[i].cor);
    printf("\nPred:");
    for(i=0; i<ordem; i++)
        printf("%3d ",G[i].predecessor);
    printf("\n");
    
}


int main(int argc, char *argv[]) {
    Vertice *G;
    int ordemG= 10;
   
    criarGrafo(&G, ordemG);
    adicionarAresta(G,ordemG,3,4);
    adicionarAresta(G,ordemG,4,2);
    adicionarAresta(G,ordemG,5,4);
    adicionarAresta(G,ordemG,2,3);
    adicionarAresta(G,ordemG,3,7);
    
    printf("\nTamanho: %d\n",calculaTamanho(G, ordemG));
    
    imprimeGrafo(G, ordemG);
    buscaProfundidade(G, ordemG);
    imprimeGrafoDistancia(G, ordemG);
	buscaLargura(G, 2,ordemG);
    verificaEuleriano(G, ordemG);
    printf("\n");
    
    return 0;
}