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
void insertionSort(int *array, int tamanho){    //Para descobrir a posição da chave no nó
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

    /*pos = tamanho - 1;

    while(tamanho > 0 && array[tamanho - 1] > chave){
        pos = tamanho - 1;
        tamanho--;
    }

    return pos;*/

}

bool divide(Node *nodu, int chave, Node *direita, Node *novo, int *chave_promovida, int ordem){
    /** REVISAR ESTE CÓDIGO DE DIVISÃO E MELHORÁ-LO **/
    if(nodu->folha == false){ //Nó a ser dividido é folha
        novo->folha = false;
    }

    int m = (int)ordem/2;

    if(chave < nodu->chaves[m]){

        int i, j = 0;   //i para for, j para vetor do novo nó
        for(i = m; i < ordem - 1; i++){
            novo->chaves[j] = nodu->chaves[i];
            novo->n_chaves++;
            nodu->n_chaves--;
            novo->ptr_node[j + 1] = nodu->ptr_node[i + 1];
            j++;
        }
        nodu->chaves[m] = chave;
        nodu->n_chaves++;

    } else {

        int i, j = 0;   //i para for, j para vetor do novo nó
        for(i = m + 1; i < ordem - 1; i++){
            novo->chaves[j] = nodu->chaves[i];
            novo->n_chaves++;
            nodu->n_chaves--;
            novo->ptr_node[j + 1] = nodu->ptr_node[i + 1];
            j++;
        }
        novo->chaves[(novo->n_chaves) + 1] = chave;
        novo->n_chaves++;
    }

    novo->ptr_node[0] = nodu->ptr_node[(nodu->n_chaves) + 1];

    insertionSort(nodu->chaves, nodu->n_chaves);
    insertionSort(novo->chaves, novo->n_chaves);

    chave_promovida = nodu->chaves[(nodu->n_chaves) - 1];
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

            } else {                            //Nó cheio
                divide(atual, chave, NULL, novo, chave_promovida, ordem);
                *promocao = true;
            }

            return true;

        } else {            //Nó atual é interno

        }
    }


}


bool insere(Node *raiz, int chave, int ordem){

    if(raiz->n_chaves == 0){ //Árvore vazia.
        printf("Arvore vazia, insere na raiz\n");
        raiz->chaves[0] = chave;
        raiz->n_chaves++;

        return true;

    } else {                //Árvore não vazia
        printf("Arvore nao vazia\n");
        bool status, promocao;
        int chave_promovida;
        Node *novo = criaNode(novo, ordem);;

        status = buscaInsere(raiz, chave, &promocao, &chave_promovida, novo, ordem);

        return status;
    }
}

int main(){

    int ordem;
    scanf("%d", &ordem);

    Node* raiz = criaNode(raiz, ordem);

    printf("%p\n", raiz);

    insere(raiz, 4, ordem);
    insere(raiz, 5, ordem);

    int i;
    for(i = 0; i < raiz->n_chaves; i++){
        printf("\nCHAVE[%d]: %d", i, raiz->chaves[i]);
    }
    printf("\n");

    return 0;

}
