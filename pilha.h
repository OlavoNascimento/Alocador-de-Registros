#ifndef PILHA_H
#define PILHA_H

/*
Implementa uma Pilha. Uma pilha vazia é criada pela operação pilha_criar(), podendo receber uma
função para destruir as informações armazenadas ao liberar a pilha.
Após a criação, informações podem ser inseridas ao topo da pilha por meio da operação
pilha_inserir().
Para obter a informação no topo da pilha é possível utilizar a função pilha_obter_topo para
acessá-la ou a função pilha_remover para acessá-la ao mesmo tempo que a remove da pilha.
Ademais é possível verificar o tamanho da pilha através da função pilha_obter_tamanho e verificar se
a mesma está vazia utilizando a função pilha_esta_vazia.
*/

#include <stdbool.h>

typedef struct Pilha_s *Pilha;

typedef void *PilhaInfo;

typedef void PilhaDestruirInfo(PilhaInfo info);

/*
Cria uma pilha.
É possível especificar uma função que libere a memória alocada pelas informações da pilha, para que
ao liberar a memória da pilha suas informações também sejam liberadas.
Retorna o endereço para uma pilha vazia.
*/
Pilha pilha_criar(PilhaDestruirInfo destruir_info);

/*
Verifica se uma pilha está vazia.
O parâmetro pilha não pode ser nulo.
Retorna verdadeiro caso a pilha esteja vazio, falso caso contrário..
*/
bool pilha_esta_vazia(Pilha pilha);

/*
Retorna o tamanho de uma pilha.
O parâmetro pilha não pode ser nulo.
Retorna o tamanho da pilha.
*/
int pilha_obter_tamanho(Pilha pilha);

/*
Adiciona uma informação em uma pilha.
Ambos os parâmetros não podem ser nulos.
A informação é definida como o novo topo da pilha.
*/
void pilha_inserir(Pilha pilha, PilhaInfo info);

/*
Remove a informação no topo de uma pilha.
O parâmetro pilha não pode ser nulo.
A informação é retirada da pilha e o a informação a seguir é definida como o novo topo da pilha.
*/
PilhaInfo pilha_remover(Pilha pilha);

/*
Retorna a informação no topo da pilha sem remove-la.
O parâmetro pilha não pode ser nulo.
A info no topo da pilha é retornada.
*/
PilhaInfo pilha_obter_topo(Pilha pilha);

/*
Libera a memória alocada por uma pilha.
O parâmetro pilha não pode ser nulo.
A memória alocada pela pilha é liberada.
*/
void pilha_destruir(Pilha pilha);

#endif
