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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <malloc.h>
#include "jsbm.h"

#define MALLOC(l,t) (t *) malloc(((size_t) l) * sizeof(t))
#define TALLOC(t)   MALLOC(1, t)
#define erro(...)   fprintf(stderr, __VA_ARGS__)
#define out(...)    fprintf(stdout, __VA_ARGS__)

#define MAX_BENCHMARKS  (10)
#define NOT_FOUND       (-1)


static void print_memsize(const int data) {
    if (data < 1048576)
        out(" (%.2f KB)\n", ((float) data) / 1024);
    else
        out(" (%.2f MB)\n", ((float) data) / 1048576);
}


void meminfo(void) {
    struct mallinfo info = mallinfo();

    out("Informações da Memória");
    out("\tTotal de bytes alocados: %i bytes", info.arena);
    print_memsize(info.arena);
    out("\tPedaços não sendo usados: %i\n", info.ordblks);
    out("\tNúmero de pedaços alocados com mmap: %i\n", info.hblks);
    out("\tTamanho dos pedaços alocados com mmap: %i bytes", info.hblkhd);
    print_memsize(info.hblkhd);
    out("\tTotal alocado com malloc: %i bytes", info.uordblks);
    print_memsize(info.uordblks);
    out("\tTotal de memória não usada: %i bytes", info.fordblks);
    print_memsize(info.fordblks);
    out("\tPedaço de memóra mais liberado: %i bytes\n", info.keepcost);
}


static int procurar_benchmark(const char *nome) {
    int i;

    for (i=0; i<bm.tamanho; i++) 
        if (bm.lista[i]->nome != NULL) 
            if (strcmp(nome, bm.lista[i]->nome) == 0)
                return i;
    return NOT_FOUND;
}


int benchmark_salvar(const char *nome) {
    struct mallinfo info;
    int             i;

    i = procurar_benchmark(nome);
    if (i == NOT_FOUND) {
        if (bm.tamanho == MAX_BENCHMARKS) {
            erro("Erro: Número máximo de benchmarks atingido.\n");
            return 0;
        }
        i = bm.tamanho++;
        {
            struct benchmark_data   *nbm;

            nbm = TALLOC(struct benchmark_data);
            bm.lista[i] = nbm;
            strcpy(bm.lista[i]->nome, nome);
        }
    }
    info = mallinfo();
    bm.lista[i]->clock = clock();
    bm.lista[i]->alocado = info.uordblks;
    bm.lista[i]->time = time(NULL);
    return 1;
}


void benchmark_mostrar(const char *a, const char *b) {
    int bma = procurar_benchmark(a);
    int bmb = procurar_benchmark(b);
    int data = bm.lista[bmb]->alocado - bm.lista[bma]->alocado;
    long unsigned int    delta = (bm.lista[bmb]->clock - bm.lista[bma]->clock);

    out("Clock: %lu\n", delta);
    out("Tempo: %lf seg\n", ((double) delta * 10) / CLOCKS_PER_SEC);
    //difftime(bm.lista[bmb]->time, bm.lista[bma]->time));
    out("Memória: %i bytes", data - (int) sizeof(struct benchmark_data));
    print_memsize(data);
}


void benchmark_limpar(void) {
    //memset(bm.lista, 0, MAX_BENCHMARKS * sizeof(struct benchmark_data));
    bm.tamanho = 0;
}


__attribute__((__constructor__ (101)))
static void inicializar_jsbm(void) {
    bm.lista = MALLOC(MAX_BENCHMARKS, struct benchmark_data *);
    assert(bm.lista != NULL);
    benchmark_limpar();
}


__attribute__((__destructor__ (101)))
static void destruir_jsbm(void) {
    free(bm.lista);
}
