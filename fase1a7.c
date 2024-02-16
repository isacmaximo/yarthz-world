//bibliotecas
#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>

//linhas e colunas da matriz (para o enredo)
#define LINHA 40
#define COLUNA 18

//cores das letras
#define VERMELHO    "\x1b[31m"
#define VERDE   "\x1b[32m"
#define AZUL    "\x1b[34m"
#define AMARELO "\x1b[33m"
#define CIANO   "\x1b[36m"
#define MAGENTA "\x1b[35m"
#define BRANCO  "\x1b[37m"
#define CINZA_C  "\x1b[90m"
#define VERMELHO_C   "\x1b[91m"
#define VERDE_C    "\x1b[92m"
#define AMARELO_C "\x1b[93m"
#define CIANO_C   "\x1b[94m"
#define MAGENTA_C "\x1b[95m"
#define BRANCO_C  "\x1b[97m"
#define RESET   "\x1b[0m"

#define F_AZUL "\x1b[44m"
#define F_VERMELHO "\x1b[41m"
#define F_AMARELO "\x1b[43m"
#define F_VERDE "\x1b[42m"
#define F_MAGENTA "\x1b[45m"
#define F_PRETO "\x1b[40m"
#define F_CINZA "\x1b[100m"
#define TESTE "\x1b[43;31m"
#define FUNDO_R "\xb1[49m"

//tipos de ataque e defesa
#define PABA "paba"
#define PABD "pabd"
#define PDBA "pdba"
#define PDBD "pdbd"
#define PABAE "pabae"
#define PDBAE "pdbae"
#define PAEBA "paeba"
#define PABAE1 "pabae1"
#define PABAE2 "pabae2"
#define PDBAE1 "pdbae1"
#define PDBAE2 "pdbae2"
#define PAEBD "paebd"
#define PAEBAE1 "paebae1"
#define PAEBAE2 "paebae2"




struct personagem{ //struct do personagem e do boss
    int vida;
    int ataque;
    int defesa;
    int envenenamento;
    int especial;
    int sair;
    char *nome;
}personagem, boss;

int contador_fases; //contará as fases


 //ESTRUTURA DA BAGAGEM
 // a seleção de um ítem vai ser de acordo com a posição do nó  
typedef struct bagagem{
    char *nome_item;   //lista vai ter um nome para o ítem
    int qtd_item;   //lista vai ter uma quandidade para esse ítem
    struct bagagem *proximo;  //ponteiro próximo
    struct bagagem *anterior; //ponteiro anterior
}Bagagem;  //tipo bagadem

Bagagem *inicio = NULL; //ponteiro início começa no NULL, ou seja lista vazia
Bagagem *fim = NULL; // ponteiro fim começa no NULL, ou seja lista vazia
int tamanho = 0; //tamanho da lista (bagagem) começa com zero


//FUNÇÂO ADCIONAR NA LISTA
void adcionar(char *nome_item, int qtd_item, int pos){ //parametros do nome do ítem e a quantidade do ítem, e a posição

    if (pos >=0  && pos <= tamanho){ //viabilidade da lista (não pode ter "buracos")

        //alocação de memória da Bagagem (No)
        Bagagem *novo = malloc(sizeof(Bagagem)); //um ponteiro novo que vai ser utilizado para adcionar mais Nos na Bagagem
        novo->nome_item = nome_item;
        novo->qtd_item = qtd_item;
        //lista começa vazia
        novo->proximo = NULL;
        novo->anterior = NULL;

        if (inicio == NULL){ // se a lista estiver vazia, então o primeiro ítem adcioanado (adcionar no ínicio)
            inicio = novo;
            fim = novo;
            tamanho++;
        }
        else if(pos == 0){ //se tiver mais de um ítem
            novo->proximo = inicio;
            inicio->anterior = novo;
            inicio = novo;
            tamanho++;
        }
        else if (pos == tamanho){ //adcionar no fim
            fim->proximo = novo;
            novo->anterior = fim;
            fim = novo;
        }
        else{ //adcionar no meio
            Bagagem *auxiliar = inicio;
            int i;
            for (i = 0; i < pos; i++){
               auxiliar = auxiliar->proximo;
            }
            novo->proximo = auxiliar;
            novo->anterior = auxiliar->anterior;
            auxiliar->anterior->proximo = novo;
            novo->proximo->anterior = novo;
        }
    }
}



void adc_qtd(char *nome_item1){ //adcionar a quantidade de um ítem
    Bagagem *auxiliar = inicio;
    while (auxiliar != NULL){
       
        if(auxiliar->nome_item == nome_item1){ //se o ítem for encontrado  
            auxiliar->qtd_item = auxiliar->qtd_item + 1; //sua quantidade aumenta + 1
            break;
        }
        auxiliar = auxiliar->proximo;


    }
   
    
}



void rmv_qtd(char *nome_item){  //função de remover quantidade de um ítem
    Bagagem *auxiliar = inicio;

    while (auxiliar->nome_item != nome_item){
        auxiliar = auxiliar->proximo;
    }
    auxiliar->qtd_item = auxiliar->qtd_item - 1;
}


void encontrar(int opct){ //função consome um ítem da bagagem e não mostra se o ítem tiver uma quantidade igual a zero
    Bagagem *auxiliar = inicio;
    int i;
    for(i = 0; i < opct - 1; i++){ //vai percorrer conforme a posição do ítem escolhido
        auxiliar = auxiliar->proximo; //auxiliar vai sempre se deslocando na lista
    }
    if (auxiliar != NULL){ //se auxiliar for diferente de null
        if (auxiliar->qtd_item > 0){ // se a quantidade desse determinado ítem for maior que zero (usável)
            if (strcmp(auxiliar->nome_item,"(+) Porção") == 0){ //se o ítem for a porção
                rmv_qtd("(+) Porção"); //remove 1 unidade da porção
                printf("\n1 (+) Porção usada, a vida do personagem aumenta + 10\n"); //informação ao usuário
                usleep(3000000); //tempo de espera para o usuário ver a mensagem
                system("cls"); //limpar a tela
                personagem.vida = personagem.vida + 10; //a vida do personagem aumenta 10 pontos

            }
            else if (strcmp(auxiliar->nome_item,"(^) Antídoto") == 0){ //se o ítem for o antídoto
                rmv_qtd("(^) Antídoto"); // remove 1 unidade do antídoto
                printf("\n1 (^) Antídoto usado, o personagem não está mais envenenado\n"); //informação ao usuário
                usleep(3000000); //tempo de espera para o usuário ver a mensagem
                system("cls"); //limpar tela
                if (personagem.envenenamento == 1){
                    personagem.envenenamento = personagem.envenenamento - 1; //torna o estatus de envenenamento para zero
                }
                else if (personagem.envenenamento == 0){
                    personagem.envenenamento = 0; //torna o estatus de envenenamento para zero
                }

            }
            else if(strcmp(auxiliar->nome_item,"(º) Bracelete Imperial") == 0){
                rmv_qtd("(º) Bracelete Imperial");
                printf("\n1 Bracelete Imperial usado, seu ataque e defesa aumentaram temporariamente\n");
                usleep(3000000);
                system("cls");
                personagem.ataque = personagem.ataque + 1;
                personagem.defesa = personagem.defesa + 1;
            }
            else if (strcmp(auxiliar->nome_item,"(§) Poder Ancestral") == 0){
                rmv_qtd("(§) Poder Ancestral");
                printf("\n O poder ancestral foi usado, o dano é maior com esse ataque");
                usleep(3000000);
                system("cls");
                personagem.especial = personagem.especial - 1;

            }
        }
    }
    else{
        printf("\nNão existe esse ítem\n"); //se o ítem não se encontrar na lista
        usleep(3000000);
        system("cls");
    }
}



void imprimir(){   //FUNÇÃO IMPRIMIR ITENS
    Bagagem *auxiliar = inicio; // um ponteiro auxiliar começa do início;
    if (auxiliar == NULL){
        printf(" "); // se não tiver nada na lista, mostra nada (bagagem)
    }

    else{ //se os ítens tiverem uma quantidade > 1, então esse ítem é mostrado
        for(int i = 0; i < tamanho; i++){
            if (strcmp(auxiliar->nome_item,"(+) Porção") == 0 && auxiliar->qtd_item > 0){ //se a porção tiver uma quantidade maior que zero
                printf("\n %d - %s  x%d\n",i+1, auxiliar->nome_item, auxiliar->qtd_item); //mostrará a posição e quantidade (bagagem)
            }
            else if (strcmp(auxiliar->nome_item,"(^) Antídoto") == 0 && auxiliar->qtd_item > 0){ //se o antídoto tiver uma quantidade maior que zero
                printf("\n %d - %s  x%d\n",i+1, auxiliar->nome_item, auxiliar->qtd_item); // mostrará a posição e quantidade (bagagem)
            }
            else if(strcmp(auxiliar->nome_item,"(º) Bracelete Imperial") == 0 && auxiliar->qtd_item > 0){
                printf("\n %d - %s  x%d\n",i+1, auxiliar->nome_item, auxiliar->qtd_item); // mostrará a posição e quantidade (bagagem)
            }
            else if (strcmp(auxiliar->nome_item,"(§) Poder Ancestral") == 0 && auxiliar->qtd_item > 0){
                printf("\n %d - %s x%d\n", i+1, auxiliar->nome_item, auxiliar->qtd_item);
            }
            else if (strcmp(auxiliar->nome_item,"(+) Porção") == 0 && auxiliar->qtd_item == 0){ //se a quantidade da porção for igual a zero
                printf(" "); //não mostrará esse ítem
            }
            else if (strcmp(auxiliar->nome_item,"(^) Antídoto") == 0 && auxiliar->qtd_item == 0){ // se a quantidade do antídoto  for igual a zero
                printf(" "); //não mostrará esse ítem
            }
            else if(strcmp(auxiliar->nome_item,"(º) Bracelete Imperial") == 0 && auxiliar->qtd_item == 0){
                printf(" "); //não mostrará esse ítem
            }
            else if (strcmp(auxiliar->nome_item,"(§) Poder Ancestral") == 0 && auxiliar->qtd_item == 0){
                printf(" ");
            }
            
            auxiliar = auxiliar->proximo;
        }
    }
}



void escolher_nome(char *nome){ //funçãode escolher o nome do personagem

    printf("\n===> Até 10 caracteres (10 Letras) <===\n");
    printf("Digite seu nome: \n");
    
    fgets(nome, sizeof(nome), stdin); //recebe o nome
    int tamanho_nome = strlen(nome); //mede o tamanho do nome

    
    if (tamanho_nome == 0){ //se não tiver nenhum caractere
        printf("\n Só é possível ter um nome de 1 a 10 caracteres! Tente novamente!");
        system("cls");
        escolher_nome(nome);
    }

    if (tamanho_nome > 10){ //se passar de 10
        printf("\n Só é possível ter um nome de 1 a 10 caracteres! Tente novamente!");
        system("cls");
        escolher_nome(nome);
    }

    else if (tamanho_nome > 0 && tamanho_nome <= 10){ //se for de 1 a 10 o tamanho
        char escolha;
        char confirmar;
        while(confirmar != 'o'){
            printf("\nDeseja realmente continuar com esse nome?");
            printf("\n1 - SIM");
            printf("\n2 - NÃO");
            printf("\nDigite sua opção: ");
            escolha = getchar(); 
            fflush(stdin);

            if (isdigit(escolha) != 0){
                if(escolha == '1'){
                    printf("\nNome salvo com sucesso!");
                    personagem.nome = nome;
                    system("cls");
                    printf("\nSeja bem vindo ao mundo de Yarthz %s", personagem.nome);
                    fflush(stdout); //stdout sempre vem depois!
                    sleep(4);
                    system("cls");
                    confirmar = 'o';
                    
                }
                else if (escolha == '2'){ //se escolher não continuar com o nome, a mesma função de escolher nome é chamada de forma recursiva 
                    system("cls");
                    escolher_nome(nome);
                    confirmar = 'o';
                }
                else{
                    system("cls");
                    void invalido();
                }

            }
            else{
                system("cls");
                void invalido();
            }
            
        }

    }
    
}


void menu_fase(int fase){ //função que contém o menu das fases

    char nome_personagem[10];

    int j = 0;
    for (int i = 0; personagem.nome[i] != '\0'; i++) {
        if (personagem.nome[i] != ' ' && personagem.nome[i] != '\n') {
            nome_personagem[j++] = personagem.nome[i];
        }
    }
    nome_personagem[j] = '\0';
    
    //condições de nome
    printf("╔═══════════════════════════════╦════════════════════════════════╗\n");
    printf("║  %-29s║  %-30s║\n", nome_personagem, "Boss");
    printf("╠═══════════════════════════════╬════════════════════════════════╣\n");
    printf("║                               ║                                ║\n");
    //condições da vida
    if (personagem.vida > 9 && boss.vida > 9)
    printf("║  ♥ vida: %d                   ║  ♥ vida: %d                    ║\n", personagem.vida, boss.vida);
    else if(personagem.vida <= 9 && boss.vida > 9)
    printf("║  ♥ vida: 0%d                   ║  ♥ vida: %d                    ║\n", personagem.vida, boss.vida);
    else if(personagem.vida > 9 && boss.vida <= 9)
    printf("║  ♥ vida: %d                   ║  ♥ vida: 0%d                    ║\n", personagem.vida, boss.vida);
    else if(personagem.vida <= 9 && boss.vida <= 9)
    printf("║  ♥ vida: 0%d                   ║  ♥ vida: 0%d                    ║\n", personagem.vida, boss.vida);
    //condições de ataque
    printf("║  ► ataque: %d                 ║  ► ataque: %d                  ║\n", personagem.ataque, boss.ataque);
    //condições de defesa
    if (boss.defesa <= 9 && personagem.defesa <= 9)
    printf("║  ◄ defesa: 0%d                 ║  ◄ defesa: 0%d                  ║\n", personagem.defesa, boss.defesa);
    else if (boss.defesa <= 9 && personagem.defesa > 9)
    printf("║  ◄ defesa: %d                 ║  ◄ defesa: 0%d                  ║\n", personagem.defesa, boss.defesa);
    else if (boss.defesa > 9 && personagem.defesa <= 9)
    printf("║  ◄ defesa: 0%d                 ║  ◄ defesa: %d                  ║\n", personagem.defesa, boss.defesa);
    else if (boss.defesa > 9 && personagem.defesa > 9)
    printf("║  ◄ defesa: %d                 ║  ◄ defesa: %d                  ║\n", personagem.defesa, boss.defesa);
    //condiçẽs de tipo e status
    //status ok
    if (fase == 1 && personagem.envenenamento == 0){
        printf("║  ○ tipo: Humano               ║  ○ tipo: Besta                 ║\n");
        printf("║  ◙ status: ok                 ║  ▲ nome: Nayliruki             ║\n");
    }
    else if (fase == 2 && personagem.envenenamento == 0){
        printf("║  ○ tipo: Humano               ║  ○ tipo: Besta Real            ║\n");
        printf("║  ◙ status: ok                 ║  ▲ nome: Rengoruki             ║\n");
    }
    else if (fase == 3 && personagem.envenenamento == 0){
        printf("║  ○ tipo: Humano               ║  ○ tipo: Mutante               ║\n");
        printf("║  ◙ status: ok                 ║  ▲ nome: Throporyot            ║\n");
    }
    else if (fase == 4 && personagem.envenenamento == 0){
        printf("║  ○ tipo: Humano               ║  ○ tipo: Aço                   ║\n");
        printf("║  ◙ status: ok                 ║  ▲ nome: Ballador              ║\n");
    }
    else if (fase == 5 && personagem.envenenamento == 0){
        printf("║  ○ tipo: Humano               ║  ○ tipo: Divino                ║\n");
        printf("║  ◙ status: ok                 ║  ▲ nome: Asmund                ║\n");
    }
    else if (fase == 6 && personagem.envenenamento == 0){
        printf("║  ○ tipo: Humano               ║  ○ tipo: Cristal               ║\n");
        printf("║  ◙ status: ok                 ║  ▲ nome: Quartstoke            ║\n");
    }
    else if (fase == 7 && personagem.envenenamento == 0){
        printf("║  ○ tipo: Humano               ║  ○ tipo: Corrompido            ║\n");
        printf("║  ◙ status: ok                 ║  ▲ nome: Amaymom               ║\n");
    }
    //status envenenado
    else if (fase == 3 && personagem.envenenamento == 1){
        printf("║  ○ tipo: Humano               ║  ○ tipo: Mutante               ║\n");
        printf("║  ◙ status: envenenado         ║  ▲ nome: Throporyot            ║\n");
    }
    else if (fase == 5 && personagem.envenenamento == 1){
        printf("║  ○ tipo: Humano               ║  ○ tipo: Divino                ║\n");
        printf("║  ◙ status: envenenado         ║  ▲ nome: Asmund                ║\n");
        
    }
    else if (fase == 7 && personagem.envenenamento == 1){
        printf("║  ○ tipo: Humano               ║  ○ tipo: Amaymom               ║\n");
        printf("║  ◙ status: envenenado         ║  ▲ nome: Nayliruki             ║\n");
    }
    //final não muda
    printf("║                               ║                                ║\n");
    printf("╠════════════════════╦══════════╩══════════╦═════════════════════╣\n");
    printf("║     1- ATAQUE      ║     2 - DEFESA      ║     3- MOCHILA      ║\n");
    printf("╚════════════════════╩═════════════════════╩═════════════════════╝\n");
    //menu de opções antes da fase 6
    if (fase == 1 || fase == 2 || fase == 3 || fase == 4 || fase == 5){
    printf("╔══════════════════════════════════════════╦═════════════════════╗\n");
    printf("║                 BLOQUEADO                ║  4 - SAIR DO JOGO   ║\n"); 
    printf("╚══════════════════════════════════════════╩═════════════════════╝\n");
    }
    //ataque especial (fases 6 e 7)
    else if(fase == 6 || fase == 7){
    printf("╔══════════════════════════════════════════╦═════════════════════╗\n");
    printf("║           5 - ATAQUE ESPECIAL            ║  4 - SAIR DO JOGO   ║\n"); 
    printf("╚══════════════════════════════════════════╩═════════════════════╝\n");
    }
}

void invalido(){ //função  que mostra um alerta de opção inválida
    printf("\nEntrada inválida, tente novamente!\n");
    usleep(2000000);
    system("cls");
}


// Função que lê arquivos
void ler_arq(char *nomearq) { // Recebe o nome do arquivo como um ponteiro para caractere
    char linha[100];  // Linha tem até 100 caracteres
    FILE *arq = fopen(nomearq, "r");  // Abre o arquivo em modo de leitura
    while (fgets(linha, sizeof(linha), arq) != NULL) { // Enquanto houver conteúdo no arquivo
        printf("%s", linha); // Imprime as strings (conteúdo do arquivo)
    }
    fclose(arq); // Fecha o arquivo
}


//função que lê arquivos com cor
void ler_arq_cor(char *nomearq, char cor[]){   //recebe o nome do arquivo, e a cor
    char linha[100];  //linha tem até 100 caracteres
    FILE * arq = fopen(nomearq, "r");  //topo arquivo tem um ponteiro que vai ser aberto em modo leitura
    while(fgets(linha, sizeof(linha), arq) != NULL){ //enquanto tiver conteúdo no arquivo
        printf("%s%s%s",cor, linha, RESET); //printará as strings (conteúdo do arquivo)
    }
    fclose(arq); //fecha o arquivo
}

//função que lê arquivos e substitui o símbolo pelo nome do personagem
void substitui_nome(char *nomearq){   //recebe o nome do arquivo
    char linha[150];  //linha tem até 100 caracteres
    char *separador; //ponteiro para separa o txt em sub strings
    char substituto[] = "@#$=&*~!";
    FILE * arq = fopen(nomearq, "r");  //topo arquivo tem um ponteiro que vai ser aberto em modo leitura

    while(fgets(linha, sizeof(linha), arq) != NULL){ //enquanto tiver conteúdo no arquivo
        
        separador = strtok(linha, " "); // as sub strins serão separadas pelo caractere espaço
        while(separador != NULL){ //enquanto for diferente de nulo
            if (strstr(separador, substituto) != 0){  //aqui mudará pelo nome doi personagem
                separador = personagem.nome;
            }
            printf(" %s", separador); //cada sub string do arquivo será exibido
            fflush(stdout);
            usleep(300000);
            separador = strtok(NULL, " ");
        }
    }
    fclose(arq); //fecha o arquivo
}


void espera_padrao(){ //espera padrão para frames das animações do jogo
    usleep(200000);
    system("cls");
}

void matriz_enredo(char *mat[6][18]){ //matriz que gerará o enrendo (história) em aspecto digitado
    int c, l;
    for(l = 0; l < 6; l++){
        for(c = 0; c < 18; c++){
            printf("%s", mat[l][c]);
            fflush(stdout);
            usleep(300000);
        }
        puts("");
    }
}

void atk_def(char *tipo){ //função que contém todos as configurações de ataques e defesas
    int dano_personagem = 0;
    int dano_boss;
    
    if (strcmp(tipo, PABA) == 0){
        dano_personagem = personagem.ataque - boss.defesa;
        if (dano_personagem > 0)
            boss.vida = (boss.vida - 1) - dano_personagem;
        else if (dano_personagem < 0)
            boss.vida = boss.vida + dano_personagem;
        
        dano_boss = boss.ataque - personagem.defesa;
        if (dano_boss > 0)
            personagem.vida = (personagem.vida - 1) - dano_personagem;
        else if (dano_boss < 0)
            personagem.vida = personagem.vida + dano_boss;
    }

    else if(strcmp(tipo,PABD) == 0){
        dano_personagem = personagem.ataque - (boss.defesa + 1);
        if(dano_personagem > 0)
            boss.vida = (boss.vida - 1) - dano_personagem;
        else if(dano_personagem < 0)
            boss.vida = boss.vida + dano_personagem;
    }

    else if(strcmp(tipo,PDBA) == 0){
        dano_boss = boss.ataque - (personagem.defesa + 1);
        if(dano_boss > 0)
            personagem.vida = (personagem.vida - 1) - dano_boss;
        else if(dano_boss < 0)
            personagem.vida = personagem.vida + dano_boss; 
    }

    else if (strcmp(tipo,PDBD) == 0){
        personagem.vida = personagem.vida + 0;
        boss.vida = boss.vida + 0;
    }

    else if (strcmp(tipo,PABAE) == 0){
        dano_personagem = personagem.ataque - boss.defesa;
        if (dano_personagem > 0)
            boss.vida = (boss.vida - 1) - dano_personagem;
        else if (dano_personagem < 0)
            boss.vida = boss.vida + dano_personagem;
        
        dano_boss = (boss.ataque + 2) - personagem.defesa;
        if (dano_boss > 0)
            personagem.vida = (personagem.vida - 1) - dano_personagem;
        else if (dano_boss < 0)
            personagem.vida = personagem.vida + dano_boss;
    }

    else if (strcmp(tipo,PDBAE) == 0){
        dano_boss = (boss.ataque + 2) - (personagem.defesa + 1);
        if(dano_boss > 0)
            personagem.vida = (personagem.vida - 1) - dano_boss;
        else if(dano_boss < 0)
            personagem.vida = personagem.vida + dano_boss;
    }

    else if (strcmp(tipo,PAEBA) == 0){
        dano_personagem = (personagem.ataque + 2) - boss.defesa;
        if (dano_personagem > 0)
            boss.vida = (boss.vida - 1) - dano_personagem;
        else if (dano_personagem < 0)
            boss.vida = boss.vida + dano_personagem; 

        dano_boss = boss.ataque - personagem.defesa;
        if (dano_boss > 0)
            personagem.vida = (personagem.vida - 1) - dano_personagem;
        else if (dano_boss < 0)
            personagem.vida = personagem.vida + dano_boss;  
    }

    else if (strcmp(tipo,PAEBD) == 0){
        dano_personagem = (personagem.ataque + 2) - (boss.defesa + 1);
        if (dano_personagem > 0)
            boss.vida = (boss.vida - 1) - dano_personagem;
        else if (dano_personagem < 0)
            boss.vida = boss.vida + dano_personagem;
    }

    else if (strcmp(tipo,PAEBAE1) == 0){
        dano_personagem = (personagem.ataque + 2) - boss.defesa;
        if (dano_personagem > 0)
            boss.vida = (boss.vida - 1) - dano_personagem;
        else if (dano_personagem < 0)
            boss.vida = boss.vida + dano_personagem;

        dano_boss = (boss.ataque + 2) - personagem.defesa;
        if (dano_boss > 0)
            personagem.vida = (personagem.vida - 1) - dano_personagem;
        else if (dano_boss < 0)
            personagem.vida = personagem.vida + dano_boss;
    }

    else if (strcmp(tipo,PAEBAE2) == 0){
        dano_personagem = (personagem.ataque + 2) - boss.defesa;
        if (dano_personagem > 0)
            boss.vida = (boss.vida - 1) - dano_personagem;
        else if (dano_personagem < 0)
            boss.vida = boss.vida + dano_personagem;

        dano_boss = (boss.ataque + 2) - personagem.defesa;
        if (dano_boss > 0)
            personagem.vida = (personagem.vida - 1) - dano_personagem;
        else if (dano_boss < 0)
            personagem.vida = personagem.vida + dano_boss;
    }

    else if (strcmp(tipo,PABAE1) == 0){
        dano_personagem = personagem.ataque - boss.defesa;
        if (dano_personagem > 0)
            boss.vida = (boss.vida - 1) - dano_personagem;
        else if (dano_personagem < 0)
            boss.vida = boss.vida + dano_personagem;
        
        dano_boss = (boss.ataque + 2) - personagem.defesa;
        if (dano_boss > 0)
            personagem.vida = (personagem.vida - 1) - dano_personagem;
        else if (dano_boss < 0)
            personagem.vida = personagem.vida + dano_boss;
    }

    else if (strcmp(tipo,PABAE2) == 0){
        dano_personagem = personagem.ataque - boss.defesa;
        if (dano_personagem > 0)
            boss.vida = (boss.vida - 1) - dano_personagem;
        else if (dano_personagem < 0)
            boss.vida = boss.vida + dano_personagem;
        
        dano_boss = (boss.ataque + 2) - personagem.defesa;
        if (dano_boss > 0)
            personagem.vida = (personagem.vida - 1) - dano_personagem;
        else if (dano_boss < 0)
            personagem.vida = personagem.vida + dano_boss;
    }

    else if (strcmp(tipo,PDBAE1) == 0){
        dano_boss = (boss.ataque + 2) - (personagem.defesa + 1);
        if (dano_boss > 0)
            personagem.vida = (personagem.vida - 1) - dano_personagem;
        else if (dano_boss < 0)
            personagem.vida = personagem.vida + dano_boss;
    }

    else if (strcmp(tipo,PDBAE2) == 0){
        dano_boss = (boss.ataque + 2) - (personagem.defesa + 1);
        if (dano_boss > 0)
            personagem.vida = (personagem.vida - 1) - dano_personagem;
        else if (dano_boss < 0)
            personagem.vida = personagem.vida + dano_boss;
    }

}


void sair_jogo(){ //função de retornar aou menu principal  durante o jogo
    char entrada_saida; //entrada de saída do menu de fases
    char ok = 'n';

    while(ok != 'o'){
        printf("\nDeseja realmente sair do jogo?\n");
        printf("\n1 - SIM");
        printf("\n2 - NÃO\n");
        printf("\nDigite sua opção:   ");
        fflush(stdin);

        entrada_saida = getchar();
        fflush(stdin);

        if (isdigit(entrada_saida) != 0){

            if (entrada_saida == '1'){ //opção para sair ao menu principal
                personagem.sair = 1;
                system("cls"); 
                ok = 'o';
            }

            else if(entrada_saida == '2'){ //opção para continuar jogando
                system("cls");
                ok = 'o';
            }
            else{
                invalido();
            }

        }
        else{
            invalido();
        }

    }
    
}



int main(){ //principal

    int i, turno;

    char entrada; //entrada para os menus de fases
    char entrada_menu; //opções do menu
    char nome_j[11];  //nome do jogador

    //intro do jogo
    char intro_f1[] = "Intro/intro_f1.txt";
    char intro_f2[] = "Intro/intro_f2.txt";

    for (int i = 0; i < 7; i++){
        ler_arq_cor(intro_f1, AMARELO_C);
        espera_padrao();
        ler_arq_cor(intro_f2, VERMELHO_C);
        espera_padrao();
    }
    
    //menu do jogo
    char menu_jogo[] = "Intro/menu_inicial.txt";
    
    //contador de fases
    contador_fases = 0;

    //laço principal do jogo
    int sair = 0;

    while(sair == 0){ //enquanto sair for difente de zero
        ler_arq_cor(menu_jogo, AMARELO);

        entrada_menu = getchar();
        espera_padrao();
        fflush(stdin);

        if(isdigit(entrada_menu) != 0){


            if (entrada_menu == '2'){
                system("cls");
                ler_arq("Intro/dicas.txt");
                printf("\nAperte um tecla seguida de enter para continuar...");
                char str;
                str = getchar();
                fflush(stdin);
                if (isdigit(str) != 0){
                    
                }
                system("cls");
                continue;
            }

            else if (entrada_menu == '3'){
                system("cls");
                ler_arq("Intro/sobre.txt");
                printf("\nAperte um tecla seguida de enter para continuar...");
                char str;
                str = getchar();
                fflush(stdin);
                if (isdigit(str) != 0){
                    
                }
                system("cls");
                continue;
            }

            //colocar a opção de sair no menu
            else if(entrada_menu == '4'){
                printf("\nSAINDO...");
                fflush(stdout);
                sleep(3);
                system("cls");
                sair = 1;
            }

            else if (entrada_menu == '1'){
                contador_fases++;

                
                escolher_nome(nome_j); //função que escolhe o nome
                system("cls");

                substitui_nome("historia/intro.txt"); //história antes da fase 1
                sleep(5);
                system("cls");

                substitui_nome("historia/hfase1.txt"); //história antes da fase 1
                sleep(5);
                system("cls");

                //turno
                turno = 0;
                
                ///características fase 1
                //personagem
                personagem.vida = 20;
                personagem.ataque = 10;
                personagem.defesa = 8;
                personagem.envenenamento = 0;

                //boss
                boss.vida = 15;
                boss.ataque = 10;
                boss.defesa = 5;
                


                //abertura da fase 1
                char stg1_1[] = "fase1/stg1_1.txt";
                char stg1_2[] = "fase1/stg1_2.txt";
                for (i = 0; i < 6; i++){

                    ler_arq(stg1_1); espera_padrao();
                    ler_arq(stg1_2); espera_padrao();
                }

                for (i = 0; i < 6; i++){
                    printf(" < < < < <      E S T Á G I O      1      > > > > > \n"); espera_padrao();
                    printf("< < < < <       E S T Á G I O      1       > > > > >\n"); espera_padrao();
                }


                //nome da fase
                printf("╔════════════════════════════════════════════════════════════════╗\n");
                printf("║                     CEMITÉRIO DAS BESTAS                       ║\n");
                printf("╚════════════════════════════════════════════════════════════════╝\n");
                fflush(stdout);
                sleep(4);
                system("cls");


                //imagem base da fase 1
                char basefase1[] = "fase1/basefase1.txt";
                //enquanto a vida do personagem for maior que zero...
                while(boss.vida > 0 && personagem.vida > 0 && personagem.sair != 1){ //ciclo da fase 1
                    //entradas - usar o do e switch para diminuir a probabiblidade de bugs

                    ler_arq(basefase1);
                    menu_fase(contador_fases);

                    printf("\n\nDigite sua opção:       \n");
                    entrada = getchar();
                    fflush(stdin); //limpar o buffer de teclado
                    

                    //condições
                    if(isdigit(entrada) != 0){
                    
                        if (entrada == '1'){ //opção de ataque


                            if (turno % 7 == 0){ //caso de ataque de defesa do boss
                                system("cls");
                                //arquivos
                                char f1_pabd[] = "fase1/f1_pabd.txt";
                                char f2_pabd[] = "fase1/f2_pabd.txt";
                                char f3_pabd[] = "fase1/f3_pabd.txt";
                                char f4_pabd[] = "fase1/f4_pabd.txt";

                                //leitura de arquivo e limpeza  de tela (animação)        
                                ler_arq(f1_pabd); espera_padrao();
                                ler_arq(f2_pabd); espera_padrao();
                                ler_arq(f3_pabd); espera_padrao();
                                ler_arq(f4_pabd); espera_padrao();
                                ler_arq(f4_pabd); espera_padrao();
                                ler_arq(f4_pabd); espera_padrao();
                                ler_arq(f3_pabd); espera_padrao();
                                ler_arq(f2_pabd); espera_padrao();
                                ler_arq(f1_pabd); espera_padrao();
                                
                                atk_def(PABD);

                                turno++;
                                continue;
                                
                            }

                            else{
                                //caso em que o personagem ataca e o boss ataca
                                system("cls");
                                char f1_paba[] = "fase1/f1_paba.txt";
                                char f2_paba[] = "fase1/f2_paba.txt";
                                char f3_paba[] = "fase1/f3_paba.txt";
                                char f4_paba[] = "fase1/f4_paba.txt"; //repete 3 vezes
                                char f5_paba[] = "fase1/f5_paba.txt";
                                char f6_paba[] = "fase1/f6_paba.txt";
                                char f7_paba[] = "fase1/f7_paba.txt";
                                char f8_paba[] = "fase1/f8_paba.txt"; //repete 3 vezes

                                ler_arq(f1_paba); espera_padrao();
                                ler_arq(f2_paba); espera_padrao();
                                ler_arq(f3_paba); espera_padrao();
                                ler_arq(f4_paba); espera_padrao();
                                ler_arq(f4_paba); espera_padrao();
                                ler_arq(f4_paba); espera_padrao();
                                ler_arq(f3_paba); espera_padrao();
                                ler_arq(f2_paba); espera_padrao();
                                ler_arq(f1_paba); espera_padrao();
                                ler_arq(f5_paba); espera_padrao();
                                ler_arq(f6_paba); espera_padrao();
                                ler_arq(f7_paba); espera_padrao();
                                ler_arq(f8_paba); espera_padrao();
                                ler_arq(f8_paba); espera_padrao();
                                ler_arq(f8_paba); espera_padrao();
                                ler_arq(f7_paba); espera_padrao();
                                ler_arq(f6_paba); espera_padrao();
                                ler_arq(f5_paba); espera_padrao();       
                                
                                atk_def(PABA);
                                
                                turno++;
                                continue;
                               
                            }
                        }

                        else if (entrada == '2'){ //caso em que o  personagem defende

                            if (turno % 7 == 0){ //quando o personagem defende e o boss defende
                                system("cls");
                                char f1_pdbd[] = "fase1/f1_pdbd.txt";
                                char f2_pdbd[] = "fase1/f2_pdbd.txt";
                                char f3_pdbd[] = "fase1/f3_pdbd.txt";

                                ler_arq(f1_pdbd); espera_padrao();
                                ler_arq(f2_pdbd); espera_padrao();
                                ler_arq(f3_pdbd); espera_padrao();
                                ler_arq(f3_pdbd); espera_padrao();
                                ler_arq(f3_pdbd); espera_padrao();
                                ler_arq(f2_pdbd); espera_padrao();
                                ler_arq(f1_pdbd); espera_padrao();
                                
                                atk_def(PDBD);

                                turno++;
                                continue;

                            }
                            else{
                                system("cls");
                                char f1_pdba[] = "fase1/f1_pdba.txt";
                                char f2_pdba[] = "fase1/f2_pdba.txt";
                                char f3_pdba[] = "fase1/f3_pdba.txt";
                                char f4_pdba[] = "fase1/f4_pdba.txt";

                                ler_arq(f1_pdba); espera_padrao();
                                ler_arq(f2_pdba); espera_padrao();
                                ler_arq(f3_pdba); espera_padrao();
                                ler_arq(f4_pdba); espera_padrao();
                                ler_arq(f4_pdba); espera_padrao();
                                ler_arq(f4_pdba); espera_padrao();
                                ler_arq(f3_pdba); espera_padrao();
                                ler_arq(f2_pdba); espera_padrao();
                                ler_arq(f1_pdba); espera_padrao();
                                
                                atk_def(PDBA);

                                turno++;
                                continue;

                            }

                        }
                        else if(entrada == '3'){ //acosso a bagagem
                            system("cls");

                            char bgm_f1;

                            printf("==================================================================\n");
                            printf("                          B A G A G E M                           \n");
                            printf("==================================================================\n");
                            imprimir();
                            printf("==================================================================\n");
                            printf("   1 - VOLTAR                                                     \n");
                            printf("                                                                  \n");
                            printf("==================================================================\n");

                            printf("\nQual opção deseja? \n");
                            bgm_f1 = getchar();
                            fflush(stdin);
                            
                            if (isdigit(bgm_f1) != 0){
                                
                                if(bgm_f1 == '1'){ //opção que retorna
                                    system("cls");
                                    continue;
                                }
                                else{
                                    invalido();
                                    continue;
                                }
                            }
                            
                            else{
                                invalido();
                                continue;
                            }


                        }

                        else if (entrada == '4'){ //opção de sair do jogo
                            system("cls");
                            sair_jogo();
                        }

                        else{
                            invalido();
                            continue;
                        }
            
                    }

                    else{
                        invalido();
                        continue;
                    }


                }
                if (personagem.vida <= 0 && boss.vida >=0){ //configuração do game over
                    printf("\nGAME OVER\n");
                    sleep(3);
                    system("cls");
                    continue;
                }

                else if (personagem.sair == 1){ //sair do jogo
                    personagem.vida = 0;
                    boss.vida = 0;
                    sleep(1);
                    system("cls");
                    continue; 
                }


                contador_fases++; //contador de fases

                //encerramento e ganhos da fase 1
                char f1_ef1[] = "fase1/f1_ef1.txt";
                char f2_ef1[] = "fase1/f2_ef1.txt";
                char ganhosf1[] = "fase1/ganhosf1.txt";
                ler_arq(f1_ef1);
                usleep(3000000);
                system("cls");
                ler_arq(f2_ef1);
                usleep(3000000);
                system("cls");
                
                usleep(2000000);

                ler_arq(ganhosf1);
                usleep(3000000);
                system("cls");

                printf("\n\n E S T Á G I O   C O N C L U Í D O!\n\n"); //configurar animação (boss derrotado e ganho de ítens / história)
                usleep(3000000);
                system("cls");

                turno = 0; //zerar turno
                
                //adição de ítens
                adcionar("(+) Porção", 5, 0); //adcionada a porção

                substitui_nome("historia/hfase2.txt"); //história antes da fase 2
                sleep(5);
                system("cls");

                //abertura da fase 2
                char stg2_1[] = "fase2/stg2_1.txt";
                char stg2_2[] = "fase2/stg2_2.txt";
                for (i = 0; i < 6; i++){

                    ler_arq(stg2_1); espera_padrao();
                    ler_arq(stg2_2); espera_padrao();
                }

                for (i = 0; i < 6; i++){
                    printf(" < < < < <      E S T Á G I O      2      > > > > > \n"); espera_padrao();
                    printf("< < < < <       E S T Á G I O      2       > > > > >\n"); espera_padrao();
                }

                //nome da fase
                printf("╔════════════════════════════════════════════════════════════════╗\n");
                printf("║                     O APARECIMENTO DO REI                      ║\n");
                printf("╚════════════════════════════════════════════════════════════════╝\n");
                fflush(stdout);
                sleep(4);
                system("cls");

                //características do personagem e doi boss na fase 2
                boss.vida = 20;
                boss.ataque = 12;
                boss.defesa = 8;

                personagem.vida = 20;
                personagem.ataque = 11;
                personagem.defesa = 9;

                //imagem base da fase 2
                char basefase2[] = "fase2/basefase2.txt";

                while (boss.vida > 0 && personagem.vida > 0 && personagem.sair != 1){ //ciclo da fase 2

                    ler_arq(basefase2);
                    menu_fase(contador_fases);

                    printf("\n\nDigite sua opção:       \n");
                    entrada = getchar();
                    fflush(stdin);
                    
                    espera_padrao();

                    if(isdigit(entrada) != 0){

                        if (entrada == '1'){ //opção de ataque
                            // a cada 6 turnos o boss defende o ataque
                            if (turno % 6 == 0){
                                system("cls");
                                char f2_f1_pabd[] = "fase2/f2_f1_pabd.txt";
                                char f2_f2_pabd[] = "fase2/f2_f2_pabd.txt";
                                char f2_f3_pabd[] = "fase2/f2_f3_pabd.txt";
                                char f2_f4_pabd[] = "fase2/f2_f4_pabd.txt";

                                ler_arq(f2_f1_pabd); espera_padrao();
                                ler_arq(f2_f2_pabd); espera_padrao();
                                ler_arq(f2_f3_pabd); espera_padrao();
                                ler_arq(f2_f4_pabd); espera_padrao();
                                ler_arq(f2_f4_pabd); espera_padrao();
                                ler_arq(f2_f4_pabd); espera_padrao();
                                ler_arq(f2_f3_pabd); espera_padrao();
                                ler_arq(f2_f2_pabd); espera_padrao();
                                ler_arq(f2_f1_pabd); espera_padrao();

                                atk_def(PABD);
                                
                                turno++;
                                continue;
                            }

                            else{
                                system("cls");
                                char f2_f1_paba[] =  "fase2/f2_f1_paba.txt";
                                char f2_f2_paba[] =  "fase2/f2_f2_paba.txt";
                                char f2_f3_paba[] =  "fase2/f2_f3_paba.txt";
                                char f2_f4_paba[] =  "fase2/f2_f4_paba.txt";
                                char f2_f5_paba[] =  "fase2/f2_f5_paba.txt";
                                char f2_f6_paba[] =  "fase2/f2_f6_paba.txt";
                                char f2_f7_paba[] =  "fase2/f2_f7_paba.txt";

                                ler_arq(f2_f1_paba); espera_padrao();
                                ler_arq(f2_f2_paba); espera_padrao();
                                ler_arq(f2_f3_paba); espera_padrao();
                                ler_arq(f2_f4_paba); espera_padrao();
                                ler_arq(f2_f4_paba); espera_padrao();
                                ler_arq(f2_f4_paba); espera_padrao();
                                ler_arq(f2_f3_paba); espera_padrao();
                                ler_arq(f2_f2_paba); espera_padrao();
                                ler_arq(f2_f1_paba); espera_padrao();
                                ler_arq(f2_f5_paba); espera_padrao();
                                ler_arq(f2_f6_paba); espera_padrao();
                                ler_arq(f2_f7_paba); espera_padrao();
                                ler_arq(f2_f7_paba); espera_padrao();
                                ler_arq(f2_f7_paba); espera_padrao();
                                ler_arq(f2_f6_paba); espera_padrao();
                                ler_arq(f2_f5_paba); espera_padrao();
                                
                                atk_def(PABA);

                                turno++;
                                continue;

                            }

                        }

                        else if (entrada == '2'){ //opção de defesa

                            if (turno % 6 == 0){
                                system("cls");
                                char f2_f1_pdbd[] = "fase2/f2_f1_pdbd.txt";
                                char f2_f2_pdbd[] = "fase2/f2_f2_pdbd.txt";
                                char f2_f3_pdbd[] = "fase2/f2_f3_pdbd.txt";

                                ler_arq(f2_f1_pdbd); espera_padrao();
                                ler_arq(f2_f2_pdbd); espera_padrao();
                                ler_arq(f2_f3_pdbd); espera_padrao();
                                ler_arq(f2_f3_pdbd); espera_padrao();
                                ler_arq(f2_f3_pdbd); espera_padrao();
                                ler_arq(f2_f2_pdbd); espera_padrao();
                                ler_arq(f2_f1_pdbd); espera_padrao();

                                atk_def(PDBA);

                                turno++;
                                continue;

                            }
                            else{
                                system("cls");
                                char f2_f1_pdba[] = "fase2/f2_f1_pdba.txt";
                                char f2_f2_pdba[] = "fase2/f2_f2_pdba.txt";
                                char f2_f3_pdba[] = "fase2/f2_f3_pdba.txt";
                                char f2_f4_pdba[] = "fase2/f2_f4_pdba.txt";

                                ler_arq(f2_f1_pdba); espera_padrao();
                                ler_arq(f2_f2_pdba); espera_padrao();
                                ler_arq(f2_f3_pdba); espera_padrao();
                                ler_arq(f2_f4_pdba); espera_padrao();
                                ler_arq(f2_f4_pdba); espera_padrao();
                                ler_arq(f2_f4_pdba); espera_padrao();
                                ler_arq(f2_f3_pdba); espera_padrao();
                                ler_arq(f2_f2_pdba); espera_padrao();
                                ler_arq(f2_f1_pdba); espera_padrao();

                                atk_def(PDBA);

                                turno++;
                                continue;
                            }

                        }
                        else if (entrada == '3'){ //acesso a bagagem
                            system("cls");
                            char bgm_f2;

                            printf("==================================================================\n");
                            printf("                          B A G A G E M                           \n");
                            printf("==================================================================\n");
                            imprimir();
                            printf("==================================================================\n");
                            printf("   5 - VOLTAR                                                     \n");
                            printf("                                                                  \n");
                            printf("==================================================================\n");

                            printf("\nQual opção deseja? \n");
                            bgm_f2 = getchar();
                            fflush(stdin);

                            if (isdigit(bgm_f2) != 0){

                                if(bgm_f2 == '5'){
                                    system("cls");
                                    continue;
                                }

                                else if (bgm_f2 == '1'){
                                    encontrar(1);
                                    continue;
                                }
                                else{
                                    invalido();
                                    continue;
                                }
                            }

                            else{
                                invalido();
                                continue;
                            }
                            

                        }

                        else if (entrada == '4'){ //opção de sair do jogo
                            system("cls");
                            sair_jogo();

                        }

                        else{
                            invalido();
                            continue;
                        }

                    }
                    else{
                        invalido();
                        continue;
                    }

                }
                if (personagem.vida <= 0 && boss.vida >=0){ //game over
                    printf("\nGAME OVER\n");
                    fflush(stdin);
                    sleep(3);
                    system("cls");
                    continue;
                }

                else if (personagem.sair == 1){ //sair do jogo
                    personagem.vida = 0;
                    boss.vida = 0;
                    sleep(1);
                    system("cls");
                    continue; 
                }

                contador_fases++; //contador de fases

                //encerramento e ganhos da fase 2
                char f2_f1_ef2[] = "fase2/f2_f1_ef2.txt";
                char f2_f2_ef2[] = "fase2/f2_f2_ef2.txt";
                char ganhosf2[] = "fase2/ganhosf2.txt";

                ler_arq(f2_f1_ef2);
                usleep(3000000);
                system("cls");
                ler_arq(f2_f2_ef2);
                usleep(3000000);
                system("cls");

                usleep(2000000);

                ler_arq(ganhosf2);
                usleep(3000000);
                system("cls");

                printf("\n\n E S T Á G I O   C O N C L U Í D O!\n\n"); //configurar animação (boss derrotado e ganho de ítens / história)
                usleep(3000000);
                system("cls");

                turno = 0; //zerar turno

                //adição de ítens
                adcionar("(^) Antídoto", 1, 0);

                substitui_nome("historia/hfase3.txt"); //história antes da fase 3
                sleep(5);
                system("cls");

                //abertura da fase 3
                char stg3_1[] = "fase3/stg3_1.txt";
                char stg3_2[] = "fase3/stg3_2.txt";
                for (i = 0; i < 6; i++){
                    ler_arq(stg3_1); espera_padrao();
                    ler_arq(stg3_2); espera_padrao();
                }

                for (i = 0; i < 6; i++){
                    printf(" < < < < <      E S T Á G I O      3      > > > > > \n"); espera_padrao();
                    printf("< < < < <       E S T Á G I O      3       > > > > >\n"); espera_padrao();
                }

                //nome da fase
                printf("╔════════════════════════════════════════════════════════════════╗\n");
                printf("║                     DESERTO DE SABLEYNOIR                      ║\n");
                printf("╚════════════════════════════════════════════════════════════════╝\n");
                fflush(stdout);
                sleep(4);
                system("cls");

                //definindo características do personagem e do boss na fase 3
                boss.vida = 25;
                boss.ataque = 12;
                boss.defesa = 10;

                personagem.vida = 25;
                personagem.ataque = 12;
                personagem.defesa = 10;
                personagem.envenenamento = 0;

                //imagem base da fase 3
                char basefase3[] = "fase3/basefase3.txt";
                while (personagem.vida > 0 && boss.vida > 0 && personagem.sair != 1){ //ciclo da fase 3
                    

                    ler_arq(basefase3);
                    menu_fase(contador_fases);

                    printf("\n\nDigite sua opção:       \n");
                    entrada = getchar();
                    fflush(stdin);

                    if(isdigit(entrada) != 0){

                        if (entrada == '1'){ //opção de ataque

                            //a cada 4 turnos o personagem ataca e o boss ataca com envenenamento
                            if (turno % 6 == 0){
                                system("cls");
                                char f3_f1_pabae[] = "fase3/f3_f1_pabae.txt";
                                char f3_f2_pabae[] = "fase3/f3_f2_pabae.txt";
                                char f3_f3_pabae[] = "fase3/f3_f3_pabae.txt";
                                char f3_f4_pabae[] = "fase3/f3_f4_pabae.txt";
                                char f3_f5_pabae[] = "fase3/f3_f5_pabae.txt";
                                char f3_f6_pabae[] = "fase3/f3_f6_pabae.txt";
                                char f3_f7_pabae[] = "fase3/f3_f7_pabae.txt";
                                
                                ler_arq(f3_f1_pabae); espera_padrao();
                                ler_arq(f3_f2_pabae); espera_padrao();
                                ler_arq(f3_f3_pabae); espera_padrao();
                                ler_arq(f3_f4_pabae); espera_padrao();
                                ler_arq(f3_f4_pabae); espera_padrao();
                                ler_arq(f3_f4_pabae); espera_padrao();
                                ler_arq(f3_f5_pabae); espera_padrao();
                                ler_arq(f3_f6_pabae); espera_padrao();
                                ler_arq(f3_f7_pabae); espera_padrao();
                                ler_arq(f3_f7_pabae); espera_padrao();
                                ler_arq(f3_f7_pabae); espera_padrao();

                                atk_def(PABAE);
                                
                                if (personagem.envenenamento == 0){
                                    printf("\nVocê foi envenenado\n");
                                    personagem.envenenamento = 1;
                                    usleep(2000000);
                                    system("cls");
                                    turno = 0;
                                    continue;
                                }

                                turno++;

                            }
                            //a cada 6 turnos o boss defende do ataque
                            else if (turno % 5 == 0){
                                system("cls");
                                char f3_f1_pabd[] = "fase3/f3_f1_pabd.txt";
                                char f3_f2_pabd[] = "fase3/f3_f2_pabd.txt";
                                char f3_f3_pabd[] = "fase3/f3_f3_pabd.txt";
                                char f3_f4_pabd[] = "fase3/f3_f4_pabd.txt";

                                ler_arq(f3_f1_pabd); espera_padrao();
                                ler_arq(f3_f2_pabd); espera_padrao();
                                ler_arq(f3_f3_pabd); espera_padrao();
                                ler_arq(f3_f4_pabd); espera_padrao();
                                ler_arq(f3_f4_pabd); espera_padrao();
                                ler_arq(f3_f4_pabd); espera_padrao();
                                ler_arq(f3_f3_pabd); espera_padrao();
                                ler_arq(f3_f2_pabd); espera_padrao();
                                ler_arq(f3_f1_pabd); espera_padrao();

                                atk_def(PABD);

                                turno++;
                            }

                            // personagem ataca e o boss ataca normalmente
                            else{
                                system("cls");
                                char f3_f1_paba[] = "fase3/f3_f1_paba.txt";
                                char f3_f2_paba[] = "fase3/f3_f2_paba.txt";
                                char f3_f3_paba[] = "fase3/f3_f3_paba.txt";
                                char f3_f4_paba[] = "fase3/f3_f4_paba.txt";
                                char f3_f5_paba[] = "fase3/f3_f5_paba.txt";
                                char f3_f6_paba[] = "fase3/f3_f6_paba.txt";
                                char f3_f7_paba[] = "fase3/f3_f7_paba.txt";

                                ler_arq(f3_f1_paba); espera_padrao();
                                ler_arq(f3_f2_paba); espera_padrao();
                                ler_arq(f3_f3_paba); espera_padrao();
                                ler_arq(f3_f4_paba); espera_padrao();
                                ler_arq(f3_f4_paba); espera_padrao();
                                ler_arq(f3_f4_paba); espera_padrao();
                                ler_arq(f3_f3_paba); espera_padrao();
                                ler_arq(f3_f2_paba); espera_padrao();
                                ler_arq(f3_f1_paba); espera_padrao();
                                ler_arq(f3_f5_paba); espera_padrao();
                                ler_arq(f3_f6_paba); espera_padrao();
                                ler_arq(f3_f7_paba); espera_padrao();
                                ler_arq(f3_f7_paba); espera_padrao();
                                ler_arq(f3_f7_paba); espera_padrao();
                                ler_arq(f3_f6_paba); espera_padrao();
                                ler_arq(f3_f5_paba); espera_padrao();

                                atk_def(PABA);

                                turno++;
                            }

                        }
                        else if (entrada == '2'){ //opção de defesa
                            //personagem defende e boss defende
                            if (turno % 5 == 0){
                                system("cls");
                                char f3_f1_pdbd[] = "fase3/f3_f1_pdbd.txt";
                                char f3_f2_pdbd[] = "fase3/f3_f2_pdbd.txt";
                                char f3_f3_pdbd[] = "fase3/f3_f3_pdbd.txt";

                                ler_arq(f3_f1_pdbd); espera_padrao();
                                ler_arq(f3_f2_pdbd); espera_padrao();
                                ler_arq(f3_f3_pdbd); espera_padrao();
                                ler_arq(f3_f3_pdbd); espera_padrao();
                                ler_arq(f3_f3_pdbd); espera_padrao();
                                ler_arq(f3_f2_pdbd); espera_padrao();
                                ler_arq(f3_f1_pdbd); espera_padrao();

                                atk_def(PDBD);

                                turno++;

                            }
                            //personagem defende e boss ataca especial            
                            else if (turno == 4){
                                system("cls");
                                char f3_f1_pdbae[] = "fase3/f3_f1_pdbae.txt";
                                char f3_f2_pdbae[] = "fase3/f3_f2_pdbae.txt";
                                char f3_f3_pdbae[] = "fase3/f3_f3_pdbae.txt";
                                char f3_f4_pdbae[] = "fase3/f3_f4_pdbae.txt";

                                ler_arq(f3_f1_pdbae); espera_padrao();
                                ler_arq(f3_f2_pdbae); espera_padrao();
                                ler_arq(f3_f3_pdbae); espera_padrao();
                                ler_arq(f3_f4_pdbae); espera_padrao();
                                ler_arq(f3_f4_pdbae); espera_padrao();
                                ler_arq(f3_f4_pdbae); espera_padrao();
                                
                                atk_def(PDBAE);
                                
                                turno++;

                            }
                            //personagem defende e boss ataca normal
                            else{
                                system("cls");
                                char f3_f1_pdba[] = "fase3/f3_f1_pdba.txt";
                                char f3_f2_pdba[] = "fase3/f3_f2_pdba.txt";
                                char f3_f3_pdba[] = "fase3/f3_f3_pdba.txt";
                                char f3_f4_pdba[] = "fase3/f3_f4_pdba.txt";

                                ler_arq(f3_f1_pdba); espera_padrao();
                                ler_arq(f3_f2_pdba); espera_padrao();
                                ler_arq(f3_f3_pdba); espera_padrao();
                                ler_arq(f3_f4_pdba); espera_padrao();
                                ler_arq(f3_f4_pdba); espera_padrao();
                                ler_arq(f3_f4_pdba); espera_padrao();
                                ler_arq(f3_f3_pdba); espera_padrao();
                                ler_arq(f3_f2_pdba); espera_padrao();
                                ler_arq(f3_f1_pdba); espera_padrao();

                                atk_def(PDBA);

                                turno++;
                            }


                        }
                        else if (entrada == '3'){ //acesso a bagagem
                            system("cls");
                            char bgm_f3;

                            printf("==================================================================\n");
                            printf("                          B A G A G E M                           \n");
                            printf("==================================================================\n");
                            imprimir();
                            printf("==================================================================\n");
                            printf("   5 - VOLTAR                                                     \n");
                            printf("                                                                  \n");
                            printf("==================================================================\n");

                            printf("\nQual opção deseja? \n");
                            bgm_f3 = getchar();
                            fflush(stdin);

                            if (isdigit(bgm_f3) != 0){

                                if(bgm_f3 == '5'){
                                    system("cls");
                                    continue;
                                }

                                else if (bgm_f3 == '1'){
                                    encontrar(1);
                                    continue;
                                }

                                else if (bgm_f3 == '2'){
                                    encontrar(2);
                                    continue;
                                }

                                else{
                                    invalido();
                                    continue;
                                }

                            }

                            else{
                                invalido();
                                continue;
                            }

                        }

                        else if (entrada == '4'){ //opção de sair do jogo
                            system("cls");
                            sair_jogo();

                        }

                        else{
                            invalido();
                            continue;

                        }

                        if (personagem.envenenamento == 1){
                            personagem.vida = personagem.vida - 2;
                            printf("\nVocê está sofrendo com os efeitos do envenenamento\n");
                            printf("Você perdeu -2 de vida\n");
                            usleep(2000000);
                            system("cls");
                            continue;
                        }
                    }

                    else{
                        invalido();
                        continue;
                    }
                
                }
                if (personagem.vida <= 0 && boss.vida >=0){ //game over
                    printf("\nGAME OVER\n");
                    fflush(stdin);
                    sleep(3);
                    system("cls");
                    continue;
                }

                else if (personagem.sair == 1){ //sair do jogo
                    personagem.vida = 0;
                    boss.vida = 0;
                    sleep(1);
                    system("cls");
                    continue; 
                }

                contador_fases++; //contador de fases


                personagem.envenenamento = 0; // o envenenamento acaba ao passar de fase
                
                //encerramento e ganhos da fase 3
                char f3_f1_ef3[] = "fase3/f3_f1_ef3.txt";
                char f3_f2_ef3[] = "fase3/f3_f2_ef3.txt";
                char ganhosf3[] = "fase3/ganhosf3.txt";

                ler_arq(f3_f1_ef3);
                usleep(3000000);
                system("cls");
                ler_arq(f3_f2_ef3);
                usleep(3000000);
                system("cls");

                usleep(2000000);

                ler_arq(ganhosf3);
                usleep(3000000);
                system("cls");

                printf("\n\n E S T Á G I O   C O N C L U Í D O!\n\n"); //configurar animação (boss derrotado e ganho de ítens / história)
                usleep(3000000);
                system("cls");

                turno = 0; //zerar turno

                //adição de ítens
                adc_qtd("(^) Antídoto");
                adc_qtd("(+) Porção");

                substitui_nome("historia/hfase4.txt"); //história antes da fase 4
                sleep(5);
                system("cls");

                //abertura da fase 4
                char stg4_1[] = "fase4/stg4_1.txt";
                char stg4_2[] = "fase4/stg4_2.txt";
                
                for (i = 0; i < 6; i++){
                    ler_arq(stg4_1); espera_padrao();
                    ler_arq(stg4_2); espera_padrao();
                }

                for (i = 0; i < 6; i++){
                    printf(" < < < < <      E S T Á G I O      4      > > > > > \n"); espera_padrao();
                    printf("< < < < <       E S T Á G I O      4       > > > > >\n"); espera_padrao();
                }

                //nome da fase
                printf("╔════════════════════════════════════════════════════════════════╗\n");
                printf("║                      CAVERNA DE HASHMUTE                       ║\n");
                printf("╚════════════════════════════════════════════════════════════════╝\n");
                fflush(stdout);
                sleep(4);
                system("cls");

                //características do personagem e do boss na fase 4
                boss.vida = 30;
                boss.ataque = 11;
                boss.defesa = 9;

                personagem.vida = 25;
                personagem.ataque = 11;
                personagem.defesa = 9;


                char basefase4[] = "fase4/basefase4.txt";

                while(boss.vida > 0 && personagem.vida > 0 && personagem.sair != 1){ //ciclo da fase 4
                    
                    ler_arq(basefase4);
                    menu_fase(contador_fases);
                    

                    printf("\n\nDigite sua opção:       \n");
                    entrada = getchar();
                    fflush(stdin);


                    if(isdigit(entrada) != 0){

                        if (entrada == '1'){ //opção de ataque

                            if (turno % 4 == 0){ //personagem ataca e boss ataca com especial 
                                system("cls");
                                char f4_f1_pabae[] = "fase4/f4_f1_pabae.txt";
                                char f4_f2_pabae[] = "fase4/f4_f2_pabae.txt";
                                char f4_f3_pabae[] = "fase4/f4_f3_pabae.txt";
                                char f4_f4_pabae[] = "fase4/f4_f4_pabae.txt";
                                char f4_f5_pabae[] = "fase4/f4_f5_pabae.txt";
                                char f4_f6_pabae[] = "fase4/f4_f6_pabae.txt";
                                char f4_f7_pabae[] = "fase4/f4_f7_pabae.txt";

                                ler_arq(f4_f1_pabae); espera_padrao();
                                ler_arq(f4_f2_pabae); espera_padrao();
                                ler_arq(f4_f3_pabae); espera_padrao();
                                ler_arq(f4_f4_pabae); espera_padrao();
                                ler_arq(f4_f4_pabae); espera_padrao();
                                ler_arq(f4_f4_pabae); espera_padrao();
                                ler_arq(f4_f3_pabae); espera_padrao();
                                ler_arq(f4_f2_pabae); espera_padrao();
                                ler_arq(f4_f1_pabae); espera_padrao();
                                ler_arq(f4_f5_pabae); espera_padrao();
                                ler_arq(f4_f6_pabae); espera_padrao();
                                ler_arq(f4_f7_pabae); espera_padrao();
                                ler_arq(f4_f7_pabae); espera_padrao();
                                ler_arq(f4_f7_pabae); espera_padrao();

                                //dano comum de ataque
                                atk_def(PABAE);
                                
                                //dano do ataque especial
                                printf("O inimigo lhe causou um dano a mais\n");
                                printf("Você perdeu 2 pontos de vida\n");
                                usleep(2000000);
                                system("cls");
                                personagem.vida = personagem.vida - 2;

                                turno++;
                                continue;

                            }
                            else if(turno % 5 == 0){ //personagem ataca e boss defende
                                system("cls");
                                char f4_f1_pabd[] = "fase4/f4_f1_pabd.txt";
                                char f4_f2_pabd[] = "fase4/f4_f2_pabd.txt";
                                char f4_f3_pabd[] = "fase4/f4_f3_pabd.txt";
                                char f4_f4_pabd[] = "fase4/f4_f4_pabd.txt";

                                ler_arq(f4_f1_pabd); espera_padrao();
                                ler_arq(f4_f2_pabd); espera_padrao();
                                ler_arq(f4_f3_pabd); espera_padrao();
                                ler_arq(f4_f4_pabd); espera_padrao();
                                ler_arq(f4_f4_pabd); espera_padrao();
                                ler_arq(f4_f4_pabd); espera_padrao();
                                ler_arq(f4_f3_pabd); espera_padrao();
                                ler_arq(f4_f2_pabd); espera_padrao();
                                ler_arq(f4_f1_pabd); espera_padrao();

                                atk_def(PABD);

                                turno++;
                                continue;

                            }
                            else{ //personagem ataca e boss ataca
                                system("cls");
                                char f4_f1_paba[] = "fase4/f4_f1_paba.txt";
                                char f4_f2_paba[] = "fase4/f4_f2_paba.txt";
                                char f4_f3_paba[] = "fase4/f4_f3_paba.txt";
                                char f4_f4_paba[] = "fase4/f4_f4_paba.txt";
                                char f4_f5_paba[] = "fase4/f4_f5_paba.txt";
                                char f4_f6_paba[] = "fase4/f4_f6_paba.txt";
                                char f4_f7_paba[] = "fase4/f4_f7_paba.txt";
                                
                                ler_arq(f4_f1_paba); espera_padrao();
                                ler_arq(f4_f2_paba); espera_padrao();
                                ler_arq(f4_f3_paba); espera_padrao();
                                ler_arq(f4_f4_paba); espera_padrao();
                                ler_arq(f4_f4_paba); espera_padrao();
                                ler_arq(f4_f4_paba); espera_padrao();
                                ler_arq(f4_f3_paba); espera_padrao();
                                ler_arq(f4_f2_paba); espera_padrao();
                                ler_arq(f4_f1_paba); espera_padrao();
                                ler_arq(f4_f5_paba); espera_padrao();
                                ler_arq(f4_f6_paba); espera_padrao();
                                ler_arq(f4_f7_paba); espera_padrao();
                                ler_arq(f4_f7_paba); espera_padrao();
                                ler_arq(f4_f7_paba); espera_padrao();
                                ler_arq(f4_f6_paba); espera_padrao();
                                ler_arq(f4_f5_paba); espera_padrao();

                                atk_def(PABA);

                                turno++;
                                continue;
                                
                            }


                        }
                        else if (entrada == '2'){ //opção de defesa

                            if (turno % 4 == 0){ //personagem defende e boss defende
                                system("cls");
                                char f4_f1_pdbd[] = "fase4/f4_f1_pdbd.txt";
                                char f4_f2_pdbd[] = "fase4/f4_f2_pdbd.txt";
                                char f4_f3_pdbd[] = "fase4/f4_f3_pdbd.txt";

                                ler_arq(f4_f1_pdbd); espera_padrao();
                                ler_arq(f4_f2_pdbd); espera_padrao();
                                ler_arq(f4_f3_pdbd); espera_padrao();
                                ler_arq(f4_f3_pdbd); espera_padrao();
                                ler_arq(f4_f3_pdbd); espera_padrao();
                                ler_arq(f4_f2_pdbd); espera_padrao();
                                ler_arq(f4_f1_pdbd); espera_padrao();

                                atk_def(PDBD);

                                turno++;
                                continue;

                            }

                            else if (turno % 5 == 0){ //personagem defende a boss ataca especial 
                                system("cls");
                                char f4_f1_pdbae[] = "fase4/f4_f1_pdbae.txt";
                                char f4_f2_pdbae[] = "fase4/f4_f2_pdbae.txt";
                                char f4_f3_pdbae[] = "fase4/f4_f3_pdbae.txt";
                                char f4_f4_pdbae[] = "fase4/f4_f4_pdbae.txt";

                                ler_arq(f4_f1_pdbae); espera_padrao();
                                ler_arq(f4_f2_pdbae); espera_padrao();
                                ler_arq(f4_f3_pdbae); espera_padrao();
                                ler_arq(f4_f4_pdbae); espera_padrao();
                                ler_arq(f4_f4_pdbae); espera_padrao();
                                ler_arq(f4_f4_pdbae); espera_padrao();
                                ler_arq(f4_f3_pdbae); espera_padrao();
                                ler_arq(f4_f2_pdbae); espera_padrao();
                                ler_arq(f4_f1_pdbae); espera_padrao();
                                
                                atk_def(PDBAE);

                                turno++;
                                continue;
                                
                            }

                            else{ // personagem defende e boss ataca
                                system("cls");
                                char f4_f1_pdba[] = "fase4/f4_f1_pdba.txt";
                                char f4_f2_pdba[] = "fase4/f4_f2_pdba.txt";
                                char f4_f3_pdba[] = "fase4/f4_f3_pdba.txt";
                                char f4_f4_pdba[] = "fase4/f4_f4_pdba.txt";

                                ler_arq(f4_f1_pdba); espera_padrao();
                                ler_arq(f4_f2_pdba); espera_padrao();
                                ler_arq(f4_f3_pdba); espera_padrao();
                                ler_arq(f4_f4_pdba); espera_padrao();
                                ler_arq(f4_f4_pdba); espera_padrao();
                                ler_arq(f4_f4_pdba); espera_padrao();
                                ler_arq(f4_f3_pdba); espera_padrao();
                                ler_arq(f4_f2_pdba); espera_padrao();
                                ler_arq(f4_f1_pdba); espera_padrao();

                                atk_def(PDBA);

                                turno++;
                                continue;
                            }

                        }
                        else if(entrada == '3'){ //opção da bagagem
                            system("cls");
                            char bgm_f4;

                            printf("==================================================================\n");
                            printf("                          B A G A G E M                           \n");
                            printf("==================================================================\n");
                            imprimir();
                            printf("==================================================================\n");
                            printf("   5 - VOLTAR                                                     \n");
                            printf("                                                                  \n");
                            printf("==================================================================\n");

                            printf("\nQual opção deseja? \n");
                            bgm_f4 = getchar();
                            fflush(stdin);

                            if (isdigit(bgm_f4) != 0){

                                if(bgm_f4 == '5'){
                                    system("cls");
                                    continue;
                                }

                                else if (bgm_f4 == '1'){
                                    encontrar(1);
                                    continue;
                                }

                                else if (bgm_f4 == '2'){
                                    encontrar(2);
                                    continue;
                                }

                                else{
                                    invalido();
                                    continue;
                                }
                            }

                            else{
                                invalido();
                                continue;
                            }

                        }

                        else if (entrada == '4'){ //opção de sair do jogo
                            system("cls");
                            sair_jogo();

                        }

                        else{
                            invalido();
                            continue;
                        }
                    }

                    else{
                        invalido();
                        continue;
                    }

                }
                if (personagem.vida <= 0 && boss.vida >=0){ //game over 
                    printf("\nGAME OVER\n");
                    fflush(stdin);
                    sleep(3);
                    system("cls");
                    continue;
                }

                else if (personagem.sair == 1){ //sair do jogo
                    personagem.vida = 0;
                    boss.vida = 0;
                    sleep(1);
                    system("cls");
                    continue; 
                }

                contador_fases++;

                char f4_f1_ef4[] = "fase4/f4_f1_ef4.txt";
                char f4_f2_ef4[] = "fase4/f4_f2_ef4.txt";
                char ganhosf4[] = "fase4/ganhosf4.txt";
                ler_arq(f4_f1_ef4);
                usleep(3000000);
                system("cls");
                ler_arq(f4_f2_ef4);
                usleep(3000000);
                system("cls");

                usleep(2000000);

                ler_arq(ganhosf4);
                usleep(3000000);
                system("cls");

                printf("\n\n E S T Á G I O   C O N C L U Í D O!\n\n"); //configurar animação (boss derrotado e ganho de ítens / história)
                usleep(3000000);
                system("cls");

                turno = 0; //turno zera

                //adição de ítens
                adcionar("(º) Bracelete Imperial", 1, 0);

                substitui_nome("historia/hfase5.txt"); //história antes da fase 5
                sleep(5);
                system("cls");

                //abertura da fase
                char stg5_1[] = "fase5/stg5_1.txt";
                char stg5_2[] = "fase5/stg5_2.txt";
                
                for (i = 0; i < 6; i++){
                    ler_arq(stg5_1); espera_padrao();
                    ler_arq(stg5_2); espera_padrao();
                }

                for (i = 0; i < 6; i++){
                    printf(" < < < < <      E S T Á G I O      5      > > > > > \n"); espera_padrao();
                    printf("< < < < <       E S T Á G I O      5       > > > > >\n"); espera_padrao();
                }

                //nome da fase
                printf("╔════════════════════════════════════════════════════════════════╗\n");
                printf("║                         RUÍNAS SAGRADAS                        ║\n");
                printf("╚════════════════════════════════════════════════════════════════╝\n");
                fflush(stdout);
                sleep(4);
                system("cls");

                
                boss.vida = 30;
                boss.ataque = 11;
                boss.defesa = 9;

                personagem.vida = 25;
                personagem.ataque = 12;
                personagem.defesa = 10;

                char basefase5[] = "fase5/basefase5.txt";

                while (personagem.vida > 0 && boss.vida > 0 && personagem.sair != 1){ //ciclo da fase 5
                    
                    ler_arq(basefase5);
                    menu_fase(contador_fases);
                    
                    printf("\n\nDigite sua opção:       \n");
                    entrada = getchar();
                    fflush(stdin);

                    if(isdigit(entrada) != 0){

                        if (entrada == '1'){ //opção de ataque

                            if (turno % 4 ==0 ){ //personagem ataca e boss ataca com especial
                                system("cls");
                                char f5_f1_pabae[] = "fase5/f5_f1_pabae.txt";
                                char f5_f2_pabae[] = "fase5/f5_f2_pabae.txt";
                                char f5_f3_pabae[] = "fase5/f5_f3_pabae.txt";
                                char f5_f4_pabae[] = "fase5/f5_f4_pabae.txt";
                                char f5_f5_pabae[] = "fase5/f5_f5_pabae.txt";
                                char f5_f6_pabae[] = "fase5/f5_f6_pabae.txt";
                                char f5_f7_pabae[] = "fase5/f5_f7_pabae.txt";

                                ler_arq(f5_f1_pabae); espera_padrao();
                                ler_arq(f5_f2_pabae); espera_padrao();
                                ler_arq(f5_f3_pabae); espera_padrao();
                                ler_arq(f5_f4_pabae); espera_padrao();
                                ler_arq(f5_f4_pabae); espera_padrao();
                                ler_arq(f5_f4_pabae); espera_padrao();
                                ler_arq(f5_f3_pabae); espera_padrao();
                                ler_arq(f5_f2_pabae); espera_padrao();
                                ler_arq(f5_f1_pabae); espera_padrao();
                                ler_arq(f5_f5_pabae); espera_padrao();
                                ler_arq(f5_f6_pabae); espera_padrao();
                                ler_arq(f5_f7_pabae); espera_padrao();
                                ler_arq(f5_f7_pabae); espera_padrao();
                                ler_arq(f5_f7_pabae); espera_padrao();
                                
                                atk_def(PABAE);
                                
                                if (personagem.envenenamento == 0){
                                    printf("\nVocê foi envenenado\n");
                                    personagem.envenenamento = 1;
                                    usleep(2000000);
                                    system("cls");
                                    turno = 0;
                                    continue;
                                }
                                turno++;
                            }

                            else if (turno % 5 == 0){ //quando o personagem ataca e o boss defende
                                system("cls");
                                char f5_f1_pabd[] = "fase5/f5_f1_pabd.txt";
                                char f5_f2_pabd[] = "fase5/f5_f2_pabd.txt";
                                char f5_f3_pabd[] = "fase5/f5_f3_pabd.txt";
                                char f5_f4_pabd[] = "fase5/f5_f4_pabd.txt";

                                ler_arq(f5_f1_pabd); espera_padrao();
                                ler_arq(f5_f2_pabd); espera_padrao();
                                ler_arq(f5_f3_pabd); espera_padrao();
                                ler_arq(f5_f4_pabd); espera_padrao();
                                ler_arq(f5_f4_pabd); espera_padrao();
                                ler_arq(f5_f4_pabd); espera_padrao();
                                ler_arq(f5_f3_pabd); espera_padrao();
                                ler_arq(f5_f2_pabd); espera_padrao();
                                ler_arq(f5_f1_pabd); espera_padrao();

                                atk_def(PABD);

                                turno++;
                            }

                            else{ // quando o personagem ataca e o boss ataca
                                system("cls");
                                char f5_f1_paba[] = "fase5/f5_f1_paba.txt";
                                char f5_f2_paba[] = "fase5/f5_f2_paba.txt";
                                char f5_f3_paba[] = "fase5/f5_f3_paba.txt";
                                char f5_f4_paba[] = "fase5/f5_f4_paba.txt";
                                char f5_f5_paba[] = "fase5/f5_f5_paba.txt";
                                char f5_f6_paba[] = "fase5/f5_f6_paba.txt";
                                char f5_f7_paba[] = "fase5/f5_f7_paba.txt";
                                
                                ler_arq(f5_f1_paba); espera_padrao();
                                ler_arq(f5_f2_paba); espera_padrao();
                                ler_arq(f5_f3_paba); espera_padrao();
                                ler_arq(f5_f4_paba); espera_padrao();
                                ler_arq(f5_f4_paba); espera_padrao();
                                ler_arq(f5_f4_paba); espera_padrao();
                                ler_arq(f5_f3_paba); espera_padrao();
                                ler_arq(f5_f2_paba); espera_padrao();
                                ler_arq(f5_f1_paba); espera_padrao();
                                ler_arq(f5_f5_paba); espera_padrao();
                                ler_arq(f5_f6_paba); espera_padrao();
                                ler_arq(f5_f7_paba); espera_padrao();
                                ler_arq(f5_f7_paba); espera_padrao();
                                ler_arq(f5_f7_paba); espera_padrao();
                                ler_arq(f5_f6_paba); espera_padrao();
                                ler_arq(f5_f5_paba); espera_padrao();

                                atk_def(PABA);
                                
                                turno++;
                            }

                        }

                        else if (entrada == '2'){ //opção de defesa

                            if (turno % 4 == 0){ //quando o personagem defende e o boss defende
                                system("cls");
                                char f5_f1_pdbd[] = "fase5/f5_f1_pdbd.txt";
                                char f5_f2_pdbd[] = "fase5/f5_f2_pdbd.txt";
                                char f5_f3_pdbd[] = "fase5/f5_f3_pdbd.txt";

                                ler_arq(f5_f1_pdbd); espera_padrao();
                                ler_arq(f5_f2_pdbd); espera_padrao();
                                ler_arq(f5_f3_pdbd); espera_padrao();
                                ler_arq(f5_f3_pdbd); espera_padrao();
                                ler_arq(f5_f3_pdbd); espera_padrao();
                                ler_arq(f5_f2_pdbd); espera_padrao();
                                ler_arq(f5_f1_pdbd); espera_padrao();

                                atk_def(PDBD);

                                turno++;

                            }

                            else if (turno % 5 == 0){ // quando o pernonagem defende e o boss ataca especial
                                system("cls");
                                char f5_f1_pdbae[] = "fase5/f5_f1_pdbae.txt";
                                char f5_f2_pdbae[] = "fase5/f5_f2_pdbae.txt";
                                char f5_f3_pdbae[] = "fase5/f5_f3_pdbae.txt";
                                char f5_f4_pdbae[] = "fase5/f5_f4_pdbae.txt";

                                ler_arq(f5_f1_pdbae); espera_padrao();
                                ler_arq(f5_f2_pdbae); espera_padrao();
                                ler_arq(f5_f3_pdbae); espera_padrao();
                                ler_arq(f5_f4_pdbae); espera_padrao();
                                ler_arq(f5_f4_pdbae); espera_padrao();
                                ler_arq(f5_f4_pdbae); espera_padrao();
                                ler_arq(f5_f3_pdbae); espera_padrao();
                                ler_arq(f5_f2_pdbae); espera_padrao();
                                ler_arq(f5_f1_pdbae); espera_padrao();

                                atk_def(PDBAE);
                                
                                turno++;

                            }
                            
                            else{ //quando o personagem defende e o boss ataca
                                system("cls");
                                char f5_f1_pdba[] = "fase5/f5_f1_pdba.txt";
                                char f5_f2_pdba[] = "fase5/f5_f2_pdba.txt";
                                char f5_f3_pdba[] = "fase5/f5_f3_pdba.txt";
                                char f5_f4_pdba[] = "fase5/f5_f4_pdba.txt";

                                ler_arq(f5_f1_pdba); espera_padrao();
                                ler_arq(f5_f2_pdba); espera_padrao();
                                ler_arq(f5_f3_pdba); espera_padrao();
                                ler_arq(f5_f4_pdba); espera_padrao();
                                ler_arq(f5_f4_pdba); espera_padrao();
                                ler_arq(f5_f4_pdba); espera_padrao();
                                ler_arq(f5_f3_pdba); espera_padrao();
                                ler_arq(f5_f2_pdba); espera_padrao();
                                ler_arq(f5_f1_pdba); espera_padrao();

                                atk_def(PDBA);

                                turno++;
                            }

                        }
                        

                        else if (entrada == '3'){ //opção da bagagem
                            system("cls");
                            char bgm_f5;

                            printf("==================================================================\n");
                            printf("                          B A G A G E M                           \n");
                            printf("==================================================================\n");
                            imprimir();
                            printf("==================================================================\n");
                            printf("   5 - VOLTAR                                                     \n");
                            printf("                                                                  \n");
                            printf("==================================================================\n");

                            printf("\nQual opção deseja? \n");
                            bgm_f5 = getchar();
                            fflush(stdin);

                            if (isdigit(bgm_f5) != 0){

                                if(bgm_f5 == '5'){
                                    system("cls");
                                    continue;
                                }

                                else if (bgm_f5 == '1'){
                                    encontrar(1);
                                    continue;
                                }

                                else if (bgm_f5 == '2'){
                                    encontrar(2);
                                    continue;
                                }

                                else if (bgm_f5 == '3'){
                                    encontrar(3);
                                    continue;
                                }

                                else{
                                    invalido();
                                    continue;
                                }

                            }

                            else{
                                invalido();
                                continue;
                            }

                        }


                        else if (entrada == '4'){ //opção de sair do jogo
                            system("cls");
                            sair_jogo();

                        }

                        else if (entrada == '5'){ //opção de poder especial
                            printf("\n Você ainda não está pronto para utilizar esse poder!");
                            sleep(3);

                        }

                        else{
                            invalido();
                            continue;
                        }

                        if (personagem.envenenamento == 1){
                            personagem.vida = personagem.vida - 2;
                            printf("\nVocê está sofrendo com os efeitos do envenenamento\n");
                            printf("Você perdeu -2 de vida\n");
                            usleep(2000000);
                            system("cls");
                        }
                    }

                    else{
                        invalido();
                        system("cls");
                    }

                }

                if (personagem.vida <= 0 && boss.vida >=0){ //game over
                    printf("\nGAME OVER\n");
                    fflush(stdin);
                    sleep(3);
                    system("cls");
                    continue;
                }

                else if (personagem.sair == 1){ //sair do jogo
                    personagem.vida = 0;
                    boss.vida = 0;
                    sleep(1);
                    system("cls");
                    continue; 
                }

                contador_fases++; //conta as fases após terminar

                personagem.envenenamento = 0; // após o térmnio da fase, o status de envenamento acaba

                //encerramento da fase e ganhos da fase 5
                char f5_f1_ef5[] = "fase5/f5_f1_ef5.txt";
                char f5_f2_ef5[] = "fase5/f5_f2_ef5.txt";
                char ganhosf5[] = "fase5/ganhosf5.txt";

                ler_arq(f5_f1_ef5);
                usleep(3000000);
                system("cls");
                ler_arq(f5_f2_ef5);
                usleep(3000000);
                system("cls");

                usleep(2000000);

                ler_arq(ganhosf5);
                usleep(3000000);
                system("cls");

                printf("\n\n E S T Á G I O   C O N C L U Í D O!\n\n"); 
                usleep(3000000);
                system("cls");

                turno = 0; //turno zera

                //adição de ítens
                adc_qtd("(+) Porção");
                adc_qtd("(^) Antídoto");

                substitui_nome("historia/hfase6.txt"); //história antes da fase 6
                sleep(5);
                system("cls");

                //abertura da fase 6
                char stg6_1[] = "fase6/stg6_1.txt";
                char stg6_2[] = "fase6/stg6_2.txt";
                for (i = 0; i < 6; i++){
                    ler_arq(stg6_1); espera_padrao();
                    ler_arq(stg6_2); espera_padrao();
                }

                for (i = 0; i < 6; i++){
                    printf(" < < < < <      E S T Á G I O      6      > > > > > \n"); espera_padrao();
                    printf("< < < < <       E S T Á G I O      6       > > > > >\n"); espera_padrao();
                }

                //nome da fase
                printf("╔════════════════════════════════════════════════════════════════╗\n");
                printf("║                       CAVERNA MISTERIOSA                       ║\n");
                printf("╚════════════════════════════════════════════════════════════════╝\n");
                fflush(stdout);
                sleep(4);
                system("cls");

                //características do personagem e do boss na fase 6
                boss.vida = 35;
                boss.ataque = 13;
                boss.defesa = 10;

                personagem.vida = 35;
                personagem.ataque = 12;
                personagem.defesa = 10;
                personagem.especial = 0;

                char basefase6[] = "fase6/basefase6.txt";

                while(boss.vida > 0 && personagem.vida > 0 && personagem.sair != 1){ //ciclo da fase 6
                    
                    ler_arq(basefase6);
                    menu_fase(contador_fases);

                    printf("\n\nDigite sua opção:       \n");
                    entrada = getchar();
                    fflush(stdin);

                    if(isdigit(entrada) != 0){ //verifica a entrada

                        if (entrada == '1'){ //opção de ataque

                            if (turno % 3 == 0){ //personagem ataca e boss ataca com especial 
                                system("cls");
                                char f6_f1_pabae[] = "fase6/f6_f1_pabae.txt";
                                char f6_f2_pabae[] = "fase6/f6_f2_pabae.txt";
                                char f6_f3_pabae[] = "fase6/f6_f3_pabae.txt";
                                char f6_f4_pabae[] = "fase6/f6_f4_pabae.txt";
                                char f6_f5_pabae[] = "fase6/f6_f5_pabae.txt";
                                char f6_f6_pabae[] = "fase6/f6_f6_pabae.txt";
                                char f6_f7_pabae[] = "fase6/f6_f7_pabae.txt";

                                ler_arq(f6_f1_pabae); espera_padrao();
                                ler_arq(f6_f2_pabae); espera_padrao();
                                ler_arq(f6_f3_pabae); espera_padrao();
                                ler_arq(f6_f4_pabae); espera_padrao();
                                ler_arq(f6_f4_pabae); espera_padrao();
                                ler_arq(f6_f4_pabae); espera_padrao();
                                ler_arq(f6_f3_pabae); espera_padrao();
                                ler_arq(f6_f2_pabae); espera_padrao();
                                ler_arq(f6_f1_pabae); espera_padrao();
                                ler_arq(f6_f5_pabae); espera_padrao();
                                ler_arq(f6_f6_pabae); espera_padrao();
                                ler_arq(f6_f7_pabae); espera_padrao();
                                ler_arq(f6_f7_pabae); espera_padrao();
                                ler_arq(f6_f7_pabae); espera_padrao();
                    
                                //dano comum de ataque
                                atk_def(PABAE);
                                
                                //dano do ataque especial
                                printf("O inimigo lhe causou um dano a mais\n");
                                printf("Você perdeu 2 pontos de vida\n");
                                usleep(2000000);
                                system("cls");
                                personagem.vida = personagem.vida - 2;

                                turno++;
                                continue;

                            }

                            else if(turno % 4 == 0){ //personagem ataca e boss defende
                                system("cls");
                                char f6_f1_pabd[] = "fase6/f6_f1_pabd.txt";
                                char f6_f2_pabd[] = "fase6/f6_f2_pabd.txt";
                                char f6_f3_pabd[] = "fase6/f6_f3_pabd.txt";
                                char f6_f4_pabd[] = "fase6/f6_f4_pabd.txt";

                                ler_arq(f6_f1_pabd); espera_padrao();
                                ler_arq(f6_f2_pabd); espera_padrao();
                                ler_arq(f6_f3_pabd); espera_padrao();
                                ler_arq(f6_f4_pabd); espera_padrao();
                                ler_arq(f6_f4_pabd); espera_padrao();
                                ler_arq(f6_f4_pabd); espera_padrao();
                                ler_arq(f6_f3_pabd); espera_padrao();
                                ler_arq(f6_f2_pabd); espera_padrao();
                                ler_arq(f6_f1_pabd); espera_padrao();

                                atk_def(PABD);

                                turno++;
                                continue;

                            }

                            else{ //personagem ataca e boss ataca
                                system("cls");
                                char f6_f1_paba[] = "fase6/f6_f1_paba.txt";
                                char f6_f2_paba[] = "fase6/f6_f2_paba.txt";
                                char f6_f3_paba[] = "fase6/f6_f3_paba.txt";
                                char f6_f4_paba[] = "fase6/f6_f4_paba.txt";
                                char f6_f5_paba[] = "fase6/f6_f5_paba.txt";
                                char f6_f6_paba[] = "fase6/f6_f6_paba.txt";
                                char f6_f7_paba[] = "fase6/f6_f7_paba.txt";
                                
                                ler_arq(f6_f1_paba); espera_padrao();
                                ler_arq(f6_f2_paba); espera_padrao();
                                ler_arq(f6_f3_paba); espera_padrao();
                                ler_arq(f6_f4_paba); espera_padrao();
                                ler_arq(f6_f4_paba); espera_padrao();
                                ler_arq(f6_f4_paba); espera_padrao();
                                ler_arq(f6_f3_paba); espera_padrao();
                                ler_arq(f6_f2_paba); espera_padrao();
                                ler_arq(f6_f1_paba); espera_padrao();
                                ler_arq(f6_f5_paba); espera_padrao();
                                ler_arq(f6_f6_paba); espera_padrao();
                                ler_arq(f6_f7_paba); espera_padrao();
                                ler_arq(f6_f7_paba); espera_padrao();
                                ler_arq(f6_f7_paba); espera_padrao();
                                ler_arq(f6_f6_paba); espera_padrao();
                                ler_arq(f6_f5_paba); espera_padrao();

                                atk_def(PABA);

                                turno++;
                                continue;
                                
                            }


                        }
                        else if (entrada == '2'){ //opção defesa

                            if (turno % 4 == 0){ //personagem defende e boss defende
                                system("cls");
                                char f6_f1_pdbd[] = "fase6/f6_f1_pdbd.txt";
                                char f6_f2_pdbd[] = "fase6/f6_f2_pdbd.txt";
                                char f6_f3_pdbd[] = "fase6/f6_f3_pdbd.txt";

                                ler_arq(f6_f1_pdbd); espera_padrao();
                                ler_arq(f6_f2_pdbd); espera_padrao();
                                ler_arq(f6_f3_pdbd); espera_padrao();
                                ler_arq(f6_f3_pdbd); espera_padrao();
                                ler_arq(f6_f3_pdbd); espera_padrao();
                                ler_arq(f6_f2_pdbd); espera_padrao();
                                ler_arq(f6_f1_pdbd); espera_padrao();

                                atk_def(PDBD);

                                turno++;
                                continue;

                            }

                            else if (turno % 4 == 0){ //personagem defende a boss ataca especial 
                                system("cls");
                                char f6_f1_pdbae[] = "fase6/f6_f1_pdbae.txt";
                                char f6_f2_pdbae[] = "fase6/f6_f2_pdbae.txt";
                                char f6_f3_pdbae[] = "fase6/f6_f3_pdbae.txt";
                                char f6_f4_pdbae[] = "fase6/f6_f4_pdbae.txt";

                                ler_arq(f6_f1_pdbae); espera_padrao();
                                ler_arq(f6_f2_pdbae); espera_padrao();
                                ler_arq(f6_f3_pdbae); espera_padrao();
                                ler_arq(f6_f4_pdbae); espera_padrao();
                                ler_arq(f6_f4_pdbae); espera_padrao();
                                ler_arq(f6_f4_pdbae); espera_padrao();
                                ler_arq(f6_f3_pdbae); espera_padrao();
                                ler_arq(f6_f2_pdbae); espera_padrao();
                                ler_arq(f6_f1_pdbae); espera_padrao();
                                
                                atk_def(PDBAE);

                                turno++;
                                continue;
                            }

                            else{ // personagem defende e boss ataca
                                system("cls");
                                char f6_f1_pdba[] = "fase6/f6_f1_pdba.txt";
                                char f6_f2_pdba[] = "fase6/f6_f2_pdba.txt";
                                char f6_f3_pdba[] = "fase6/f6_f3_pdba.txt";
                                char f6_f4_pdba[] = "fase6/f6_f4_pdba.txt";

                                ler_arq(f6_f1_pdba); espera_padrao();
                                ler_arq(f6_f2_pdba); espera_padrao();
                                ler_arq(f6_f3_pdba); espera_padrao();
                                ler_arq(f6_f4_pdba); espera_padrao();
                                ler_arq(f6_f4_pdba); espera_padrao();
                                ler_arq(f6_f4_pdba); espera_padrao();
                                ler_arq(f6_f3_pdba); espera_padrao();
                                ler_arq(f6_f2_pdba); espera_padrao();
                                ler_arq(f6_f1_pdba); espera_padrao();

                                atk_def(PDBA); 

                                turno++;
                                continue;
                            }

                        }
                        else if(entrada == '3'){ //opção da mochila
                            system("cls");
                            char bgm_f6;

                            printf("==================================================================\n");
                            printf("                          B A G A G E M                           \n");
                            printf("==================================================================\n");
                            imprimir();
                            printf("==================================================================\n");
                            printf("   5 - VOLTAR                                                     \n");
                            printf("                                                                  \n");
                            printf("==================================================================\n");

                            printf("\nQual opção deseja? \n");
                            bgm_f6 = getchar();
                            fflush(stdin);

                            if (isdigit(bgm_f6) != 0){

                                if(bgm_f6 == '5'){
                                    system("cls");
                                    continue;
                                }

                                else if (bgm_f6 == '1'){
                                    encontrar(1);
                                    continue;
                                }

                                else if (bgm_f6 == '2'){
                                    encontrar(2);
                                    continue;
                                }

                                else if (bgm_f6 == '3'){
                                    encontrar(3);
                                    continue;
                                }

                                else if(bgm_f6 == '4'){
                                    encontrar(4);
                                    continue;
                                }
                                else{
                                    invalido();
                                    continue;
                                }

                            }
                            else{
                                invalido();
                                continue;
                            }

                        }

                        else if (entrada == '4'){ //opção de sair do jogo
                            system("cls");
                            sair_jogo();

                        }

                        else if (entrada == '5'){ //opção de poder especial
                            printf("\n Você ainda não está pronto para utilizar esse poder!");
                            sleep(3);

                        }

                        else{
                            invalido();
                            continue;
                        }

                    }

                    else{
                        invalido();
                        continue;
                    }
                
                }
                if (personagem.vida <= 0 && boss.vida >=0){ //game over
                    printf("\nGAME OVER\n");
                    fflush(stdin);
                    sleep(3);
                    system("cls");
                    continue;
                }

                else if (personagem.sair == 1){ //sair do jogo
                    personagem.vida = 0;
                    boss.vida = 0;
                    sleep(1);
                    system("cls");
                    continue; 
                }

                contador_fases++; //conta a fase após o térmnio

                //encerramento e ganhos da fase 6
                char f6_f1_ef6[] = "fase6/f6_f1_ef6.txt";
                char f6_f2_ef6[] = "fase6/f6_f2_ef6.txt";
                char ganhosf6[] = "fase6/ganhosf6.txt";
                ler_arq(f6_f1_ef6);
                usleep(3000000);
                system("cls");
                ler_arq(f6_f2_ef6);
                usleep(3000000);
                system("cls");

                usleep(2000000);

                ler_arq(ganhosf6);
                usleep(3000000);
                system("cls");

                printf("\n\n E S T Á G I O   C O N C L U Í D O!\n\n"); //configurar animação (boss derrotado e ganho de ítens / história)
                usleep(3000000);
                system("cls");

                turno = 0; //turno zera

                //adição de ítens
                adc_qtd("(º) Bracelete Imperial");
                adc_qtd("(+) Porção");
                adc_qtd("(+) Porção");

                substitui_nome("historia/hfase7.txt"); //história antes da fase 7
                sleep(5);
                system("cls");


                //abertura da fase 7
                char stg7_1[] = "fase7/stg7_1.txt";
                char stg7_2[] = "fase7/stg7_2.txt";
                for (i = 0; i < 6; i++){
                    ler_arq(stg7_1); espera_padrao();
                    ler_arq(stg7_2); espera_padrao();
                }

                for (i = 0; i < 6; i++){
                    printf(" < < < < <      E S T Á G I O      7      > > > > > \n"); espera_padrao();
                    printf("< < < < <       E S T Á G I O      7       > > > > >\n"); espera_padrao();
                }

                //nome da fase
                printf("╔════════════════════════════════════════════════════════════════╗\n");
                printf("║                      A ARMADILHA DO XAMÃ                       ║\n");
                printf("╚════════════════════════════════════════════════════════════════╝\n");
                fflush(stdout);
                sleep(4);
                system("cls");

                //definindo características do personagem e do boss na fase 7
                boss.vida = 70;
                boss.ataque = 14;
                boss.defesa = 11;

                personagem.vida = 55;
                personagem.ataque = 12;
                personagem.defesa = 10;
                personagem.envenenamento = 0;

                //imagem base da fase 7
                char basefase7[] = "fase7/basefase7.txt";

                while(boss.vida > 0 && personagem.vida > 0 && personagem.sair != 1){ //ciclo da fase 7
                    
                    ler_arq(basefase7);
                    menu_fase(contador_fases);

                    printf("\n\nDigite sua opção:       \n");
                    entrada = getchar();
                    fflush(stdin);

                    if(isdigit(entrada) != 0){
                        if (entrada == '1'){

                            if (turno % 3 == 0){ //personagem ataca e boss ataca com especial 1
                                system("cls");
                                char f7_f1_pabae1[] = "fase7/f7_f1_pabae1.txt";
                                char f7_f2_pabae1[] = "fase7/f7_f2_pabae1.txt";
                                char f7_f3_pabae1[] = "fase7/f7_f3_pabae1.txt";
                                char f7_f4_pabae1[] = "fase7/f7_f4_pabae1.txt";
                                char f7_f5_pabae1[] = "fase7/f7_f5_pabae1.txt";
                                char f7_f6_pabae1[] = "fase7/f7_f6_pabae1.txt";
                                char f7_f7_pabae1[] = "fase7/f7_f7_pabae1.txt";

                                ler_arq(f7_f1_pabae1); espera_padrao();
                                ler_arq(f7_f2_pabae1); espera_padrao();
                                ler_arq(f7_f3_pabae1); espera_padrao();
                                ler_arq(f7_f4_pabae1); espera_padrao();
                                ler_arq(f7_f4_pabae1); espera_padrao();
                                ler_arq(f7_f4_pabae1); espera_padrao();
                                ler_arq(f7_f3_pabae1); espera_padrao();
                                ler_arq(f7_f2_pabae1); espera_padrao();
                                ler_arq(f7_f1_pabae1); espera_padrao();
                                ler_arq(f7_f5_pabae1); espera_padrao();
                                ler_arq(f7_f6_pabae1); espera_padrao();
                                ler_arq(f7_f7_pabae1); espera_padrao();
                                ler_arq(f7_f7_pabae1); espera_padrao();
                                ler_arq(f7_f7_pabae1); espera_padrao();
                    
                                //dano comum de ataque
                                atk_def(PABAE1);
                                
                                //dano do ataque especial
                                if (personagem.envenenamento == 0){
                                    printf("\nVocê foi envenenado\n");
                                    personagem.envenenamento = 1;
                                    usleep(2000000);
                                    system("cls");
                                    turno = 0;
                                    continue;
                                }

                                turno++;
                                continue;

                            }

                            else if (turno % 4){ //personagem ataca e boss ataca com especial 2
                                system("cls");
                                char f7_f1_pabae2[] = "fase7/f7_f1_pabae2.txt";
                                char f7_f2_pabae2[] = "fase7/f7_f2_pabae2.txt";
                                char f7_f3_pabae2[] = "fase7/f7_f3_pabae2.txt";
                                char f7_f4_pabae2[] = "fase7/f7_f4_pabae2.txt";
                                char f7_f5_pabae2[] = "fase7/f7_f5_pabae2.txt";
                                char f7_f6_pabae2[] = "fase7/f7_f6_pabae2.txt";
                                char f7_f7_pabae2[] = "fase7/f7_f7_pabae2.txt";

                                ler_arq(f7_f1_pabae2); espera_padrao();
                                ler_arq(f7_f2_pabae2); espera_padrao();
                                ler_arq(f7_f3_pabae2); espera_padrao();
                                ler_arq(f7_f4_pabae2); espera_padrao();
                                ler_arq(f7_f4_pabae2); espera_padrao();
                                ler_arq(f7_f4_pabae2); espera_padrao();
                                ler_arq(f7_f3_pabae2); espera_padrao();
                                ler_arq(f7_f2_pabae2); espera_padrao();
                                ler_arq(f7_f1_pabae2); espera_padrao();
                                ler_arq(f7_f5_pabae2); espera_padrao();
                                ler_arq(f7_f6_pabae2); espera_padrao();
                                ler_arq(f7_f7_pabae2); espera_padrao();
                                ler_arq(f7_f7_pabae2); espera_padrao();
                                ler_arq(f7_f7_pabae2); espera_padrao();

                                atk_def(PABAE2);

                                //dano do ataque especial
                                printf("O inimigo lhe causou um dano a mais\n");
                                printf("Você perdeu 2 pontos de vida\n");
                                usleep(2000000);
                                system("cls");
                                personagem.vida = personagem.vida - 2;

                                turno++;

                            } 

                            else if(turno % 5 == 0){ //personagem ataca e boss defende
                                system("cls");
                                char f7_f1_pabd[] = "fase7/f7_f1_pabd.txt";
                                char f7_f2_pabd[] = "fase7/f7_f2_pabd.txt";
                                char f7_f3_pabd[] = "fase7/f7_f3_pabd.txt";
                                char f7_f4_pabd[] = "fase7/f7_f4_pabd.txt";

                                ler_arq(f7_f1_pabd); espera_padrao();
                                ler_arq(f7_f2_pabd); espera_padrao();
                                ler_arq(f7_f3_pabd); espera_padrao();
                                ler_arq(f7_f4_pabd); espera_padrao();
                                ler_arq(f7_f4_pabd); espera_padrao();
                                ler_arq(f7_f4_pabd); espera_padrao();
                                ler_arq(f7_f3_pabd); espera_padrao();
                                ler_arq(f7_f2_pabd); espera_padrao();
                                ler_arq(f7_f1_pabd); espera_padrao();

                                atk_def(PABD);

                                turno++;
                            }


                            else{ //personagem ataca e boss ataca
                                system("cls");
                                char f7_f1_paba[] = "fase7/f7_f1_paba.txt";
                                char f7_f2_paba[] = "fase7/f7_f2_paba.txt";
                                char f7_f3_paba[] = "fase7/f7_f3_paba.txt";
                                char f7_f4_paba[] = "fase7/f7_f4_paba.txt";
                                char f7_f5_paba[] = "fase7/f7_f5_paba.txt";
                                char f7_f6_paba[] = "fase7/f7_f6_paba.txt";
                                char f7_f7_paba[] = "fase7/f7_f7_paba.txt";
                                
                                ler_arq(f7_f1_paba); espera_padrao();
                                ler_arq(f7_f2_paba); espera_padrao();
                                ler_arq(f7_f3_paba); espera_padrao();
                                ler_arq(f7_f4_paba); espera_padrao();
                                ler_arq(f7_f4_paba); espera_padrao();
                                ler_arq(f7_f4_paba); espera_padrao();
                                ler_arq(f7_f3_paba); espera_padrao();
                                ler_arq(f7_f2_paba); espera_padrao();
                                ler_arq(f7_f1_paba); espera_padrao();
                                ler_arq(f7_f5_paba); espera_padrao();
                                ler_arq(f7_f6_paba); espera_padrao();
                                ler_arq(f7_f7_paba); espera_padrao();
                                ler_arq(f7_f7_paba); espera_padrao();
                                ler_arq(f7_f7_paba); espera_padrao();
                                ler_arq(f7_f6_paba); espera_padrao();
                                ler_arq(f7_f5_paba); espera_padrao();

                                atk_def(PABA);

                                turno++;
                            }


                        }
                        else if (entrada == '2'){

                            if (turno % 4 == 0){ //personagem defende e boss defende
                                system("cls");
                                char f7_f1_pdbd[] = "fase7/f7_f1_pdbd.txt";
                                char f7_f2_pdbd[] = "fase7/f7_f2_pdbd.txt";
                                char f7_f3_pdbd[] = "fase7/f7_f3_pdbd.txt";

                                ler_arq(f7_f1_pdbd); espera_padrao();
                                ler_arq(f7_f2_pdbd); espera_padrao();
                                ler_arq(f7_f3_pdbd); espera_padrao();
                                ler_arq(f7_f3_pdbd); espera_padrao();
                                ler_arq(f7_f3_pdbd); espera_padrao();
                                ler_arq(f7_f2_pdbd); espera_padrao();
                                ler_arq(f7_f1_pdbd); espera_padrao();

                                atk_def(PDBD);

                                turno++;
                            }

                            else if (turno % 4 == 0){ //personagem defende a boss ataca com especial 1
                                system("cls");
                                char f7_f1_pdbae1[] = "fase7/f7_f1_pdbae1.txt";
                                char f7_f2_pdbae1[] = "fase7/f7_f2_pdbae1.txt";
                                char f7_f3_pdbae1[] = "fase7/f7_f3_pdbae1.txt";
                                char f7_f4_pdbae1[] = "fase7/f7_f4_pdbae1.txt";
                                
                                ler_arq(f7_f1_pdbae1); espera_padrao();
                                ler_arq(f7_f2_pdbae1); espera_padrao();
                                ler_arq(f7_f3_pdbae1); espera_padrao();
                                ler_arq(f7_f4_pdbae1); espera_padrao();
                                ler_arq(f7_f4_pdbae1); espera_padrao();
                                ler_arq(f7_f4_pdbae1); espera_padrao();
                        
                                atk_def(PDBAE1);

                                turno++;
                            }

                            else if (turno % 5 == 0){ //personagem defende a boss ataca com especial 2
                                system("cls");
                                char f7_f1_pdbae2[] = "fase7/f7_f1_pdbae2.txt";
                                char f7_f2_pdbae2[] = "fase7/f7_f2_pdbae2.txt";
                                char f7_f3_pdbae2[] = "fase7/f7_f3_pdbae2.txt";
                                char f7_f4_pdbae2[] = "fase7/f7_f4_pdbae2.txt";
                            

                                ler_arq(f7_f1_pdbae2); espera_padrao();
                                ler_arq(f7_f2_pdbae2); espera_padrao();
                                ler_arq(f7_f3_pdbae2); espera_padrao();
                                ler_arq(f7_f4_pdbae2); espera_padrao();
                                ler_arq(f7_f4_pdbae2); espera_padrao();
                                ler_arq(f7_f4_pdbae2); espera_padrao();
                        
                                atk_def(PDBAE2);

                                turno++;
                            }

                            else{ // personagem defende e boss ataca
                                system("cls");
                                char f7_f1_pdba[] = "fase7/f7_f1_pdba.txt";
                                char f7_f2_pdba[] = "fase7/f7_f2_pdba.txt";
                                char f7_f3_pdba[] = "fase7/f7_f3_pdba.txt";
                                char f7_f4_pdba[] = "fase7/f7_f4_pdba.txt";

                                ler_arq(f7_f1_pdba); espera_padrao();
                                ler_arq(f7_f2_pdba); espera_padrao();
                                ler_arq(f7_f3_pdba); espera_padrao();
                                ler_arq(f7_f4_pdba); espera_padrao();
                                ler_arq(f7_f4_pdba); espera_padrao();
                                ler_arq(f7_f4_pdba); espera_padrao();
                                ler_arq(f7_f3_pdba); espera_padrao();
                                ler_arq(f7_f2_pdba); espera_padrao();
                                ler_arq(f7_f1_pdba); espera_padrao();

                                atk_def(PDBA);

                                turno++;
                            }

                        }
                        else if(entrada == '3'){
                            system("cls");
                            char bgm_f7;

                            printf("==================================================================\n");
                            printf("                          B A G A G E M                           \n");
                            printf("==================================================================\n");
                            imprimir();
                            printf("==================================================================\n");
                            printf("   5 - VOLTAR                                                     \n");
                            printf("                                                                  \n");
                            printf("==================================================================\n");

                            printf("\nQual opção deseja? \n");
                            bgm_f7 = getchar();
                            fflush(stdin);

                            if (isdigit(bgm_f7) != 0){

                                if(bgm_f7 == '5'){
                                    system("cls");
                                    continue;
                                }

                                else if (bgm_f7 == '1'){
                                    encontrar(1);
                                    continue;
                                }

                                else if (bgm_f7 == '2'){
                                    encontrar(2);
                                    continue;
                                }

                                else if (bgm_f7 == '3'){
                                    encontrar(3);
                                    continue;
                                }

                                else if(bgm_f7 == '4'){
                                    encontrar(4);
                                    continue;
                                }

                                else{
                                    invalido();
                                    continue;
                                }

                            }

                            else{
                                invalido();
                                continue;
                            }

                        }

                        else if(entrada == '5'){

                                if (turno % 3 == 0){ //quando o personagem ataca com especial e boss ataca
                                system("cls");
                                char f7_f1_paeba[] = "fase7/f7_f1_paeba.txt";
                                char f7_f2_paeba[] = "fase7/f7_f2_paeba.txt";
                                char f7_f3_paeba[] = "fase7/f7_f3_paeba.txt";
                                char f7_f4_paeba[] = "fase7/f7_f4_paeba.txt";
                                char f7_f5_paeba[] = "fase7/f7_f5_paeba.txt";
                                char f7_f6_paeba[] = "fase7/f7_f6_paeba.txt";

                                ler_arq(f7_f1_paeba); espera_padrao();
                                ler_arq(f7_f2_paeba); espera_padrao();
                                ler_arq(f7_f3_paeba); espera_padrao();
                                ler_arq(f7_f3_paeba); espera_padrao();
                                ler_arq(f7_f3_paeba); espera_padrao();
                                ler_arq(f7_f4_paeba); espera_padrao();
                                ler_arq(f7_f5_paeba); espera_padrao();
                                ler_arq(f7_f6_paeba); espera_padrao();
                                ler_arq(f7_f6_paeba); espera_padrao();
                                ler_arq(f7_f6_paeba); espera_padrao();
                                

                                //ataque especial do personagem
                                printf("Você causou um poco a mais de dano\n");
                                printf("O Boss perdeu -2 pontos de vida\n");
                                usleep(2000000);
                                system("cls");
                                boss.vida = boss.vida - 2;

                                atk_def(PAEBA);

                                turno++;
                            }

                            else if(turno % 4 == 0){ //quando o personagem atacaca com especial e o boss defende
                                system("cls");
                                char f7_f1_paebd[] = "fase7/f7_f1_paebd.txt";
                                char f7_f2_paebd[] = "fase7/f7_f2_paebd.txt";
                                char f7_f3_paebd[] = "fase7/f7_f3_paebd.txt";
                                char f7_f4_paebd[] = "fase7/f7_f4_paebd.txt";

                                ler_arq(f7_f1_paebd); espera_padrao();
                                ler_arq(f7_f2_paebd); espera_padrao();
                                ler_arq(f7_f3_paebd); espera_padrao();
                                ler_arq(f7_f4_paebd); espera_padrao();
                                ler_arq(f7_f4_paebd); espera_padrao();
                                ler_arq(f7_f4_paebd); espera_padrao();

                                atk_def(PAEBD);
                                
                                turno++;
                            }

                            else if(turno % 5 == 0){ //qunado o personagem ataca com especial e o boss ataca com especial 1
                                system("cls");
                                char f7_f1_paebae1[] = "fase7/f7_f1_paebae1.txt";
                                char f7_f2_paebae1[] = "fase7/f7_f2_paebae1.txt";
                                char f7_f3_paebae1[] = "fase7/f7_f3_paebae1.txt";
                                char f7_f4_paebae1[] = "fase7/f7_f4_paebae1.txt";
                                char f7_f5_paebae1[] = "fase7/f7_f5_paebae1.txt";
                                char f7_f6_paebae1[] = "fase7/f7_f6_paebae1.txt";

                                ler_arq(f7_f1_paebae1); espera_padrao();
                                ler_arq(f7_f2_paebae1); espera_padrao();
                                ler_arq(f7_f3_paebae1); espera_padrao();
                                ler_arq(f7_f3_paebae1); espera_padrao();
                                ler_arq(f7_f3_paebae1); espera_padrao();
                                ler_arq(f7_f4_paebae1); espera_padrao();
                                ler_arq(f7_f5_paebae1); espera_padrao();
                                ler_arq(f7_f6_paebae1); espera_padrao();
                                ler_arq(f7_f6_paebae1); espera_padrao();
                                ler_arq(f7_f6_paebae1); espera_padrao();
                                

                                atk_def(PAEBAE1);

                                //ataque especial do personagem
                                printf("Você causou um poco a mais de dano\n");
                                printf("O Boss perdeu -2 pontos de vida\n");
                                usleep(2000000);
                                system("cls");
                                boss.vida = boss.vida - 2;

                                //envenenamento
                                if (personagem.envenenamento == 0){
                                    printf("\nVocê foi envenenado\n");
                                    personagem.envenenamento = 1;
                                    usleep(2000000);
                                    system("cls");
                                    turno = 0;
                                    continue;
                                }

                                turno++;
                            }

                            else if(turno % 7 == 0){ //qunado o personagem ataca com especial e o boss ataca com especial 2
                                system("cls");
                                char f7_f1_paebae2[] = "fase7/f7_f1_paebae2.txt";
                                char f7_f2_paebae2[] = "fase7/f7_f2_paebae2.txt";
                                char f7_f3_paebae2[] = "fase7/f7_f3_paebae2.txt";
                                char f7_f4_paebae2[] = "fase7/f7_f4_paebae2.txt";
                                char f7_f5_paebae2[] = "fase7/f7_f5_paebae2.txt";
                                char f7_f6_paebae2[] = "fase7/f7_f6_paebae2.txt";

                                ler_arq(f7_f1_paebae2); espera_padrao();
                                ler_arq(f7_f2_paebae2); espera_padrao();
                                ler_arq(f7_f3_paebae2); espera_padrao();
                                ler_arq(f7_f3_paebae2); espera_padrao();
                                ler_arq(f7_f3_paebae2); espera_padrao();
                                ler_arq(f7_f4_paebae2); espera_padrao();
                                ler_arq(f7_f5_paebae2); espera_padrao();
                                ler_arq(f7_f6_paebae2); espera_padrao();
                                ler_arq(f7_f6_paebae2); espera_padrao();
                                ler_arq(f7_f6_paebae2); espera_padrao();
                               
                                //ataque especial do personagem
                                printf("Você causou um poco a mais de dano\n");
                                printf("O Boss perdeu -2 pontos de vida\n");
                                usleep(2000000);
                                system("cls");
                                boss.vida = boss.vida - 2;

                                //dano do ataque especial
                                printf("O inimigo lhe causou um dano a mais\n");
                                printf("Você perdeu 2 pontos de vida\n");
                                usleep(2000000);
                                system("cls");
                                personagem.vida = personagem.vida - 2;

                                atk_def(PAEBAE2);

                                turno++;
                            }

                            else{
                                printf("\n Seu poder ainda não está carregado!\n");
                                sleep(2);
                                system("cls");
                            }  

                        }

                        else if (entrada == '4'){ //opção de sair do jogo
                            system("cls");
                            sair_jogo();

                        }

                        else{ //impedindo oputras entradas no menu
                            invalido();
                            continue;
                        }

                        if (personagem.envenenamento == 1){
                            personagem.vida = personagem.vida - 2;
                            printf("\nVocê está sofrendo com os efeitos do envenenamento\n");
                            printf("Você perdeu -2 de vida\n");
                            usleep(2000000);
                            system("cls");
                            continue;
                        }
                    }

                    else{
                        invalido();
                        continue;
                    }
                }

                if (personagem.vida <= 0 && boss.vida >=0){ //game over
                    printf("\nGAME OVER\n");
                    fflush(stdin);
                    sleep(3);
                    system("cls");
                    continue;
                }

                else if (personagem.sair == 1){ //sair do jogo
                    personagem.vida = 0;
                    boss.vida = 0;
                    sleep(1);
                    system("cls");
                    continue; 
                }

                //encerramento da fase
                char f7_f1_ef7[] = "fase7/f7_f1_ef7.txt";
                char f7_f2_ef7[] = "fase7/f7_f2_ef7.txt";
                ler_arq(f7_f1_ef7);
                usleep(3000000);
                system("cls");
                ler_arq(f7_f2_ef7);
                usleep(3000000);
                system("cls");

                usleep(2000000);


                printf("\n\n E S T Á G I O   C O N C L U Í D O!\n\n"); //configurar animação (boss derrotado e ganho de ítens / história)
                usleep(3000000);
                system("cls");

                substitui_nome("historia/desfecho.txt"); //desfecho
                sleep(5);
                system("cls");

                printf("╔════════════════════════════════════════════════════════════════╗\n");
                printf("║                               FIM                              ║\n");
                printf("╚════════════════════════════════════════════════════════════════╝\n");
                fflush(stdout);
                sleep(4);
                system("cls");

            }

            else{
                invalido();
                continue;
            }
            
        }
        else{ //impedindo outras entradas no menu
            invalido();
            continue;
        }


    }
    //printf("\nAinda Falta Aprimorar!");
    usleep(2000000);


    


    return 0;
}
