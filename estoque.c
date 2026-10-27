#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_PRODUTOS 50
#define MAX_CATEGORIAS 20
#define ARQ_PRODUTOS "produtos.txt"
#define ARQ_CATEGORIAS "categorias.txt"

/* Struct da categoria */
struct Categoria {
    int codigo;
    char nome[30];
};

/* Struct do produto: guarda so o CODIGO da categoria, nao a categoria inteira */
struct Produto {
    int codigo;
    char nome[50];
    char prateleira;
    float preco;
    int quantidade;
    int disponivel;
    int categoria_codigo;
};

/* ==================== FUNCOES AUXILIARES ==================== */

/* Limpa o que sobrou no teclado (o \n do Enter) depois de um scanf */
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        /* so descarta os caracteres */
    }
}

/* Le um texto com espacos (fgets), sem deixar lixo no teclado */
void lerTexto(char destino[], int tamanho) {
    fgets(destino, tamanho, stdin);
    if (strchr(destino, '\n') == NULL) {
        limparBuffer(); /* o texto era maior que o vetor, joga fora o resto */
    }
    destino[strcspn(destino, "\n")] = '\0';
}

/* Confere se a string tem pelo menos um caractere que nao seja espaco */
int nomeValido(char texto[]) {
    int i;
    for (i = 0; texto[i] != '\0'; i++) {
        if (!isspace((unsigned char)texto[i])) {
            return 1;
        }
    }
    return 0;
}

/* ==================== BUSCAS ==================== */

/* Procura categoria pelo codigo. Devolve a posicao ou -1 */
int buscarCategoriaPorCodigo(struct Categoria lista[], int total, int codigo) {
    int i;
    for (i = 0; i < total; i++) {
        if (lista[i].codigo == codigo) {
            return i;
        }
    }
    return -1;
}

/* Procura produto pelo codigo. Devolve a posicao ou -1 */
int buscarPorCodigo(struct Produto lista[], int total, int codigo) {
    int i;
    for (i = 0; i < total; i++) {
        if (lista[i].codigo == codigo) {
            return i;
        }
    }
    return -1;
}

/* Procura produto pelo nome (strcmp). Devolve a posicao ou -1 */
int buscarPorNome(struct Produto lista[], int total, char nome[]) {
    int i;
    for (i = 0; i < total; i++) {
        if (strcmp(lista[i].nome, nome) == 0) {
            return i;
        }
    }
    return -1;
}

/* ==================== ARQUIVOS ==================== */

/* Salva as categorias: uma por linha, no formato codigo;nome */
void salvarCategorias(struct Categoria lista[], int total) {
    FILE *arq;
    int i;

    arq = fopen(ARQ_CATEGORIAS, "w");
    if (arq == NULL) {
        printf("Aviso: nao foi possivel salvar o arquivo %s\n", ARQ_CATEGORIAS);
        return;
    }
    for (i = 0; i < total; i++) {
        fprintf(arq, "%d;%s\n", lista[i].codigo, lista[i].nome);
    }
    fclose(arq);
}

/* Salva os produtos: um por linha, campos separados por ; */
void salvarProdutos(struct Produto lista[], int total) {
    FILE *arq;
    int i;

    arq = fopen(ARQ_PRODUTOS, "w");
    if (arq == NULL) {
        printf("Aviso: nao foi possivel salvar o arquivo %s\n", ARQ_PRODUTOS);
        return;
    }
    for (i = 0; i < total; i++) {
        fprintf(arq, "%d;%s;%c;%.2f;%d;%d;%d\n",
                lista[i].codigo, lista[i].nome, lista[i].prateleira,
                lista[i].preco, lista[i].quantidade, lista[i].disponivel,
                lista[i].categoria_codigo);
    }
    fclose(arq);
}

/* Carrega as categorias do arquivo. Se o arquivo nao existir, devolve 0 */
int carregarCategorias(struct Categoria lista[]) {
    FILE *arq;
    char linha[100];
    int total = 0;

    arq = fopen(ARQ_CATEGORIAS, "r");
    if (arq == NULL) {
        return 0; /* primeira vez: comeca com a lista vazia */
    }
    while (total < MAX_CATEGORIAS && fgets(linha, sizeof(linha), arq) != NULL) {
        if (sscanf(linha, "%d;%29[^\n]", &lista[total].codigo, lista[total].nome) == 2) {
            total++;
        }
    }
    fclose(arq);
    return total;
}

/* Carrega os produtos do arquivo. Se o arquivo nao existir, devolve 0 */
int carregarProdutos(struct Produto lista[]) {
    FILE *arq;
    char linha[200];
    int total = 0;

    arq = fopen(ARQ_PRODUTOS, "r");
    if (arq == NULL) {
        return 0;
    }
    while (total < MAX_PRODUTOS && fgets(linha, sizeof(linha), arq) != NULL) {
        if (sscanf(linha, "%d;%49[^;];%c;%f;%d;%d;%d",
                   &lista[total].codigo, lista[total].nome, &lista[total].prateleira,
                   &lista[total].preco, &lista[total].quantidade,
                   &lista[total].disponivel, &lista[total].categoria_codigo) == 7) {
            total++;
        }
    }
    fclose(arq);
    return total;
}

/* ==================== CATEGORIAS ==================== */

/* Mostra so as linhas "codigo - nome" das categorias */
void mostrarCategorias(struct Categoria lista[], int total) {
    int i;
    for (i = 0; i < total; i++) {
        printf("  %d - %s\n", lista[i].codigo, lista[i].nome);
    }
}

void listarCategorias(struct Categoria lista[], int total) {
    printf("\n=== LISTA DE CATEGORIAS ===\n");
    if (total == 0) {
        printf("Nenhuma categoria foi cadastrada.\n");
        return;
    }
    mostrarCategorias(lista, total);
}

/* Cadastra uma categoria e devolve o novo total */
int cadastrarCategoria(struct Categoria lista[], int total) {
    struct Categoria nova;

    printf("\n=== CADASTRAR CATEGORIA ===\n");

    if (total >= MAX_CATEGORIAS) {
        printf("Lista cheia! Nao e possivel cadastrar mais categorias (maximo %d).\n", MAX_CATEGORIAS);
        return total;
    }

    /* Codigo: positivo e nao repetido */
    do {
        printf("Codigo da categoria: ");
        if (scanf("%d", &nova.codigo) != 1) {
            nova.codigo = 0;
        }
        limparBuffer();

        if (nova.codigo <= 0) {
            printf("Codigo invalido! Digite um numero positivo.\n");
        } else if (buscarCategoriaPorCodigo(lista, total, nova.codigo) != -1) {
            printf("Ja existe uma categoria com esse codigo! Digite outro.\n");
            nova.codigo = 0;
        }
    } while (nova.codigo <= 0);

    /* Nome: nao pode ficar em branco (e sem ; para nao quebrar o arquivo) */
    do {
        printf("Nome da categoria: ");
        lerTexto(nova.nome, 30);

        if (!nomeValido(nova.nome)) {
            printf("O nome nao pode ficar em branco!\n");
        } else if (strchr(nova.nome, ';') != NULL) {
            printf("O nome nao pode ter ponto e virgula (;)!\n");
            nova.nome[0] = '\0';
        }
    } while (!nomeValido(nova.nome));

    lista[total] = nova;
    total = total + 1;

    printf("\n--- Categoria cadastrada com sucesso! ---\n");
    printf("Codigo: %d\n", nova.codigo);
    printf("Nome: %s\n", nova.nome);

    return total;
}

/* Submenu de categorias. Devolve o total de categorias atualizado */
int menuCategorias(struct Categoria lista[], int total) {
    int opcao;

    do {
        printf("\n----- CATEGORIAS -----\n");
        printf("1 - Cadastrar Categoria\n");
        printf("2 - Listar Categorias\n");
        printf("3 - Voltar\n");
        printf("Escolha uma opcao: ");

        if (scanf("%d", &opcao) != 1) {
            opcao = 0;
        }
        limparBuffer();

        switch (opcao) {
            case 1:
                total = cadastrarCategoria(lista, total);
                break;
            case 2:
                listarCategorias(lista, total);
                break;
            case 3:
                break;
            default:
                printf("Opcao invalida! Tente novamente.\n");
        }
    } while (opcao != 3);

    return total;
}

/* ==================== PRODUTOS ==================== */

/* Mostra um produto. Recebe tambem a lista de categorias para achar o NOME */
void exibirProduto(struct Produto p, struct Categoria cats[], int totalCats) {
    int posCat;

    printf("Codigo: %d\n", p.codigo);
    printf("Nome: %s\n", p.nome);
    printf("Prateleira: %c\n", p.prateleira);
    printf("Preco unitario: R$ %.2f\n", p.preco);
    printf("Quantidade em estoque: %d\n", p.quantidade);

    posCat = buscarCategoriaPorCodigo(cats, totalCats, p.categoria_codigo);
    if (posCat != -1) {
        printf("Categoria: %s\n", cats[posCat].nome);
    } else {
        printf("Categoria: desconhecida\n");
    }

    printf("Disponivel para venda: %d", p.disponivel);
    if (p.disponivel == 1) {
        printf(" (Sim)\n");
    } else {
        printf(" (Nao)\n");
    }
}

/* Cadastra um produto e devolve o novo total */
int cadastrarProduto(struct Produto lista[], int total, struct Categoria cats[], int totalCats) {
    struct Produto novo;
    int posCat;

    printf("\n=== CADASTRAR PRODUTO ===\n");

    if (total >= MAX_PRODUTOS) {
        printf("Lista cheia! Nao e possivel cadastrar mais produtos (maximo %d).\n", MAX_PRODUTOS);
        return total;
    }

    /* Sem categoria nao tem como cadastrar produto */
    if (totalCats == 0) {
        printf("Nenhuma categoria cadastrada! Cadastre uma categoria antes (menu 4).\n");
        return total;
    }

    /* ---------- CODIGO (positivo e nao repetido) ---------- */
    do {
        printf("Codigo: ");
        if (scanf("%d", &novo.codigo) != 1) {
            novo.codigo = 0;
        }
        limparBuffer();

        if (novo.codigo <= 0) {
            printf("Codigo invalido! Digite um numero positivo.\n");
        } else if (buscarPorCodigo(lista, total, novo.codigo) != -1) {
            printf("Ja existe um produto com esse codigo! Digite outro.\n");
            novo.codigo = 0;
        }
    } while (novo.codigo <= 0);

    /* ---------- NOME ---------- */
    do {
        printf("Nome: ");
        lerTexto(novo.nome, 50);

        if (!nomeValido(novo.nome)) {
            printf("O nome nao pode ficar em branco!\n");
        } else if (strchr(novo.nome, ';') != NULL) {
            printf("O nome nao pode ter ponto e virgula (;)!\n");
            novo.nome[0] = '\0';
        }
    } while (!nomeValido(novo.nome));

    /* ---------- PRATELEIRA ---------- */
    do {
        printf("Prateleira (A, B, C ou D): ");
        scanf(" %c", &novo.prateleira);
        limparBuffer();
        novo.prateleira = toupper(novo.prateleira);

        if (novo.prateleira != 'A' && novo.prateleira != 'B' &&
            novo.prateleira != 'C' && novo.prateleira != 'D') {
            printf("Prateleira invalida! Use apenas A, B, C ou D.\n");
        }
    } while (novo.prateleira != 'A' && novo.prateleira != 'B' &&
             novo.prateleira != 'C' && novo.prateleira != 'D');

    /* ---------- PRECO ---------- */
    do {
        printf("Preco unitario: ");
        if (scanf("%f", &novo.preco) != 1) {
            novo.preco = 0;
        }
        limparBuffer();

        if (novo.preco <= 0) {
            printf("Preco invalido! Tem que ser maior que zero.\n");
        }
    } while (novo.preco <= 0);

    /* ---------- QUANTIDADE ---------- */
    do {
        printf("Quantidade em estoque: ");
        if (scanf("%d", &novo.quantidade) != 1) {
            novo.quantidade = -1;
        }
        limparBuffer();

        if (novo.quantidade < 0) {
            printf("Quantidade invalida! Nao pode ser negativa.\n");
        }
    } while (novo.quantidade < 0);

    /* ---------- CATEGORIA (precisa existir na lista de categorias) ---------- */
    printf("\nCategorias cadastradas:\n");
    mostrarCategorias(cats, totalCats);
    do {
        printf("Codigo da categoria: ");
        if (scanf("%d", &novo.categoria_codigo) != 1) {
            novo.categoria_codigo = 0;
        }
        limparBuffer();

        posCat = buscarCategoriaPorCodigo(cats, totalCats, novo.categoria_codigo);
        if (posCat == -1) {
            printf("Categoria inexistente! Escolha um codigo da lista.\n");
        }
    } while (posCat == -1);

    /* ---------- DISPONIVEL (calculado pelo sistema) ---------- */
    if (novo.quantidade > 0) {
        novo.disponivel = 1;
    } else {
        novo.disponivel = 0;
    }

    lista[total] = novo;
    total = total + 1;

    printf("\n--- Produto cadastrado com sucesso! ---\n");
    exibirProduto(lista[total - 1], cats, totalCats);

    return total;
}

void listarProdutos(struct Produto lista[], int total, struct Categoria cats[], int totalCats) {
    int i;

    printf("\n=== LISTA DE PRODUTOS ===\n");

    if (total == 0) {
        printf("Nenhum produto foi cadastrado.\n");
        return;
    }

    for (i = 0; i < total; i++) {
        printf("\n--- Produto %d de %d ---\n", i + 1, total);
        exibirProduto(lista[i], cats, totalCats);
    }
}

void pesquisarProduto(struct Produto lista[], int total, struct Categoria cats[], int totalCats) {
    int tipo;
    int codigo;
    char nome[50];
    int posicao;

    printf("\n=== PESQUISAR PRODUTO ===\n");
    printf("1 - Por codigo\n");
    printf("2 - Por nome\n");
    printf("Escolha: ");
    if (scanf("%d", &tipo) != 1) {
        tipo = 0;
    }
    limparBuffer();

    if (tipo == 1) {
        printf("Digite o codigo: ");
        if (scanf("%d", &codigo) != 1) {
            codigo = -1;
        }
        limparBuffer();
        posicao = buscarPorCodigo(lista, total, codigo);
    } else if (tipo == 2) {
        printf("Digite o nome: ");
        lerTexto(nome, 50);
        posicao = buscarPorNome(lista, total, nome);
    } else {
        printf("Opcao de pesquisa invalida!\n");
        return;
    }

    if (posicao == -1) {
        printf("Nenhum produto foi encontrado.\n");
    } else {
        printf("\n--- Produto encontrado ---\n");
        exibirProduto(lista[posicao], cats, totalCats);
    }
}

/* ==================== MAIN ==================== */

int main() {
    struct Produto produtos[MAX_PRODUTOS];
    struct Categoria categorias[MAX_CATEGORIAS];
    int totalProdutos;
    int totalCategorias;
    int opcao;

    /* Ao iniciar, carrega os dados dos arquivos (se existirem) */
    totalCategorias = carregarCategorias(categorias);
    totalProdutos = carregarProdutos(produtos);

    do {
        printf("\n===== SISTEMA DE ESTOQUE =====\n");
        printf("1 - Cadastrar Produto\n");
        printf("2 - Listar Produtos\n");
        printf("3 - Pesquisar Produto\n");
        printf("4 - Categorias\n");
        printf("5 - Sair\n");
        printf("Escolha uma opcao: ");

        if (scanf("%d", &opcao) != 1) {
            opcao = 0;
        }
        limparBuffer();

        switch (opcao) {
            case 1:
                totalProdutos = cadastrarProduto(produtos, totalProdutos, categorias, totalCategorias);
                salvarProdutos(produtos, totalProdutos); /* salva a cada cadastro */
                break;
            case 2:
                listarProdutos(produtos, totalProdutos, categorias, totalCategorias);
                break;
            case 3:
                pesquisarProduto(produtos, totalProdutos, categorias, totalCategorias);
                break;
            case 4:
                totalCategorias = menuCategorias(categorias, totalCategorias);
                salvarCategorias(categorias, totalCategorias);
                break;
            case 5:
                /* Ao sair, salva as duas listas de novo */
                salvarCategorias(categorias, totalCategorias);
                salvarProdutos(produtos, totalProdutos);
                printf("Dados salvos. Saindo do sistema... Ate mais!\n");
                break;
            default:
                printf("Opcao invalida! Tente novamente.\n");
        }
    } while (opcao != 5);

    return 0;
}
