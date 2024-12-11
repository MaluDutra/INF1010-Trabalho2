#include <stdio.h>
#include <stdlib.h>

typedef struct no {
    int n1; //chave 1
    int n2; //chave 2
    int overflow;
    struct no* p1; //ponteiro filho esquerdo
    struct no* p2; //ponteiro filho meio
    struct no* p3; //ponteiro filho direito
    struct no* pai;
    struct no* pOverflow;
    int ehFolha; //indica se é folha (1 - sim || 0 - não)
} No;

No* criaNo(int ehFolha);
No* insereFolha(No* folha, int val);
No* cisaoFolha(No* noFilho, No* noPai);
No* insereArvore(No* arvoreBMais, int val);
void verificaPai(No* noPai);
No* removeNo(No* arvoreBMais, int val);
No* corrigeNos(No* arvore);
No* roubaEsq(No* arvore, int indexPai);
No* roubaDir(No* arvore, int indexPai);
No* concatDir(No* arvore);
No* buscaChave(No* arvoreBMais, int val);
void exibeNo(No* no);
void exibeArvore(No* arvoreBMais);

No* criaNo(int ehFolha){
    No* no_raiz = (No*)malloc(sizeof(No));
    no_raiz->n1 = -1;
    no_raiz->n2 = -1;
    no_raiz->overflow = -1;
    no_raiz->p1 = NULL;
    no_raiz->p2 = NULL;
    no_raiz->p3 = NULL;
    no_raiz->pai = NULL;
    no_raiz->pOverflow = NULL;
    no_raiz->ehFolha = ehFolha;
    return no_raiz;
}

No* insereFolha(No* folha, int val){
    if (folha->n1 == -1){ //primeira posição vazia
        folha->n1 = val;
    } else if (folha->n2 == -1){ //segunda posição vazia
        if (val > folha->n1){
            folha->n2 = val;
        } else{
            int aux = folha->n1;
            folha->n1 = val;
            folha->n2 = aux;
        }
    } else{ //duas primeiras posições cheias --> overflow do nó
        if (val > folha->n2){
            folha->overflow = val;
        } else if (val > folha->n1){
            int aux = folha->n2;
            folha->n2 = val;
            folha->overflow = aux;
        } else{
            int aux = folha->n1;
            int auxaux = folha->n2;
            folha->n1 = val;
            folha->n2 = aux;
            folha->overflow = auxaux;
        }
    }
    return folha;
}

No* cisaoFolha(No* noFilho, No* noPai){
    if (noPai == NULL){
        noPai = criaNo(0);
    }
    
    int ehFolha = noFilho->ehFolha;
    if (noPai->n1 == -1){ //novo nó pai --> primeira posição do pai vazia
        noPai->n1 = noFilho->n2; 
        
        No* novoNo = criaNo(ehFolha);
        if (ehFolha){
            novoNo->n1 = noFilho->n2;
            novoNo->n2 = noFilho->overflow;
        } else {
            novoNo->n1 = noFilho->overflow;
        }
        novoNo->p1 = noFilho->p3;
        novoNo->p2 = noFilho->pOverflow;
        novoNo->pai = noPai;
        noPai->p2 = novoNo;
        
        noFilho->n2 = -1;
        noFilho->overflow = -1;
        noFilho->p3 = NULL;
        noFilho->pOverflow = NULL;
        noFilho->pai = noPai;
        noPai->p1 = noFilho;
    } else if (noPai->n2 == -1){ //primeira posição do pai ocupada e segunda posição vazia
        if (noFilho->n2 > noPai->n1){
            noPai->n2 = noFilho->n2;

            No* novoNo = criaNo(ehFolha);
            
            if (ehFolha){
                novoNo->n1 = noFilho->n2;
                novoNo->n2 = noFilho->overflow;
                novoNo->pai = noPai;
                noPai->p3 = novoNo;
                
                noFilho->n2 = -1;
                noFilho->overflow = -1;
                noFilho->pai = noPai;
                noPai->p2 = noFilho;
            } else {
                novoNo->n1 = noFilho->overflow;
                novoNo->p1 = noFilho->p3;
                novoNo->p2 = noFilho->pOverflow;
                novoNo->pai = noPai;
                noPai->p3 = novoNo;
                
                noFilho->n2 = -1;
                noFilho->overflow = -1;
                noFilho->p3 = NULL;
                noFilho->pOverflow = NULL;
                noFilho->pai = noPai;
                noPai->p2 = noFilho;
            }
        } else{
            noPai->n2 = noPai->n1;
            noPai->n1 = noFilho->n2;

            noPai->p3 = noPai->p2;
            No* novoNo = criaNo(ehFolha);
            
            if (ehFolha){
                novoNo->n1 = noFilho->n2;
                novoNo->n2 = noFilho->overflow;
                novoNo->pai = noPai;
                noPai->p2 = novoNo;
                
                noFilho->n2 = -1;
                noFilho->overflow = -1;
                noFilho->pai = noPai;
                noPai->p1 = noFilho;
            } else {
                novoNo->n1 = noFilho->overflow;
                novoNo->p1 = noFilho->p3;
                novoNo->p2 = noFilho->pOverflow;
                novoNo->pai = noPai;
                noPai->p2 = novoNo;
                
                noFilho->n2 = -1;
                noFilho->overflow = -1;
                noFilho->p3 = NULL;
                noFilho->pOverflow = NULL;
                noFilho->pai = noPai;
                noPai->p1 = noFilho;
            }
        }
    } else if (noPai->overflow == -1){
        if (noFilho->n2 < noPai->n1){
            noPai->overflow = noPai->n2;
            noPai->n2 = noPai->n1;
            noPai->n1 = noFilho->n2;
    
            noPai->pOverflow = noPai->p3;
            noPai->p3 = noPai->p2;
    
            No* novoNo = criaNo(ehFolha);
            
            if (ehFolha){
                novoNo->n1 = noFilho->n2;
                novoNo->n2 = noFilho->overflow;
                noPai->p2 = novoNo;
                
                noFilho->n2 = -1;
                noFilho->overflow = -1;
                noPai->p1 = noFilho;
                
                novoNo->pai = noPai->p2;
                noFilho->pai = noPai->p2;
            } else {
                novoNo->n1 = noFilho->overflow;
                novoNo->p1 = noFilho->p3;
                novoNo->p2 = noFilho->pOverflow;
                noPai->p2 = novoNo;
                
                noFilho->n2 = -1;
                noFilho->overflow = -1;
                noFilho->p3 = NULL;
                noFilho->pOverflow = NULL;
                noPai->p1 = noFilho;
                
                novoNo->pai = noPai->p2;
                noFilho->pai = noPai->p2;
            }
        } else if (noFilho->n2 < noPai->n2){
            noPai->overflow = noPai->n2;
            noPai->n2 = noFilho->n2;

            noPai->pOverflow = noPai->p3;
            No* novoNo = criaNo(ehFolha);
            
            if (ehFolha){
                novoNo->n1 = noFilho->n2;
                novoNo->n2 = noFilho->overflow;
                noPai->p3 = novoNo;
                
                noFilho->n2 = -1;
                noFilho->overflow = -1;
                noPai->p2 = noFilho;
                
                novoNo->pai = noPai->p2;
                noFilho->pai = noPai->p2;
            } else {
                novoNo->n1 = noFilho->overflow;
                novoNo->p1 = noFilho->p3;
                novoNo->p2 = noFilho->pOverflow;
                noPai->p3 = novoNo;
                
                noFilho->n2 = -1;
                noFilho->overflow = -1;
                noFilho->p3 = NULL;
                noFilho->pOverflow = NULL;
                noPai->p2 = noFilho;
                
                novoNo->pai = noPai->p2;
                noFilho->pai = noPai->p2;
            }
        } else{
            noPai->overflow = noFilho->n2;

            No* novoNo = criaNo(ehFolha);
            
            if (ehFolha){
                novoNo->n1 = noFilho->n2;
                novoNo->n2 = noFilho->overflow;
                noPai->pOverflow = novoNo;
                
                noFilho->n2 = -1;
                noFilho->overflow = -1;
                noPai->p3 = noFilho;
                
                novoNo->pai = noPai->p2;
                noFilho->pai = noPai->p2;
            } else {
                novoNo->n1 = noFilho->overflow;
                novoNo->p1 = noFilho->p3;
                novoNo->p2 = noFilho->pOverflow;
                noPai->pOverflow = novoNo;
                
                noFilho->n2 = -1;
                noFilho->overflow = -1;
                noFilho->p3 = NULL;
                noFilho->pOverflow = NULL;
                noPai->p3 = noFilho;
                
                novoNo->pai = noPai->p2;
                noFilho->pai = noPai->p2;
            }
        }
    }
    return noPai;
}

No* insereArvore(No* arvoreBMais, int val){
    No* noAtual = arvoreBMais;
    No* noPai = NULL;
    
    while (!noAtual->ehFolha){ //percorrer a árvore até chegar na folha
        if (val < noAtual->n1){
            noAtual = noAtual->p1; //percorre o filho esquerdo
        } else if (val < noAtual->n2 || noAtual->n2 == -1){  //percorre o filho do meio
            noAtual = noAtual->p2;
        } else{ //percorre o filho direito
            noAtual = noAtual->p3;
        }
    }
    noAtual = insereFolha(noAtual, val);
    
    if (noAtual->overflow == -1){ //insere normalmente
        return arvoreBMais; //não mudou os ponteiros, então retorna a raiz
    }
    
    while (noAtual != arvoreBMais){
        if (noAtual->overflow != -1){
            noAtual->pai = cisaoFolha(noAtual, noAtual->pai);
        }
        
        noAtual = noAtual->pai;
    }
    

    if (noAtual->overflow != -1){ //noAtual == arvoreBMais
        noAtual->pai = cisaoFolha(noAtual, noAtual->pai);
        noAtual = noAtual->pai;
    }
    
    verificaPai(noAtual);
    return noAtual;
}

void verificaPai(No* noPai){
    if (noPai == NULL) return;
    No* noEsq = noPai->p1;
    No* noMeio = noPai->p2;
    No* noDir = noPai->p3;

    if (noEsq != NULL) {
        noEsq->pai = noPai;
        verificaPai(noEsq);
    }

    if (noMeio != NULL) {
        noMeio->pai = noPai;
        verificaPai(noMeio);
    }

    if (noDir != NULL) {
        noDir->pai = noPai;
        verificaPai(noDir);
    }
}

No* removeNo(No* arvoreBMais, int val){
    int aux;
    No* arvAux = arvoreBMais;
    No* arvAuxAux;
    while (1){ // achar e remover chave
        if (arvAux->ehFolha && (arvAux->n1 == val)){ // n1 removido
            if(arvAux->n2 != -1){ // existe n2
                arvAux->n1 = arvAux->n2;
                aux = arvAux->n1;
                arvAux->n2 = -1;
                while(arvAux->pai){ // altera valores dos nós internos
                    arvAux = arvAux->pai;
                    if(arvAux->n1 == val){
                        arvAux->n1 = aux;
                    }
                    else if (arvAux->n2 == val){
                        arvAux->n2 = aux;
                    }
                }
            }
            else { // não existe n2
                if (!arvAux->pai){
                    return NULL;
                }
                arvAux = arvAux->pai;
                if (val < arvAux->n1){ // remover do nó da esquerda
                    if (arvAux->p2->n2 != -1){ // caso tenha 2 chaves na direita
                        aux = arvAux->p2->n1;
                        arvAux->p2->n1 = arvAux->p2->n2;
                        arvAux->p2->n2 = -1;
                        arvAux->n1 = arvAux->p2->n1;
                        arvAux->p1->n1 = aux;
                        while(arvAux->pai){ // altera valores dos nós internos
                            arvAux = arvAux->pai;
                            if(arvAux->n1 == val){
                                arvAux->n1 = aux;
                            }
                            else if (arvAux->n2 == val){
                                arvAux->n2 = aux;
                            }
                        }
                    }
                    else { // não tem 2 chaves na direita
                        if (arvAux->n2 != -1){ // tem 2 chaves no nó interno
                            free(arvAux->p1);
                            arvAux->n1 = arvAux->n2;
                            arvAux->n2 = -1;
                            arvAux->p1 = arvAux->p2;
                            arvAux->p2 = arvAux->p3;
                            arvAux->p3 = NULL;
                            aux = arvAux->p1->n1;
                            while(arvAux->pai){ // altera valores dos nós internos
                                arvAux = arvAux->pai;
                                if(arvAux->n1 == val){
                                    arvAux->n1 = aux;
                                }
                                else if (arvAux->n2 == val){
                                    arvAux->n2 = aux;
                                }
                            }
                        }
                        else { // tem apenas uma chave no nó interno
                            if (!arvAux->pai){ // está no nó raiz
                                free(arvAux->p1);
                                free(arvAux->p2);
                                arvAux->p1 = NULL;
                                arvAux->p2 = NULL;
                                arvAux->ehFolha = 1;
                            }
                            else if (val < arvAux->pai->n1){ // valor está no ramo da esquerda
                                if (arvAux->pai->n2 != -1){ // nó pai possui 2 chaves
                                    if (arvAux->pai->p2->n2 != -1){ // nó da direita possui 2 chaves
                                        free(arvAux->p1);
                                        arvAux->p1 = arvAux->p2;
                                        arvAux->p2 = arvAux->pai->p2->p1;
                                        arvAux->p2->pai = arvAux;
                                        arvAux->n1 = arvAux->pai->n1;
                                        arvAux->pai->n1 = arvAux->pai->p2->n1;
                                        arvAux->pai->p2->n1 = arvAux->pai->p2->n2;
                                        arvAux->pai->p2->n2 = -1;
                                        arvAux->pai->p2->p1 = arvAux->pai->p2->p2;
                                        arvAux->pai->p2->p2 = arvAux->pai->p2->p3;
                                        arvAux->pai->p2->p3 = NULL;
                                        aux = arvAux->p1->n1;
                                        while(arvAux->pai){ // altera valores dos nós internos
                                            arvAux = arvAux->pai;
                                            if(arvAux->n1 == val){
                                                arvAux->n1 = aux;
                                            }
                                            else if (arvAux->n2 == val){
                                                arvAux->n2 = aux;
                                            }
                                        }
                                    }
                                    else { // nó da direita possui apenas 1 chave
                                        free(arvAux->p1);
                                        arvAux->p1 = arvAux->p2;
                                        arvAux->p2 = arvAux->pai->p2->p1;
                                        arvAux->p2->pai = arvAux;
                                        arvAux->p3 = arvAux->pai->p2->p2;
                                        arvAux->p3->pai = arvAux;
                                        arvAux->n1 = arvAux->p2->n1;
                                        arvAux->n2 = arvAux->p3->n1;
                                        arvAux->pai->n1 = arvAux->pai->n2;
                                        arvAux->pai->n2 = -1;
                                        free(arvAux->pai->p2);
                                        arvAux->pai->p2 = arvAux->pai->p3;
                                        arvAux->pai->p3 = NULL;
                                        aux = arvAux->p1->n1;
                                        while(arvAux->pai){ // altera valores dos nós internos
                                            arvAux = arvAux->pai;
                                            if(arvAux->n1 == val){
                                                arvAux->n1 = aux;
                                            }
                                            else if (arvAux->n2 == val){
                                                arvAux->n2 = aux;
                                            }
                                        }
                                    }
                                }
                                else { // nó pai possui 1 chave
                                    if (arvAux->pai->p2->n2 != -1){ // nó da direita possui 2 chaves
                                        free(arvAux->p1);
                                        arvAux->p1 = arvAux->p2;
                                        arvAux->p2 = arvAux->pai->p1->p1;
                                        arvAux->p2->pai = arvAux;
                                        arvAux->n1 = arvAux->p2->n1;
                                        arvAux->pai->n1 = arvAux->pai->p2->n1;
                                        arvAux->pai->p2->n1 = arvAux->pai->p2->n2;
                                        arvAux->pai->p2->n2 = -1;
                                        arvAux->pai->p2->p1 = arvAux->pai->p2->p2;
                                        arvAux->pai->p2->p2 = arvAux->pai->p2->p3;
                                        arvAux->pai->p2->p3 = NULL;
                                        aux = arvAux->p1->n1;
                                        while(arvAux->pai){ // altera valores dos nós internos
                                            arvAux = arvAux->pai;
                                            if(arvAux->n1 == val){
                                                arvAux->n1 = aux;
                                            }
                                            else if (arvAux->n2 == val){
                                                arvAux->n2 = aux;
                                            }
                                        }
                                    }
                                    else { // nó da direita possui apenas 1 chave
                                        free(arvAux->p1);
                                        arvAux->p1 = arvAux->p2;
                                        arvAux->p2 = arvAux->pai->p2->p1;
                                        arvAux->p2->pai = arvAux;
                                        arvAux->p3 = arvAux->pai->p2->p2;
                                        arvAux->p3->pai = arvAux;
                                        arvAux->n1 = arvAux->p2->n1;
                                        arvAux->n2 = arvAux->p3->n1;
                                        arvAux->pai->n1 = -1;
                                        aux = arvAux->p1->n1;
                                        free(arvAux->pai->p2);
                                        arvAuxAux = arvAux;
                                        while(arvAuxAux->pai){ // altera valores dos nós internos
                                            arvAuxAux = arvAuxAux->pai;
                                            if(arvAuxAux->n1 == val){
                                                arvAuxAux->n1 = aux;
                                            }
                                            else if (arvAuxAux->n2 == val){
                                                arvAuxAux->n2 = aux;
                                            }
                                        }
                                        corrigeNos(arvAux->pai);
                                    }
                                }
                            }
                            else if (arvAux->pai->n2 == -1 || val < arvAux->pai->n2){ // valor está no ramo do meio
                                if (arvAux->pai->n2 != -1){ // nó pai possui 2 chaves
                                    if (arvAux->pai->p1->n2 != -1){ // nó da esquerda possui 2 chaves
                                        free(arvAux->p1);
                                        arvAux->p1 = arvAux->pai->p1->p3;
                                        arvAux->p1->pai = arvAux;
                                        arvAux->pai->n1 = arvAux->p1->n1;
                                        arvAux->pai->p1->n2 = -1;
                                        arvAux->pai->p1->p3 = NULL;
                                    }
                                    else if (arvAux->pai->p3->n2 != -1) { // nó da direita possui 2 chaves
                                        free(arvAux->p1);
                                        arvAux->p1 = arvAux->p2;
                                        arvAux->p2 = arvAux->pai->p3->p1;
                                        arvAux->p2->pai = arvAux;
                                        arvAux->n1 = arvAux->p2->n1;
                                        arvAux->pai->n1 = arvAux->p1->n1;
                                        arvAux->pai->n2 = arvAux->pai->p3->n1;
                                        arvAux->pai->p3->n1 = arvAux->pai->p3->n2;
                                        arvAux->pai->p3->n2 = -1;
                                        arvAux->pai->p3->p1 = arvAux->pai->p3->p2;
                                        arvAux->pai->p3->p2 = arvAux->pai->p3->p3;
                                        arvAux->pai->p3->p3 = NULL;
                                    }
                                    else { // nós aos lados possuem apenas 1 chave
                                        free(arvAux->p1);
                                        arvAux = arvAux->pai->p1;
                                        arvAux->p3 = arvAux->pai->p2->p2;
                                        arvAux->p3->pai = arvAux;
                                        arvAux->n2 = arvAux->pai->p2->n1;
                                        arvAux->pai->n1 = arvAux->pai->n2;
                                        arvAux->pai->n2 = -1;
                                        free(arvAux->pai->p2);
                                        arvAux->pai->p2 = arvAux->pai->p3;
                                        arvAux->pai->p3 = NULL;
                                    }
                                }
                                else { // nó pai possui 1 chave
                                    if (arvAux->pai->p1->n2 != -1){ // nó da esquerda possui 2 chaves
                                        free(arvAux->p1);
                                        arvAux->p1 = arvAux->pai->p1->p3;
                                        arvAux->p1->pai = arvAux;
                                        arvAux->pai->n1 = arvAux->p1->n1;
                                        arvAux->pai->p1->n2 = -1;
                                        arvAux->pai->p1->p3 = NULL;
                                    }
                                    else { // nó da esquerda possui apenas 1 chave
                                        free(arvAux->p1);
                                        arvAux = arvAux->pai->p1;
                                        arvAux->p3 = arvAux->pai->p2->p2;
                                        arvAux->p3->pai = arvAux;
                                        arvAux->n2 = arvAux->p3->n1;
                                        arvAux->pai->n1 = -1;
                                        free(arvAux->pai->p2);
                                        corrigeNos(arvAux->pai);
                                    }
                                }
                            }
                            else if (val >= arvAux->pai->n2){ // valor está no ramo da direita
                                if (arvAux->pai->p2->n2 != -1){ // nó da esquerda possui 2 chaves
                                    free(arvAux->p1);
                                    arvAux->p1 = arvAux->pai->p2->p3;
                                    arvAux->p1->pai = arvAux;
                                    arvAux->pai->n2 = arvAux->p1->n1;
                                    arvAux->pai->p2->n2 = -1;
                                    arvAux->pai->p2->p3 = NULL;
                                }
                                else { // nó da esquerda possui apenas 1 chave
                                    free(arvAux->p1);
                                    arvAux = arvAux->pai->p2;
                                    arvAux->p3 = arvAux->pai->p3->p2;
                                    arvAux->p3->pai = arvAux;
                                    arvAux->n2 = arvAux->p3->n1;
                                    arvAux->pai->n2 = -1;
                                    free(arvAux->pai->p3);
                                    arvAux->pai->p3 = NULL;
                                }
                            }
                        }
                    }
                }
                else if (val == arvAux->n1){ // remover do nó do meio
                    if (arvAux->p1->n2 != -1){ // caso tenha 2 chaves na esquerda
                        aux = arvAux->p1->n2;
                        arvAux->p1->n2 = -1;
                        arvAux->n1 = aux;
                        arvAux->p2->n1 = aux;
                        while(arvAux->pai){ // altera valores dos nós internos
                            arvAux = arvAux->pai;
                            if(arvAux->n1 == val){
                                arvAux->n1 = aux;
                            }
                            else if (arvAux->n2 == val){
                                arvAux->n2 = aux;
                            }
                        }
                    }
                    else if ((arvAux->p3) && (arvAux->p3->n2 != -1)){ // caso tenha 2 chaves na direita
                        aux = arvAux->p3->n1;
                        arvAux->p3->n1 = arvAux->p3->n2;
                        arvAux->p3->n2 = -1;
                        arvAux->n2 = arvAux->p3->n1;
                        arvAux->p2->n1 = aux;
                        arvAux->n1 = aux;
                        while(arvAux->pai){ // altera valores dos nós internos
                            arvAux = arvAux->pai;
                            if(arvAux->n1 == val){
                                arvAux->n1 = aux;
                            }
                            else if (arvAux->n2 == val){
                                arvAux->n2 = aux;
                            }
                        }
                    }
                    else { // não tem 2 chaves aos lados
                        if (arvAux->p3){ // tem 2 chaves no nó interno
                            free(arvAux->p2);
                            arvAux->p2 = arvAux->p3;
                            arvAux->p3 = NULL;
                            arvAux->n1 = arvAux->p2->n1;
                            arvAux->n2 = -1;
                        }
                        else { // tem apenas 1 chave no nó interno
                            if (!arvAux->pai){ // está no nó raiz
                                aux = arvAux->p1->n1;
                                free(arvAux->p1);
                                free(arvAux->p2);
                                arvAux->n1 = aux;
                                arvAux->p1 = NULL;
                                arvAux->p2 = NULL;
                                arvAux->ehFolha = 1;
                            }
                            else if (val < arvAux->pai->n1){ // valor está no ramo da esquerda
                                if (arvAux->pai->n2 != -1){ // nó pai possui 2 chaves
                                    if (arvAux->pai->p2->n2 != -1){ // nó da direita possui 2 chaves
                                        free(arvAux->p2);
                                        arvAux->p2 = arvAux->pai->p2->p1;
                                        arvAux->p2->pai = arvAux;
                                        arvAux->n1 = arvAux->p2->n1;
                                        arvAux->pai->n1 = arvAux->pai->p2->n1;
                                        arvAux->pai->p2->n1 = arvAux->pai->p2->n2;
                                        arvAux->pai->p2->n2 = -1;
                                        arvAux->pai->p2->p1 = arvAux->pai->p2->p2;
                                        arvAux->pai->p2->p2 = arvAux->pai->p2->p3;
                                        arvAux->pai->p2->p3 = NULL;
                                    }
                                    else { // nó da direita possui apenas 1 chave
                                        free(arvAux->p2);
                                        arvAux->p2 = arvAux->pai->p2->p1;
                                        arvAux->p2->pai = arvAux;
                                        arvAux->p3 = arvAux->pai->p2->p2;
                                        arvAux->p3->pai = arvAux;
                                        arvAux->n1 = arvAux->p2->n1;
                                        arvAux->n2 = arvAux->p3->n1;
                                        arvAux->pai->n1 = arvAux->pai->n2;
                                        arvAux->pai->n2 = -1;
                                        free(arvAux->pai->p2);
                                        arvAux->pai->p2 = arvAux->pai->p3;
                                        arvAux->pai->p3 = NULL;
                                    }
                                }
                                else { // nó pai possui 1 chave
                                    if (arvAux->pai->p2->n2 != -1){ // nó da direita possui 2 chaves
                                        free(arvAux->p2);
                                        arvAux->p2 = arvAux->pai->p2->p1;
                                        arvAux->p2->pai = arvAux;
                                        arvAux->n1 = arvAux->p2->n1;
                                        arvAux->pai->n1 = arvAux->pai->p2->n1;
                                        arvAux->pai->p2->n1 = arvAux->pai->p2->n2;
                                        arvAux->pai->p2->n2 = -1;
                                        arvAux->pai->p2->p1 = arvAux->pai->p2->p2;
                                        arvAux->pai->p2->p2 = arvAux->pai->p2->p3;
                                        arvAux->pai->p2->p3 = NULL;
                                    }
                                    else { // nó da direita possui apenas 1 chave
                                        free(arvAux->p2);
                                        arvAux->p2 = arvAux->pai->p2->p1;
                                        arvAux->p2->pai = arvAux;
                                        arvAux->p3 = arvAux->pai->p2->p2;
                                        arvAux->p3->pai = arvAux;
                                        arvAux->n1 = arvAux->p2->n1;
                                        arvAux->n2 = arvAux->p3->n1;
                                        arvAux->pai->n1 = -1;
                                        free(arvAux->pai->p2);
                                        corrigeNos(arvAux->pai);
                                    }
                                }
                            }
                            else if (val < arvAux->pai->n2 || arvAux->pai->n2 == -1){ // valor está no ramo do meio
                                if (arvAux->pai->n2 != -1){ // nó pai possui 2 chaves
                                    if (arvAux->pai->p1->n2 != -1){ // nó da esquerda possui 2 chaves
                                        free(arvAux->p2);
                                        arvAux->p2 = arvAux->p1;
                                        arvAux->p1 = arvAux->pai->p1->p3;
                                        arvAux->p1->pai = arvAux;
                                        arvAux->n1 = arvAux->p2->n1;
                                        arvAux->pai->n1 = arvAux->p1->n1;
                                        arvAux->pai->p1->n2 = -1;
                                        arvAux->pai->p1->p3 = NULL;
                                    }
                                    else if (arvAux->pai->p3->n2 != -1) { // nó da direita possui 2 chaves
                                        free(arvAux->p2);
                                        arvAux->p2 = arvAux->pai->p3->p1;
                                        arvAux->p2->pai = arvAux;
                                        arvAux->n1 = arvAux->p2->n1;
                                        arvAux->pai->n2 = arvAux->pai->p3->n1;
                                        arvAux->pai->p3->n1 = arvAux->pai->p3->n2;
                                        arvAux->pai->p3->n2 = -1;
                                        arvAux->pai->p3->p1 = arvAux->pai->p3->p2;
                                        arvAux->pai->p3->p2 = arvAux->pai->p3->p3;
                                        arvAux->pai->p3->p3 = NULL;
                                    }
                                    else { // nós aos lados possuem apenas 1 chave
                                        free(arvAux->p2);
                                        arvAux = arvAux->pai->p1;
                                        arvAux->p3 = arvAux->pai->p2->p1;
                                        arvAux->p3->pai = arvAux;
                                        arvAux->n2 = arvAux->p3->n1;
                                        arvAux->pai->n1 = arvAux->pai->n2;
                                        arvAux->pai->n2 = -1;
                                        free(arvAux->pai->p2);
                                        arvAux->pai->p2 = arvAux->pai->p3;
                                        arvAux->pai->p3 = NULL;                                        
                                    }
                                }
                                else { // nó pai possui 1 chave
                                    if (arvAux->pai->p1->n2 != -1){ // nó da esquerda possui 2 chaves
                                        free(arvAux->p2);
                                        arvAux->p2 = arvAux->p1;
                                        arvAux->p1 = arvAux->pai->p1->p3;
                                        arvAux->p1->pai = arvAux;
                                        arvAux->n1 = arvAux->p2->n1;
                                        arvAux->pai->n1 = arvAux->p1->n1;
                                        arvAux->pai->p1->n2 = -1;
                                        arvAux->pai->p1->p3 = NULL;
                                    }
                                    else { // nó da esquerda possui apenas 1 chave
                                        free(arvAux->p2);
                                        arvAux = arvAux->pai->p1;
                                        arvAux->p3 = arvAux->pai->p2->p1;
                                        arvAux->p3->pai = arvAux;
                                        arvAux->n2 = arvAux->p3->n1;
                                        arvAux->pai->n1 = -1;
                                        free(arvAux->pai->p2);
                                        corrigeNos(arvAux->pai);
                                    }
                                }
                            }
                            else if (val >= arvAux->pai->n2){ // valor está no ramo da direita
                                if (arvAux->pai->p2->n2 != -1){ // nó da esquerda possui 2 chaves
                                    free(arvAux->p2);
                                    arvAux->p2 = arvAux->p1;
                                    arvAux->p1 = arvAux->pai->p2->p3;
                                    arvAux->p1->pai = arvAux;
                                    arvAux->n1 = arvAux->p2->n1;
                                    arvAux->pai->n2 = arvAux->p1->n1;
                                    arvAux->pai->p2->n2 = -1;
                                    arvAux->pai->p2->p3 = NULL;
                                }
                                else { // nó da esquerda possui apenas 1 chave
                                    free(arvAux->p2);
                                    arvAux = arvAux->pai->p2;
                                    arvAux->p3 = arvAux->pai->p3->p1;
                                    arvAux->p3->pai = arvAux;
                                    arvAux->n2 = arvAux->p3->n1;
                                    arvAux->pai->n2 = -1;
                                    free(arvAux->pai->p3);
                                    arvAux->pai->p3 = NULL;
                                }
                            }
                        }
                    }
                }
                else if (val == arvAux->n2){ // remover do nó da direita
                    if (arvAux->p2->n2 != -1){ // caso tenha 2 chaves no meio
                        aux = arvAux->p2->n2;
                        arvAux->p2->n2 = -1;
                        arvAux->n2 = aux;
                        arvAux->p3->n1 = aux;
                    }
                    else { // não tem 2 chaves no meio
                        arvAux->n2 = -1;
                        free(arvAux->p3);
                        arvAux->p3 = NULL;
                    }
                }
            }
            break;
        }
        else if (arvAux->ehFolha && (arvAux->n2 == val)){ // n2 removido
            arvAux->n2 = -1;
            break;
        }
        else if (val < arvAux->n1){
            if (!arvAux->p1){
                printf("A chave não foi encontrada\n");
                return arvoreBMais;
            }
            arvAux = arvAux->p1;
        }
        else if ((val < arvAux->n2) || (arvAux->n2 == -1)){
            if (!arvAux->p2){
                printf("A chave não foi encontrada\n");
                return arvoreBMais;
            }
            arvAux = arvAux->p2;
        }
        else {
            if (!arvAux->p3){
                printf("A chave não foi encontrada\n");
                return arvoreBMais;
            }
            arvAux = arvAux->p3;
        }
    }

    return arvoreBMais;
}

No* corrigeNos(No* arvore){ // corrige a estrutura da arvore após concatenação
    No* arvAux;
    No* arvPai = arvore->pai;
    if (arvore->pai == NULL){
        if (arvore->p1 != NULL){ // transforma o nó em raiz caso o pai tenha sido removido
            arvAux = arvore->p1;
            arvore->n1 = arvAux->n1;
            arvore->n2 = arvAux->n2;
            arvore->p1 = arvAux->p1;
            arvore->p1->pai = arvore;
            arvore->p2 = arvAux->p2;
            arvore->p2->pai = arvore;
            arvore->p3 = arvAux->p3;
            arvore->p3->pai = arvore;
            free(arvAux);
            return arvore;
        }
    }
    int paiIndex;
    No* filhoVet[3] = {arvPai->p1, arvPai->p2, arvPai->p3};
    int filhoChaves1, filhoChaves2;
    int paiChaves;
    if (arvore->n1 == -1){
        arvPai = arvore->pai;
        for (paiIndex = 0; filhoVet[paiIndex] != arvore; paiIndex++);

        if (filhoVet[paiIndex-1] && filhoVet[paiIndex-1]->n2 == -1) filhoChaves1 = 1;
        else filhoChaves1 = 2;

        if (filhoVet[paiIndex+1] && filhoVet[paiIndex+1]->n2 == -1) filhoChaves2 = 1;
        else filhoChaves2 = 2;

        if (arvPai->n2 == -1) paiChaves = 1;
        else paiChaves = 2;

        if (paiIndex > 0 && filhoChaves1 > 1){ // rouba da esquerda caso o nó à esquerda tenha 2 chaves e o nó atual não seja o da esquerda
            roubaEsq(arvore, paiIndex);
        }
        else if (paiIndex < paiChaves && filhoChaves2 > 1){ // rouba da direita caso o nó à direita tenha 2 chaves e o nó atual não seja o da direita
            roubaDir(arvore, paiIndex);
        }
        else if (paiIndex == 0){ // caso seja o nó da esquerda e não consiga redistribuir, concatena com o da direita
            No* proxNo = concatDir(arvore);
            corrigeNos(proxNo->pai);
        }
        else { // caso não seja o nó da esquerda e não consiga redistribuir, concatena com o da esquerda
            No* proxNo = concatDir(filhoVet[paiIndex-1]);
            corrigeNos(proxNo->pai);
        }
    }
    return arvore;
}

No* roubaEsq(No* arvore, int indexPai){ // rouba as chaves do nó da esquerda
    No* arvVet[3] = {arvore->p1, arvore->p2, arvore->p3};
    No* arvPai = arvore->pai;
    No* paiVet[3] = {arvPai->p1, arvPai->p2, arvPai->p3};
    int arvQtdChaves;
    int arvChaves[2] = {arvore->n1, arvore->n2};
    int paiChaves[2] = {arvPai->n1, arvPai->n2};
    int irmaoQtdChaves;

    if (arvore->n1 == -1) arvQtdChaves = 0;
    else if (arvore->n2 == -1) arvQtdChaves = 1;
    else arvQtdChaves = 2;

    for (int i = arvQtdChaves - 1; i > 0; i--){ // abre espaço pra chave a ser colocada. não será necessário em uma árvore de 2 chaves
        arvChaves[i] = arvChaves[i-1];
    }

    No* irmaoEsq = paiVet[indexPai-1];
    No* irmaoVet[3] = {irmaoEsq->p1, irmaoEsq->p2, irmaoEsq->p3};
    int irmaoChaves[2] = {irmaoEsq->n1, irmaoEsq->n2};

    if (irmaoEsq->n2 == -1) irmaoQtdChaves = 1;
    else irmaoQtdChaves = 2;
    
    arvChaves[0] = paiChaves[indexPai-1]; // faz a distriuição das chaves
    paiChaves[indexPai-1] = irmaoChaves[irmaoQtdChaves-1];
    irmaoChaves[irmaoQtdChaves-1] = -1;
    arvQtdChaves += 1;

    for (int i = arvQtdChaves; i > 0; i--){ // move os filhos do nó para abrir espaço para o novo
        if (i == 1) arvore->p2 = arvore->p1;
        if (i == 2) arvore->p3 = arvore->p2;
        arvVet[i] = arvVet[i-1];
    }

    arvore->p1 = irmaoVet[irmaoQtdChaves]; // pega o filho do irmão da esquerda
    irmaoVet[irmaoQtdChaves] = NULL;
    if (irmaoQtdChaves == 2) irmaoEsq->p3 = NULL;
    if (irmaoQtdChaves == 1) irmaoEsq->p2 = NULL;
    if (irmaoQtdChaves == 0) irmaoEsq->p1 = NULL;
    arvore->p1->pai = arvore;

    arvore->n1 = arvChaves[0]; // organiza as chaves
    arvore->n2 = arvChaves[1];
    arvPai->n1 = paiChaves[0];
    arvPai->n2 = paiChaves[1];
    irmaoEsq->n1 = irmaoChaves[0];
    irmaoEsq->n2 = irmaoChaves[1];

    return arvore;
}

No* roubaDir(No* arvore, int indexPai){ // rouba as chaves do nó da direita
    No* arvVet[3] = {arvore->p1, arvore->p2, arvore->p3};
    No* arvPai = arvore->pai;
    No* paiVet[3] = {arvPai->p1, arvPai->p2, arvPai->p3};
    int arvQtdChaves;
    int arvChaves[2] = {arvore->n1, arvore->n2};
    int paiChaves[2] = {arvPai->n1, arvPai->n2};
    int irmaoQtdChaves;

    if (arvore->n1 == -1) arvQtdChaves = 0;
    else if (arvore->n2 == -1) arvQtdChaves = 1;
    else arvQtdChaves = 2;

    No* irmaoDir = paiVet[indexPai+1];
    No* irmaoVet[3] = {irmaoDir->p1, irmaoDir->p2, irmaoDir->p3};
    int irmaoChaves[2] = {irmaoDir->n1, irmaoDir->n2};

    if (irmaoDir->n1 == -1) irmaoQtdChaves = 0;
    else if (irmaoDir->n2 == -1) irmaoQtdChaves = 1;
    else irmaoQtdChaves = 2;

    arvChaves[arvQtdChaves] = paiChaves[indexPai]; // realiza a distriuição das chaves
    paiChaves[indexPai] = irmaoChaves[0];
    irmaoChaves[0] = -1;
    arvQtdChaves += 1;
    

    arvVet[arvQtdChaves] = irmaoVet[0]; // realiza a distriuição dos filhos
    if (arvQtdChaves == 0) arvore->p1 = irmaoVet[0];
    if (arvQtdChaves == 1) arvore->p2 = irmaoVet[0];
    if (arvQtdChaves == 2) arvore->p3 = irmaoVet[0];
    arvVet[arvQtdChaves]->pai = arvore;
    irmaoVet[0] = NULL;
    irmaoDir->p1 = NULL;

    for (int i = 1; i < irmaoQtdChaves+1; i++){ // move os filhos do nó da direita para tomar o espaço do que foi roubado
        if (i == 1){
            irmaoDir->p1 = irmaoDir->p2;
            irmaoDir->p2 = NULL;
        }
        if (i == 2){
            irmaoDir->p2 = irmaoDir->p3;
            irmaoDir->p3 = NULL;
        }
        irmaoVet[i-1] = irmaoVet[i];
        irmaoVet[i] = NULL;
    }

    for (int i = 0; i < irmaoQtdChaves; i++){ // move as chaves do nó da direita para tomar o espaço da que foi roubada
        irmaoChaves[i-1] = irmaoChaves[i];
        irmaoChaves[i] = -1;
    }

    arvore->n1 = arvChaves[0]; // organiza as chaves
    arvore->n2 = arvChaves[1];
    arvPai->n1 = paiChaves[0];
    arvPai->n2 = paiChaves[1];
    irmaoDir->n1 = irmaoChaves[0];
    irmaoDir->n2 = irmaoChaves[1];

    return arvore;
}

No* concatDir(No* arvore){ // concatena o nó com o nó da direita
    No* arvPai = arvore->pai;
    No* vetPai[3] = {arvPai->p1, arvPai->p2, arvPai->p3};
    No* vetArv[3] = {arvore->p1, arvore->p2, arvore->p3};
    int paiChaves[2] = {arvPai->n1, arvPai->n2};
    int arvChaves[2] = {arvore->n1, arvore->n2};
    int indexPai = 0;
    int arvQtdChaves;
    int paiQtdChaves;
    int irmaoDirQtd;

    if (arvore->n1 == -1) arvQtdChaves = 0;
    else if (arvore->n2 == -1) arvQtdChaves = 1;
    else arvQtdChaves = 2;

    if (arvPai->n1 == -1) paiQtdChaves = 0;
    else if (arvPai->n2 == -1) paiQtdChaves = 1;
    else paiQtdChaves = 2;

    for (indexPai = 0; vetPai[indexPai] != arvore; indexPai++);

    No* irmaoDir = vetPai[indexPai+1];
    No* vetIrmaoDir[3] = {irmaoDir->p1, irmaoDir->p2, irmaoDir->p3};
    int irmaoDirChaves[2] = {irmaoDir->n1, irmaoDir->n2};

    if (irmaoDir->n1 == -1) irmaoDirQtd = 0;
    else if (irmaoDir->n2 == -1) irmaoDirQtd = 1;
    else irmaoDirQtd = 2;
    
    arvChaves[arvQtdChaves] = paiChaves[indexPai]; // rouba uma chave do pai para a concatenação
    paiChaves[indexPai] = -1;

    for (int i = 0; i < irmaoDirQtd; i++){ // rouba uma chave do nó da direita para a concatenação
        int indexDestino = arvQtdChaves + 1 + i;
        arvChaves[indexDestino] = irmaoDirChaves[i];
    }
    
    for (int i = 0; i <= irmaoDirQtd; i++){ // rouba os filhos do nó da direita
        if (arvQtdChaves+1+i == 1){
            arvore->p2 = vetIrmaoDir[i];
            arvore->p2->pai = arvore;
        }
        else if (arvQtdChaves+1+i == 2){
            arvore->p3 = vetIrmaoDir[i];
            arvore->p3->pai = arvore;
        }
        vetArv[arvQtdChaves+1+i] = vetIrmaoDir[i];
        vetArv[arvQtdChaves+1+i]->pai = arvore;
    }

    for (int i = indexPai+1; i < paiQtdChaves; i++){ // move os filhos e as chaves do pai para ocupar o espaço da chave roubada
        if (i == 1){
            arvPai->p2 = arvPai->p3;
        }
        vetPai[i] = vetPai[i+1];
        paiChaves[i-1] = paiChaves[i];
        paiChaves[i] = -1;
    }
    free(irmaoDir);
    arvore->n1 = arvChaves[0]; // organiza as chaves
    arvore->n2 = arvChaves[1];
    arvPai->n1 = paiChaves[0];
    arvPai->n2 = paiChaves[1];

    return arvore;
}

No* buscaChave(No* arvoreBMais, int val){
    while (!arvoreBMais->ehFolha){ //percorrer a árvore até chegar na folha
        if (val < arvoreBMais->n1){
            arvoreBMais = arvoreBMais->p1; //percorre o filho esquerdo
        } else if (val < arvoreBMais->n2 || arvoreBMais->n2 == -1){  //percorre o filho do meio
            arvoreBMais = arvoreBMais->p2;
        } else{ //percorre o filho direito
            arvoreBMais = arvoreBMais->p3;
        }
    }
    if (arvoreBMais->n1 == val || arvoreBMais->n2 == val){
        return arvoreBMais;
    } else{
        return NULL;
    }
}

void exibeNo(No* no){
    printf("%p --> Nó: [%d, %d] (ptr1=%p, ptr2=%p, ptr3=%p)\n",
            no, no->n1, no->n2, no->p1, no->p2, no->p3);
}

//exibição em pré-ordem
void exibeArvore(No* arvoreBMais){
    if (arvoreBMais == NULL) return;
    exibeNo(arvoreBMais);

    if (!arvoreBMais->ehFolha){
        exibeArvore(arvoreBMais->p1);
        exibeArvore(arvoreBMais->p2);
        exibeArvore(arvoreBMais->p3);
    }
}

int main(){
    int nums[] = {6,7,8,9,10,11,12,13,14,1,2,3};
    No* arvoreBMais = criaNo(1); //criação do nó raiz inicialmente como folha

    for (int i = 0; i < 12; i++){
        printf("Inserindo %d na árvore...\n", nums[i]);
        arvoreBMais = insereArvore(arvoreBMais, nums[i]);
        printf("\nÁrvore atual:\n");
        exibeArvore(arvoreBMais);
        printf("-----------------------------------\n");
    }

    printf("\nÁrvore final:\n");
    exibeArvore(arvoreBMais);
    
    int buscaNum = 7;
    No* busca = buscaChave(arvoreBMais, buscaNum);
    if (busca == NULL){
        printf("\n\nChave %d não foi encontrada!\n", buscaNum);
    } else{
        printf("\n\nNó da chave %d desejada:\n", buscaNum);
        exibeNo(busca);
    }

    buscaNum = 90;
    busca = buscaChave(arvoreBMais, buscaNum);
    if (busca == NULL){
        printf("\n\nChave %d não foi encontrada!\n", buscaNum);
    } else{
        printf("\n\nNó da chave %d desejada:\n", buscaNum);
        exibeNo(busca);
    }
    
    buscaNum = 10;
    busca = buscaChave(arvoreBMais, buscaNum);
    if (busca == NULL){
        printf("\n\nChave %d não foi encontrada!\n", buscaNum);
    } else{
        printf("\n\nNó da chave %d desejada:\n", buscaNum);
        exibeNo(busca);
    }
    
    printf("\nÁrvore antes da deleção:\n");
    exibeArvore(arvoreBMais);
    
    printf("\nÁrvore depois da deleção da chave %d:\n", 2);
    removeNo(arvoreBMais, 2);
    exibeArvore(arvoreBMais);

    printf("\nÁrvore depois da deleção da chave %d:\n", 3);
    removeNo(arvoreBMais, 3);
    exibeArvore(arvoreBMais);

    printf("\nÁrvore depois da deleção da chave %d:\n", 8);
    removeNo(arvoreBMais, 8);
    exibeArvore(arvoreBMais);
    
    printf("\nÁrvore depois da deleção da chave %d:\n", 10);
    removeNo(arvoreBMais, 10);
    exibeArvore(arvoreBMais);

    printf("\nÁrvore depois da deleção da chave %d:\n", 9);
    removeNo(arvoreBMais, 9);
    exibeArvore(arvoreBMais);
    
    printf("\nTentando deletar chave %d que não está na árvore:\n", 9);
    removeNo(arvoreBMais, 9);
    
    printf("\nTentando deletar chave %d que não está na árvore:\n", 2);
    removeNo(arvoreBMais, 2);
    
    printf("\nTentando deletar chave %d que não está na árvore:\n", 8);
    removeNo(arvoreBMais, 8);
    
    printf("\nÁrvore final:\n");
    exibeArvore(arvoreBMais);
    
    return 0;
}