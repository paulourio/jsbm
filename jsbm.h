/* Esta obra foi licenciada com a Licença Creative Commons
 * Attribution-NonCommercial 3.0 Brazil. Para ver uma cópia desta licença,
 * visite http://creativecommons.org/licenses/by-nc/3.0/br/ ou envie um 
 * pedido por carta para Creative Commons, 444 Castro Street, Suite 900, 
 * Mountain View, California, 94041, USA.
 *
 * Autor: Paulo Roberto Urio
 *
 * Biblioteca para simples medição de desempenho de trechos de códigos
 * na linguagem C.
 */
#ifndef JSBM_INCLUDE_H
#define JSBM_INCLUDE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <assert.h>
#include <malloc.h>
#include <features.h>

#define MAX_WORD_LEN    (30)

#define bm_iniciar  \
    { \
        benchmark_salvar("jsbm.auto.init"); \
    }

#define bm_finalizar \
    { \
        benchmark_salvar("jsbm.auto.end"); \
        benchmark_mostrar("jsbm.auto.init", "jsbm.auto.end"); \
        benchmark_limpar(); \
    }

struct benchmark_data {
    char    nome[MAX_WORD_LEN];
    int     alocado;
    clock_t clock;
    time_t  time;
};

struct benchmark {
    struct benchmark_data   **lista;
    int                     tamanho;
} bm;

/* Criar um benchmark
 * Salva o estado atual do programa salvando com o nome.
 * Parâmetros
 *      nome:   Nome do novo benchmark.
 * Retorno
 *      1 quando foi salvo com sucesso
 *      0 quando o número máximo de benchmarks for atingido.
 */
int benchmark_salvar(const char *nome) __nonnull((1));

/* Mostrar resultado de um benchmark
 * Compara o valor inicial "a" com o valor final "b" e mostra
 * na saída padrão o resultado do benchmark.
 * Parâmetros
 *      a: Nome do benchmark inicial
 *      b: Nome do benchmark final
 */
void benchmark_mostrar(const char *a, const char *b) __nonnull((1,2));

/* Remover todos os benchmarks salvos. */
void benchmark_limpar(void);

/* Imprimir na saída padrão os dados atuais da memória. */
void meminfo(void);

#endif
