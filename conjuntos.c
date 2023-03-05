#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#ifdef _WIN32
#define clear_screen system("cls")
#else
#define clear_screen system("clear")
#endif

#define N 10 // MAX Conjuntos
#define M 100 // MAX Tamanho dos conjuntos

typedef struct {
    int size;
    int arr[3*M];
} set;

// AUXILIARES
void merge(int arr[], int l, int mid, int r);
void merge_sort(int arr[], int l, int r);
void sort(set* st) { merge_sort(st->arr, 0, st->size-1); }
int set_binary_search(int v, set st);
int empty_of_sets(int quantidade);
int full_of_sets(int quantidade);
int full_conj(int size);
void mostra_menu();
void placeholder();

// Le um indice de set valido
void read_valid_set(int *number, int quantidade, char msg[]){
    printf("%s", msg);
    scanf("%d", number);
    printf("\n");
    while (*number > quantidade || *number <= 0){
        if(*number >= 0) printf("Conjunto inexistente.\n");
        else printf("Insira um valor nao negativo.\n");
        printf("\n");
        printf("%s", msg);
        scanf("%d", number);
        printf("\n");
    }
}

// Printa um set e, caso show != 0, destaca value 
void mostra_conjunto(set st, int show, int value){
    if(st.size == 0){
        printf("vazio\n");
        return;
    }
    printf("{");
    for(int i = 0; i < st.size; i++){
        if(show && st.arr[i] == value) printf("[");
        printf("%d", st.arr[i]);
        if(show && st.arr[i] == value) printf("]");
        printf("%c", " }"[i == st.size-1]);
    }
    printf("\n");
}

// mostra uma matriz (que na verdade eh so um vetor) de sets
void mostra_matriz(set m[N], int quantidade){
    if(empty_of_sets(quantidade)) return;
    for(int i = 0; i < quantidade; i++){
        printf("Conjunto %d: ", i+1);
        mostra_conjunto(m[i], 0, 0);
    }
}

// cria um set vazio na matriz de sets
void cria_conjunto(set m[N], int* quantidade){
    if(full_of_sets(*quantidade)) return;
    m[*quantidade].size = 0;
    (*quantidade)++;
    printf("Conjunto %d criado com sucesso!\n", (*quantidade));
}

// insere um valor em um set
void insere_no_conjunto(int v, set* st){
    int pos = st->size;
    st->arr[pos] = v;
    st->size++;
    while(pos > 0 && (st->arr[pos] < st->arr[pos-1])){
        int temp = st->arr[pos];
        st->arr[pos] = st->arr[pos-1];
        st->arr[pos-1] = temp;
        pos--;
    }
}

// insere dados em um set da matriz de sets
void insere_dados(set m[N], int quantidade){
    if(empty_of_sets(quantidade)) return;
    mostra_matriz(m, quantidade);
    printf("\n");
    char msg[] = "Escolha um conjunto para inserir dados: ";
    int conj;
    read_valid_set(&conj, quantidade, msg);
    if(full_conj(m[conj-1].size)) return;
    printf("Digite quantos valores deseja inserir: ");
    int add;
    scanf("%d", &add);
    printf("\n");
    int oldsize = m[conj-1].size;
    while (add + oldsize > M || add < 0){
        if(add >= 0) printf("Voce pode inserir no maximo %d valor(es).\n", M-oldsize);
        else printf("Insira um valor nao negativo.\n");
        printf("\n");
        printf("Digite quantos valores deseja inserir: ");
        scanf("%d", &add);
        printf("\n");
    }
    int newwsize = oldsize + add;
    for(int i = oldsize; i < newwsize; i++){
        int newwvalue;
        printf("Valor [%d]: ", i+1);
        scanf("%d", &newwvalue);
        printf("\n");
        while(set_binary_search(newwvalue, m[conj-1]) != -1){
            printf("Valor ja esta no conjunto.\n");
            printf("\n");
            printf("Valor [%d]: ", i+1);
            scanf("%d", &newwvalue);
            printf("\n");
        }
        insere_no_conjunto(newwvalue, &m[conj-1]);
    }
    printf("Valores inseridos com sucesso!\n");
    printf("\n");
    printf("Conjunto %d: ", conj);
    mostra_conjunto(m[conj-1], 0, 0);
}

// remove um set da matriz de sets
void remove_conjunto(set m[N], int* quantidade){
    if(empty_of_sets(*quantidade)) return;
    mostra_matriz(m, *quantidade);
    printf("\n");
    char msg[] = "Escolha um conjunto para ser removido: ";
    int conj;
    read_valid_set(&conj, *quantidade, msg);
    for(int i = conj-1; i < (*quantidade); i++){
        m[i] = m[i+1];
    }
    (*quantidade)--;
    printf("Conjunto %d removido com sucesso!\n", conj);
}

// une dois sets da matriz de sets
void uniao(set m[N], int* quantidade){
    if(full_of_sets(*quantidade)) return;
    else if (empty_of_sets(*quantidade)) return;
    else if (*quantidade < 2){
        printf("Voce deve possuir ao menos 2 conjuntos para poder fazer a uniao.\n");
        return;
    }
    mostra_matriz(m, *quantidade);
    printf("\n");
    char msg1[] = "Escolha o primeiro conjunto: ";
    int conj1;
    read_valid_set(&conj1, *quantidade, msg1);
    char msg2[] = "Escolha o segundo conjunto: ";
    int conj2;
    read_valid_set(&conj2, *quantidade, msg2);
    set temp = m[conj1-1];
    int estourou = 0;
    for(int i = 0; i < m[conj2-1].size; i++){
        int value = m[conj2-1].arr[i];
        if(set_binary_search(value, temp) == -1){
            if(temp.size+1 > M){
                estourou++;
            }
            insere_no_conjunto(value, &temp);
        }
    }
    if(estourou){
        printf("Impossivel unir os conjuntos %d e %d.\n", conj1, conj2);
        printf("%d uniao %d: ", conj1, conj2);
        mostra_conjunto(temp, 0, 0);
        printf("O resultado teria tamanho %d, porem o maximo permitido eh %d.\n", temp.size, M);
        return;
    }
    m[*quantidade] = temp;
    (*quantidade)++;
    if(conj1 != conj2) printf("Conjuntos %d e %d unidos com sucesso!\n", conj1, conj2);
    else printf("Conjunto %d unido consigo mesmo com sucesso!\n", conj1);
    printf("\n");
    printf("Conjunto %d: ", *quantidade);
    mostra_conjunto(temp, 0, 0);
}

// faz interseccao de dois sets da matriz de sets
void interseccao(set m[N], int* quantidade){
    if(full_of_sets(*quantidade)) return;
    else if (empty_of_sets(*quantidade)) return;
    else if (*quantidade < 2){
        printf("Voce deve possuir ao menos 2 conjuntos para poder fazer a interseccao.\n");
        return;
    }
    mostra_matriz(m, *quantidade);
    printf("\n");
    char msg1[] = "Escolha o primeiro conjunto: ";
    int conj1;
    read_valid_set(&conj1, *quantidade, msg1);
    char msg2[] = "Escolha o segundo conjunto: ";
    int conj2;
    read_valid_set(&conj2, *quantidade, msg2);
    set temp;
    temp.size = 0;
    for(int i = 0; i < m[conj1-1].size; i++){
        int value = m[conj1-1].arr[i];
        if(set_binary_search(value, m[conj2-1]) != -1){
            insere_no_conjunto(value, &temp);
        }
    }
    m[*quantidade] = temp;
    (*quantidade)++;
    if(conj1 != conj2) printf("Conjuntos %d e %d interseccionados com sucesso!\n", conj1, conj2);
    else printf("Conjunto %d interseccionado consigo mesmo com sucesso!\n", conj1);
    printf("\n");
    printf("Conjunto %d: ", *quantidade);
    mostra_conjunto(temp, 0, 0);
}

// mostra os detalhes dos elementos de um set
void mostra_conjunto_escolhido(set m[N], int quantidade){
    if(empty_of_sets(quantidade)) return;
    printf("Voce tem %d conjunto(s).\n", quantidade);
    printf("\n");
    char msg[] = "Escolha um conjunto para mostrar: ";
    int conj;
    read_valid_set(&conj, quantidade, msg);
    printf("Conjunto %d: ", conj);
    mostra_conjunto(m[conj-1], 0, 0);
    int size = m[conj-1].size;
    if(size == 0){
        return;
    }
    set impares;
    set pares;
    set positivos;
    set negativos;
    impares.size = 0;
    pares.size = 0;
    positivos.size = 0;
    negativos.size = 0;
    int zero = 0;
    long long soma = 0;
    int maior = m[conj-1].arr[0];
    int menor = m[conj-1].arr[0];
    for(int i = 0; i < size; i++){
        int value = m[conj-1].arr[i];
        soma += value;
        if(value & 1){
            insere_no_conjunto(value, &impares);
        } else {
            insere_no_conjunto(value, &pares);
        }
        if(value > 0){
            insere_no_conjunto(value, &positivos);
        } else if (value < 0){
            insere_no_conjunto(value, &negativos);
        } else {
            zero = 1;
        }
        maior = (value > maior ? value : maior);
        menor = (value < menor ? value : menor);
    }
    printf("\n");
    printf("O conjunto %d possui %d valores:\n", conj, size);
    printf("%d valor(es) pares: ", pares.size);
    mostra_conjunto(pares, 0, 0);
    printf("%d valor(es) impares: ", impares.size);
    mostra_conjunto(impares, 0, 0);
    printf("%d valor(es) negativos: ", negativos.size);
    mostra_conjunto(negativos, 0, 0);
    printf("%d valor(es) positivos: ", positivos.size);
    mostra_conjunto(positivos, 0, 0);
    if(zero) printf("O conjunto possui valor zero.\n");
    else printf("O conjunto nao possui valor zero.\n");
    printf("\n");
    printf("O maior valor do conjunto eh: %d\n", maior);
    printf("O menor valor do conjunto eh: %d\n", menor);
    printf("A soma dos valores do conjunto eh: %lld\n", soma);
}

// busca por um valor na matriz de sets
void busca_valor(set m[N], int quantidade){
    if (empty_of_sets(quantidade)) return;
    set conjuntos;
    conjuntos.size = 0;
    printf("Insira o valor a ser buscado: ");
    int value;
    scanf("%d", &value);
    printf("\n");
    for(int i = 0; i < quantidade; i++){
        if(set_binary_search(value, m[i]) != -1){
            insere_no_conjunto(i+1, &conjuntos);
        }
    }
    if(conjuntos.size == 0){
        printf("O valor %d nao esta em nenhum conjunto\n", value);
        return;
    }
    
    printf("O valor %d esta no(s) seguinte(s) conjunto(s):\n", value);
    for(int i = 0; i < conjuntos.size; i++){
        printf("Conjunto %d: ", conjuntos.arr[i]);
        mostra_conjunto(m[conjuntos.arr[i]-1], 1, value);
    }
}

// main
int main(){
    clear_screen;
    set matriz_conjuntos[N];
    int quantidade = 0;
    while(1){
        placeholder();
        mostra_menu();
        int opcao;
        printf("Digite uma opcao: ");
        scanf("%d", &opcao);
        placeholder();
        if(opcao == 1){
            clear_screen;
            placeholder();
            cria_conjunto(matriz_conjuntos, &quantidade);
        } else if (opcao == 2){
            clear_screen;
            placeholder();
            insere_dados(matriz_conjuntos, quantidade);
        } else if (opcao == 3){
            clear_screen;
            placeholder();
            remove_conjunto(matriz_conjuntos, &quantidade);
        } else if (opcao == 4){
            clear_screen;
            placeholder();
            uniao(matriz_conjuntos, &quantidade);
        } else if (opcao == 5){
            clear_screen;
            placeholder();
            interseccao(matriz_conjuntos, &quantidade);
        } else if (opcao == 6){
            clear_screen;
            placeholder();
            mostra_conjunto_escolhido(matriz_conjuntos, quantidade);
        } else if (opcao == 7){
            clear_screen;
            placeholder();
            mostra_matriz(matriz_conjuntos, quantidade);
        } else if (opcao == 8){
            clear_screen;
            placeholder();
            busca_valor(matriz_conjuntos, quantidade);
        } else if (opcao == 9){
            break;
        } else {
            clear_screen;
            placeholder();
            printf("Opcao invalida!\n");
        }
    }
    return 0;
}

// AUXILIARES

void placeholder(){ printf("\n-------------------------------------------\n\n"); }

void mostra_menu(){
    printf("Escolha uma opcao:\n\n");
    printf("(1) Criar novo conjunto.\n");
    printf("(2) Inserir dados em um conjunto.\n");
    printf("(3) Remover um conjunto.\n");
    printf("(4) Fazer uniao entre dois conjuntos.\n");
    printf("(5) Fazer interseccao entre dois conjuntos.\n");
    printf("(6) Mostrar um conjunto.\n");
    printf("(7) Mostrar todos conjuntos.\n");
    printf("(8) Buscar valor.\n");
    printf("(9) Sair do programa.\n\n");
}

void merge(int arr[], int l, int mid, int r){
    int neww[r-l+1];
    int idx = 0, i = l, j = mid+1;
    while( i <= mid && j <= r ) {
        if(arr[i] <= arr[j]){
            neww[idx] = arr[i];
            i++;
        } else {
            neww[idx] = arr[j];
            j++;
        }
        idx++;
    }
    while(i <= mid){
        neww[idx] = arr[i];
        idx++;
        i++;
    }
    while(j <= r){
        neww[idx] = arr[j];
        idx++;
        j++;
    }
    for(i = l, idx = 0; i <= r; i++, idx++){
        arr[i] = neww[idx];
    }
}

void merge_sort(int arr[], int l, int r){
    if( l < r ){
        int mid = l+(r-l)/2;
        merge_sort(arr, l, mid);
        merge_sort(arr, mid+1, r);
        merge(arr, l, mid, r);
    }
}

int binary_search(int v, int arr[], int l, int r){
    int mid;
    while(l <= r){
        mid = l + (r-l)/2;
        if(arr[mid] == v) return mid;
        else if(arr[mid] < v){
            l = mid+1;
        } else if(arr[mid] > v){
            r = mid-1;
        }
    }
    return -1;
}
int set_binary_search(int v, set st){ return binary_search(v, st.arr, 0, st.size-1); }

int empty_of_sets(int quantidade){
    if(quantidade <= 0){
        printf("Voce nao criou nenhum conjunto ainda.\n");
        return 1;
    }
    return 0;
}

int full_of_sets(int quantidade){
    if(quantidade >= N){
        printf("Voce ja criou muitos conjuntos!\n");
        return 1;
    }
    return 0;
}

int full_conj(int size){
    if(size >= M){
        printf("Conjunto cheio!\n");
        return 1;
    }
    return 0;
}
