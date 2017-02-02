#include <stdio.h>
#include <stdlib.h>

typedef enum {false, true} bool; //definindo tipo bool

typedef struct Node{
    int n_chaves;            //Número de chaves no nó
    int *chaves;             //Array de chaves
    struct Node **ptr_node;  //Array de ponteiros para nós filho
    bool folha;              //Diferencia nós folha de nós interiores
} Node;

Node * criaNode(Node *no, int ordem){

    int i;                  //Contador de for

    no = malloc(sizeof(Node));

    no->folha = true;
    no->n_chaves = 0;       //Inicializa sem chaves
    no->chaves = (int*) malloc((ordem-1) * sizeof(int));  //Aloca dinamicamente as chaves e
    for(i = 0; i < ordem - 1; i++){                       //inicializa elas com -1 (sem valor)
        no->chaves[i] = -1;
    }

    no->ptr_node = (Node **) malloc(ordem * sizeof(struct Node *));  //Aloca dinamicamente os ponteiros e
    for(i = 0; i < ordem; i++){                                      //inicializa eles com NULL
        no->ptr_node[i] = NULL;
    }

    return no;
}

void insertionSort(int *array, int tamanho){
    int i, j, item;

    for(i = 1; i < tamanho; i++){
        item = array[i];
        j = i - 1;

        while (j >= 0 && array[j] > item){
            array[j + 1] = array[j];
            j--;
        }

        array[j + 1] = item;
    }
}

void copiaNode(Node *sourc, Node *dest, int ordem, int m){
    int i, j = 0;

    for(i = m; i < ordem - 1; i++){
        dest->chaves[j] = sourc->chaves[i];
        dest->ptr_node[j + 1] = sourc->ptr_node[i + 1];
        sourc->chaves[i] = -1;           //Verificar se precisa
        sourc->ptr_node[i + 1] = NULL;   //Verificar se dá merda
        dest->n_chaves++;
        sourc->n_chaves--;
        j++;
    }
}

int ordenaNode(Node *no, int chave){
    int i = no->n_chaves;

    while(i > 0 && no->chaves[i - 1] > chave){
        no->chaves[i] = no->chaves[i - 1];
        no->ptr_node[i + 1] = no->ptr_node[i];
        i--;
    }

    no->chaves[i] = chave;
    no->n_chaves++;

    return i;
}

int posDescida(int *array, int pos, int chave){

    while(pos > 0 && array[pos - 1] > chave){
        pos--;
    }

    return pos;
}

void divide(Node *no, int chave, Node *direita, Node *novo, int *chave_promovida, int ordem){

    if(no->folha == false){ //Nó a ser dividido é folha
        novo->folha = false;
    }

    int m, i, j, pos;

    m = (int)(ordem - 1)/2;

    j = 0;       //i para for, j para vetor do novo nó

    if(chave < no->chaves[m]){
        copiaNode(no, novo, ordem, m);
        no->chaves[no->n_chaves] = chave;
        no->n_chaves++;
        insertionSort(no->chaves, no->n_chaves);
        pos = posDescida(no->chaves, no->n_chaves, chave);
        novo->ptr_node[0] = no->ptr_node[pos];
        no->ptr_node[pos] = direita;

    } else {
        copiaNode(no, novo, ordem, m + 1);
        novo->chaves[novo->n_chaves] = chave;
        novo->n_chaves++;

        insertionSort(novo->chaves, novo->n_chaves);
        pos = posDescida(novo->chaves, novo->n_chaves, chave);
        novo->ptr_node[0] = no->ptr_node[no->n_chaves];
        novo->ptr_node[pos] = direita;
    }

    *chave_promovida = no->chaves[(no->n_chaves) - 1];

    no->chaves[(no->n_chaves) - 1] = -1;
    no->n_chaves--;
    printf("promo=%d\n", *chave_promovida);
}

bool buscaInsere(Node *atual, int chave, bool *promocao, int *chave_promovida, Node **novo, int ordem){

    int i;      //Contador para for
    bool jaExiste = false;      //Para saber se chave já existe no nó
    for(i = 0; i < atual->n_chaves; i++){//Percorre nó para ver se chave já existe
        if(chave == atual->chaves[i]){
            jaExiste = true;
        }
    }

    if(jaExiste){ //Chave já existe

        return false;

    } else {      //Chave não existe

        if(atual->folha){   //Nó atual é folha

            if(atual->n_chaves < ordem - 1){    //Nó não cheio

                atual->chaves[(atual->n_chaves)] = chave;
                atual->n_chaves++;
                atual->ptr_node[(atual->n_chaves) + 1] = NULL;
                *promocao = false;
                insertionSort(atual->chaves, atual->n_chaves);

            } else {                            //Nó cheio

                divide(atual, chave, NULL, *novo, chave_promovida, ordem);
                *promocao = true;
            }

            return true;

        } else {            //Nó atual é interno

            int pos = posDescida(atual->chaves, atual->n_chaves, chave);

            bool status = buscaInsere(atual->ptr_node[pos], chave, promocao, chave_promovida, novo, ordem);

            if(status == true && *promocao == true){
                if(atual->n_chaves < ordem - 1){
                    printf("oi\n");
                    pos = ordenaNode(atual, *chave_promovida);
                    atual->ptr_node[pos + 1] = *novo;
                    *promocao = false;
                } else {
                    printf("me\n");
                    Node *filho = criaNode(filho, ordem);
                    divide(atual, *chave_promovida, *novo, filho, chave_promovida, ordem);
                    *novo = filho;
                }
            }

            return status;
        }
    }


}


bool insere(Node **raiz, int chave, int ordem){

    if(*raiz == NULL){ //Árvore vazia.

        *raiz = criaNode(*raiz, ordem);
        (*raiz)->chaves[0] = chave;
        (*raiz)->n_chaves++;

        return true;

    } else {                //Árvore não vazia

        bool status, promocao;
        int chave_promovida;
        Node *novo = criaNode(novo, ordem);

        status = buscaInsere(*raiz, chave, &promocao, &chave_promovida, &novo, ordem);

        if(promocao == true){
            Node *nova_raiz = criaNode(nova_raiz, ordem);
            nova_raiz->folha = false;

            nova_raiz->chaves[0] = chave_promovida;
            nova_raiz->n_chaves++;
            nova_raiz->ptr_node[0] = *raiz;
            nova_raiz->ptr_node[1] = novo;

            *raiz = nova_raiz;
        }

        return status;
    }
}

void printArvore(Node *no){
    int i;
    bool mesmo_nivel = true;

    printf("[");

    for(i = 0; i < no->n_chaves - 1; i++){
        printf("%d ", no->chaves[i]);
    }
    printf("%d]", no->chaves[(no->n_chaves) - 1]);

    if(no->folha == false && mesmo_nivel == true){
        printf("\n");
        for(i = 0; i <= no->n_chaves; i++){
            printArvore(no->ptr_node[i]);
        }
    }
}

void destroi(Node *no){
	if (no->folha == true){
		free (no);
		no = NULL;
	} else {
		int i; 									//contador para for
		for(i = 0; i <= no->n_chaves; i++)
			destroi(no->ptr_node[i]);
		free(no);
		no = NULL;
	}
}

int main(){

    int ordem;
    scanf("%d", &ordem);

    Node* raiz = NULL;

    insere(&raiz, 4, ordem);

    insere(&raiz, 7, ordem);

    insere(&raiz, 20, ordem);

    insere(&raiz, 15, ordem);

    insere(&raiz, 6, ordem);

    insere(&raiz, 4, ordem);

    insere(&raiz, 18, ordem);

    insere(&raiz, 5, ordem);

    insere(&raiz, 70, ordem);

    insere(&raiz, 50, ordem);

    insere(&raiz, 9, ordem);

    insere(&raiz, 35, ordem);

    insere(&raiz, 25, ordem);


   // printArvore(raiz);
    printf("\n");

    destroi(raiz);

    return 0;
}
