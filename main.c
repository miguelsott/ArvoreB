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

/** REVISAR E VER SE DIVISÃO PRECISA DE MÉTODO DE ORDENAÇÃO PRA FUNCIONAR **/
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

int posChave(int *array, int pos, int chave){

    while(pos > 0 && chave < array[pos - 1]){
        pos--;
    }

    return pos;
}

void divide(Node *no, int chave, Node *direita, Node *novo, int *chave_promovida, int ordem){
    /** REVISAR ESTE CÓDIGO DE DIVISÃO E MELHORÁ-LO **/
    if(no->folha == false){ //Nó a ser dividido é folha
        novo->folha = false;
    }

    int m, i, j, pos;

    m = (int)(ordem - 1)/2;

    j = 0;       //i para for, j para vetor do novo nó

    if(chave < no->chaves[m]){

        for(i = m; i < ordem - 1; i++){
            novo->chaves[j] = no->chaves[i];
            novo->ptr_node[j + 1] = no->ptr_node[i + 1];
            no->chaves[i] = -1;           //Verificar se precisa
            no->ptr_node[i + 1] = NULL;   //Verificar se dá merda
            novo->n_chaves++;
            no->n_chaves--;
            j++;
        }
        no->chaves[m] = chave;
        no->n_chaves++;

        insertionSort(no->chaves, no->n_chaves);
        pos = posChave(no->chaves, no->n_chaves, chave);

    } else {

        for(i = m + 1; i < ordem - 1; i++){
            novo->chaves[j] = no->chaves[i];
            novo->ptr_node[j + 1] = no->ptr_node[i + 1];
            no->chaves[i] = -1;           //Verificar se precisa
            no->ptr_node[i + 1] = NULL;   //Verificar se dá merda
            novo->n_chaves++;
            no->n_chaves--;
            j++;
        }
        novo->chaves[novo->n_chaves] = chave;
        novo->n_chaves++;

        insertionSort(novo->chaves, novo->n_chaves);
        pos = posChave(novo->chaves, novo->n_chaves, chave);
    }

    novo->ptr_node[0] = no->ptr_node[no->n_chaves];
    novo->ptr_node[pos] = direita;

    *chave_promovida = no->chaves[(no->n_chaves) - 1];

    no->chaves[(no->n_chaves) - 1] = -1;
    no->n_chaves--;
}

bool buscaInsere(Node *atual, int chave, bool *promocao, int *chave_promovida, Node *novo, int ordem){

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
        printf("Chave nao existe\n");
        if(atual->folha){   //Nó atual é folha
            printf("Eh folha\n");
            if(atual->n_chaves < ordem - 1){    //Nó não cheio
                printf("Nao cheio\n");
                atual->chaves[(atual->n_chaves)] = chave;
                atual->n_chaves++;
                atual->ptr_node[(atual->n_chaves) + 1] = NULL;
                *promocao = false;
                insertionSort(atual->chaves, atual->n_chaves);

            } else {                            //Nó cheio
                printf("Cheio\n");
                divide(atual, chave, NULL, novo, chave_promovida, ordem);
                *promocao = true;
            }

            return true;

        } else {            //Nó atual é interno

            i = posChave(atual->chaves, atual->n_chaves, chave);

            bool status = buscaInsere(atual->ptr_node[i], chave, promocao, chave_promovida, novo, ordem);

            if(status == true && *promocao == true){
                if(atual->n_chaves < ordem - 1){
                    atual->chaves[atual->n_chaves] = chave_promovida;
                    atual->n_chaves++;
                    atual->ptr_node[atual->n_chaves] = novo;
                    promocao = false;
                } else {
                    Node *filho = criaNode(filho, ordem);
                    divide(atual, chave, novo, filho, chave_promovida, ordem);
                    novo = filho;
                }
            }

            return status;
        }
    }


}


bool insere(Node **raiz, int chave, int ordem){

    if(*raiz == NULL){ //Árvore vazia.
        printf("Arvore vazia, insere na raiz\n");
        *raiz = criaNode(*raiz, ordem);
        (*raiz)->chaves[0] = chave;
        (*raiz)->n_chaves++;

        return true;

    } else {                //Árvore não vazia
        printf("Arvore nao vazia\n");
        bool status, promocao;
        int chave_promovida;
        Node *novo = criaNode(novo, ordem);

        status = buscaInsere(*raiz, chave, &promocao, &chave_promovida, novo, ordem);

        if(promocao == true){
            Node *nova_raiz = criaNode(nova_raiz, ordem);
            nova_raiz->folha = false;

            nova_raiz->chaves[0] = chave_promovida;
            nova_raiz->ptr_node[0] = *raiz;
            nova_raiz->ptr_node[1] = novo;

            *raiz = nova_raiz;
        }

        return status;
    }
}

void printArvore(Node *no, int ordem){
    int i;
    printf("FOLHA: %d\n", no->folha);
    printf("N_CHAVES: %d\n", no->n_chaves);
    for(i = 0; i < ordem - 1; i++){
        printf("CHAVE[%d]: %d\n", i, no->chaves[i]);
    }
    for(i = 0; i < ordem; i++){
        printf("PTR_NODE[%d]: %d\n", i, no->ptr_node[i]);
    }
    printf("\n");
}

int main(){

    int ordem;
    scanf("%d", &ordem);

    Node* raiz = NULL;

    insere(&raiz, 4, ordem);
    //printArvore(raiz, ordem);

    insere(&raiz, 10, ordem);
    //printArvore(raiz, ordem);

    insere(&raiz, 1, ordem);
    //printArvore(raiz, ordem);

    insere(&raiz, 3, ordem);
    printArvore(raiz, ordem);
    //printArvore(raiz->ptr_node[0], ordem);
    //printArvore(raiz->ptr_node[1], ordem);

    return 0;

}
