// Autor: Leonardo Coelho
// Arvore AVL

/*Rotacoes que devem ser implementadas
Os parametros e retorno sao de escolha do aluno
	rotation_LL();
	rotation_RR();
	rotation_LR();
	rotation_RL();
Obs: facam uma funcao para cada rotacao
Obs2: Essas funcoes sao exclusivas do avl.c, pois o usuario
NAO DEVE poder fazer uma dessas operacoes (ele pode apenas
inserir e remover na arvore). Assim, esses prototipos nao devem ir
no avl.h*/

#include "avl.h"

//Funcoes de impressao da arvore ja implementadas
void print_tab(int level){
  int i;
  for (i = 0; i < level; i++ )
    printf("\t");
}

void print_tree( Nodo_AVL *current, int level ){
  if ( current == NULL ) {
    print_tab(level );
    printf("~\n");
  } else {
    print_tree( current->right, level + 1 );
    print_tab(level);
    printf ("%d\n", current->key );
    print_tree( current->left, level + 1 );
  }
}



Nodo_AVL *avl_insert(Nodo_AVL *current, int key){
      if(current == NULL){ // se a arvore estiver vazia, inicia a arvore
        Nodo_AVL *current = malloc(sizeof(Nodo_AVL));
        current->key = key;
        current->height = 0;
        current->left = NULL;
        current->right = NULL;
        return(current);
    }

      if (key < current->key) // se a key for menor que a atual, insere na esquerda
          current->left  = avl_insert(current->left, key);
      else
        if (key > current->key) // se a key for maior que a atual, insere na direita
          current->right = avl_insert(current->right, key);
          else
              return current; // senao os valores sao iguais e so retorna a arvore nao modificada

      current->height = 1 + ((avl_height(current->left) > avl_height(current->right)) ? avl_height(current->left) : avl_height(current->right)); // adiciona a nova altura do nodo inserido

      // variavel para o fator de balanceamento
      int b = (current == NULL) ? 0 : avl_height(current->left) - avl_height(current->right);

      // Balanceamento em todos os casos
      if (b > 1 && key < current->left->key)
          return rotation_LL(current);

      if (b < -1 && key > current->right->key)
          return rotation_RR(current);

      if (b > 1 && key > current->left->key)
          return rotation_LR(current);

      if (b < -1 && key < current->right->key)
          return rotation_RL(current);

      // Se ja estiver balanceado
      return current;
}


Nodo_AVL *avl_delete(Nodo_AVL *current, int key){
    if (current == NULL) // Se a arvore estiver vazia, retorna a propria arvore (== NULL)
        return current;

    if (key < current->key) // se key < key do atual, verifica o filho da esquerda
        current->left = avl_delete(current->left, key);
    else
        if(key > current->key) // se key > key do atual, verifica o filho da direita
            current->right = avl_delete(current->right, key);

    else{ // se key == key do atual, achou um nodo com o valor
        if( (current->left == NULL) || (current->right == NULL) ){ // se ele ele nao tiver pelo menos 1 filho
            Nodo_AVL *aux = current->left ? current->left : current->right;

            if (aux == NULL) // se nao tiver filhos (folha)
            {
                aux = current;
                current = NULL; // tira aux de current
            }
            else // se tiver 1 filho
             *current = *aux; // current <- aux

            free(aux); // libera aux, apagando aquele nodo
        }
        else{ // se nodo-pai tiver 2 filhos
            Nodo_AVL *aux, *aux2 = current->right;

            // calculo do nodo mais inferior a esquerda (nodo minimo)
            while (aux2->left != NULL)
                aux2 = aux2->left;

            aux = aux2;

            current->key = aux->key; // key do atual = key do minimo

            current->right = avl_delete(current->right, aux->key); // verifica no filho da direita e seus descendentes
        }
    }

    if (current == NULL) // Se a arvore for nula, retorna NULL
      return current;

    current->height = 1 + ((avl_height(current->left) > avl_height(current->right)) ? avl_height(current->left) : avl_height(current->right)); // atualiza a altura dos nodos

    int b = (current == NULL) ? 0 : avl_height(current->left) - avl_height(current->right); // variavel para o fator de balaceamento

    // balanceamento (caso precisar)
    if (b > 1 && ((current->left == NULL) ? 0 : avl_height(current->left->left) - avl_height(current->left->right)) >= 0)
        return rotation_LL(current);

    if (b > 1 && ((current->left == NULL) ? 0 : avl_height(current->left->left) - avl_height(current->left->right)) < 0)
        return rotation_LR(current);

    if (b < -1 && ((current->right == NULL) ? 0 : avl_height(current->right->left) - avl_height(current->right->right)) <= 0)
        return rotation_RR(current);

    if (b < -1 && ((current->right == NULL) ? 0 : avl_height(current->right->left) - avl_height(current->right->right)) > 0)
        return rotation_RL(current);

    return current; // se nao precisar, retorna a arvore ja balanceada
}

void print_inOrder(Nodo_AVL *current){ // f.esq -> raiz -> f.dir
    if(current==NULL)
        return;

    print_inOrder(current->left);
    printf("%d\n", current->key);
    print_inOrder(current->right);
}


void avl_destroy(Nodo_AVL *current){
    if(current == NULL)
        return;
    if(current->left == NULL && current->right == NULL)
        free(current);

    if(current->left != NULL)
        avl_destroy(current->left);

    avl_destroy(current->right);

}


Nodo_AVL *avl_search(Nodo_AVL *current, int key){
    Nodo_AVL *aux = current;
    while((aux->key != key)){
        if(key < aux->key) aux = aux->left;
        if(key > aux->key) aux = aux->right;
    }
    if(aux->key == key) return aux;

    return NULL;
}


int avl_height(Nodo_AVL *raiz){
    if (raiz == NULL)
      return -1;

    return raiz->height;
}

Nodo_AVL *rotation_LL(Nodo_AVL *raiz){
    Nodo_AVL *aux1 = raiz->left;
    Nodo_AVL *aux2 = aux1->right;

    aux1->right = raiz;
    raiz->left = aux2;

    // atualiza as alturas
    raiz->height = 1 + ((avl_height(raiz->left) > avl_height(raiz->right)) ? avl_height(raiz->left) : avl_height(raiz->right));
    aux1->height = 1 + ((avl_height(aux1->left) > avl_height(aux1->right)) ? avl_height(aux1->left) : avl_height(aux1->right));

    return aux1;
}
Nodo_AVL *rotation_RR(Nodo_AVL *raiz){
    Nodo_AVL *aux1 = raiz->right;
    Nodo_AVL *aux2 = aux1->left;

    aux1->left = raiz;
    raiz->right = aux2;

    // atualiza as alturas
    raiz->height = 1 + ((avl_height(raiz->left) > avl_height(raiz->right)) ? avl_height(raiz->left) : avl_height(raiz->right));
    aux1->height = 1 + ((avl_height(aux1->left) > avl_height(aux1->right)) ? avl_height(aux1->left) : avl_height(aux1->right));

    return aux1;
}
Nodo_AVL *rotation_LR(Nodo_AVL *raiz){
    raiz->left = rotation_RR(raiz->left);
    return rotation_LL(raiz);
}
Nodo_AVL *rotation_RL(Nodo_AVL *raiz){
    raiz->right = rotation_LL(raiz->right);
    return rotation_RR(raiz);
}
