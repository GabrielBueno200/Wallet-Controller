#include <locale.h>
#include <gtk/gtk.h>
#include <string.h>
#include "GTKAbas.h"
#include "funcsLogin.h"
#include "funcsUser.h"
#include "funcsGTK.h"
#include "main.h"

#define MAX 100

//Struct de meses, será responsável por identificar os meses (será colocado em um array para identificar o mês pela sua posição)
struct Meses{
	char nome[20];
};

/*=================================================================================
		              VARIÁVEIS GLOBAIS
==================================================================================*/
//de outros arquivos
extern struct usuarioLogado user; // captura os dados do usuário loga
extern int qtdFinal; //captura a quantidade de categorias cadastradas pelo usuário
extern struct Transacao* transacoes; /* será utilizada como array dinâmico (malloc), e reunirá todas as transações feitas pelo 								 usuário durante a execução do programa*/
extern int qtdTransacoes; //captura a quantidade de transacoes cadastradas pelo usuário. Inicialmente zerada.

extern GtkWidget *comboboxTransacoes;
extern GtkWidget *comboboxCategorias;
extern GtkWidget *comboboxConta;
extern GtkWidget *campoSaldo;
extern GtkWidget *window;

struct Meses meses[12]; //Array de meses, linkará os meses pelas suas posições



//define o nome dos meses pelos seus índices
void defineMeses(){
	strcpy(meses[0].nome, "Janeiro");    strcpy(meses[1].nome, "Fevereiro");    strcpy(meses[2].nome, "Março");
	strcpy(meses[3].nome, "Abril");      strcpy(meses[4].nome, "Maio");         strcpy(meses[5].nome, "Junho");
	strcpy(meses[6].nome, "Julho");      strcpy(meses[7].nome, "Agosto");       strcpy(meses[8].nome, "Setembro");
	strcpy(meses[9].nome, "Outubro");    strcpy(meses[10].nome, "Novembro");    strcpy(meses[11].nome, "Dezembro");
}


//Concatena o nome da pasta (constituída pelo CPF do usuário logado) ao nome da categoria
void criaCaminhoArquivo(char* nomeArquivo, char* categoria){
	sprintf(nomeArquivo, "%s/%s.txt", user.CPF, categoria);
}

//pega as transações disponíveis no arquivo de cada categoria e armazena no array dinâmico de struct de transações
void atualizaTransacoes(){
	
	FILE* arquivoCategoria; //variável de arquivo (para as categorias)
	char enderecoArquivo[MAX], linha[MAX]; //enderecoArquivo - armazena os endereços de cada arquivo
									       //linha - armazena as linhas dos arquivos
        
        //variáveis para armazenar os dados extraídos do arquivo
	int dia, mes, ano, hora, minuto;  
	double valor;
        char descricao[MAX*2];
	
	/*laço para percorrer o array dinâmico de categorias contido no struct de usuário.
	Utilizado para que cada arquivo seja analisado através do seu respectivo nome (que se encontra no array citado)*/
	int i;
        int contLinhas = 1; //captura os índices de cada linha do arquivo
	for(i = 0; i < qtdFinal; i++){
		
            //identifica o caminho do arquivo - "pasta/nomecategoria.txt" - formado pelo cpf do usuário e pelo nome da categoria analisada
            criaCaminhoArquivo(enderecoArquivo, user.Categoria[i].Nome);

            //abre o arquivo da categoria analisada no modo de leitura
            arquivoCategoria = fopen(enderecoArquivo, "r");

            //Laço para percorrer cada linha do arquivo analisado
            while (fgets(linha, MAX, arquivoCategoria) != NULL){

                //extrai os dados do arquivo
                if(contLinhas % 2 == 1){
                    sscanf(linha, "%d %*c %d %*c %d   %*s  %d %*c %d   %*c  %*s %*s %lf", &dia, &mes, &ano, &hora, &minuto, &valor);
                } else if(contLinhas % 2 == 0){
                    strcpy(descricao, linha);
                    removeQuebra(descricao);
                }
                
                //se a linha for par (toda linha par representa o fim de uma transação - linha da descrição)
                if(contLinhas % 2 == 0){
                    /*realoca o array dinâmico de transações, aumentando sua capacidade de armazenamento
                    de acordo com o número de linhas encontradas (= nº de transações)

                    OBS* realoca para o valor de qtdTransacoes + 1 pois o valor do relloc não poderia ser mudado para 0 bytes,
                    o que eventualmente ocorreria na primeira vez que o laço fosse inicializado (qtdTransacoes = 0) caso não houvesse tal soma*/
                    transacoes = realloc(transacoes, sizeof(struct Transacao)*(qtdTransacoes+1));


                    /*Armazena os valores extraídos de cada linha dos arquivos (ou seja, cada transação já registrada)
                    no array dinâmico de struct de transações*/
                    transacoes[qtdTransacoes].Data[0] = dia;
                    transacoes[qtdTransacoes].Data[1] = mes;
                    transacoes[qtdTransacoes].Data[2] = ano;
                    transacoes[qtdTransacoes].Horario[0] = hora;
                    transacoes[qtdTransacoes].Horario[1] = minuto;
                    transacoes[qtdTransacoes].valor = valor;
                    strcpy(transacoes[qtdTransacoes].descricao, descricao);
                    transacoes[qtdTransacoes].Categoria =  &user.Categoria[i]; //seta a categoria do struct de transações para o endereço 

                    qtdTransacoes++; //incrementa a quantidade de transações de acordo com o número de transações lidas

                }

                contLinhas++;  //incrementa a quantidade de linhas lidas no arquivo														   

            }
            fclose(arquivoCategoria); //fecha o arquivo anteriormente analisado
	}
	
}



void imprimeTransacoes(int opc, struct camposUser* dados){
    //variável para a impressão dos detalhes da transação
    gchar transacao[MAX*2];
    
    //limpa o textarea
    limpaBuffer(dados->buffer);
    
    int j;
    // Laço para percorrer o array dinâmico de struct de transações
    for(j = 0; j < qtdTransacoes; j++){
		
        /*Verifica se a categoria da transação analisada tem o nome igual ao da categoria informada pelo usuário
        Se tiver, printa a transação*/
        if(strcmp(transacoes[j].Categoria->Nome, user.Categoria[opc-1].Nome) == 0){
            sprintf(transacao,"%d/%d/%d as %d:%d - Total: R$ %.2f\nDescrição: %s\n\n", 
            transacoes[j].Data[0], transacoes[j].Data[1], transacoes[j].Data[2],
            transacoes[j].Horario[0], transacoes[j].Horario[1], transacoes[j].valor,
            transacoes[j].descricao);
            
            insereTextoTextarea(&dados->buffer, &dados->iter, transacao);
        }
    }    
    
}


//imprime o relatório anual do ano informado
void imprimeRelatorioAnual(GtkDialog *caixaDialogo, gint response_id, struct camposUser* dados){
    //captura o ano informado pelo usuário no combobox
    int ano;
    sscanf(gtk_combo_box_text_get_active_text (GTK_COMBO_BOX_TEXT(dados->campoDialog)), "%d", &ano);
    
    //fecha a janela de diálogo
    gtk_widget_destroy (GTK_WIDGET (caixaDialogo));
    
    
    //variáveis para o laço
    int mes, i, j, qtdMes;
    
    //limpa o buffer do textarea
    limpaBuffer(dados->buffer);
    
    
    //variáveis para impressão das transações
    gchar cabecalho[MAX];
    gchar nomeCategoria[MAX];
    gchar nomeMes[MAX];
    gchar detalhesTransacao[MAX];
    gchar msg[MAX];


    //laço para rodar cada mês do ano
    for(mes = 11; mes >= 0; mes--){
        
        //laço para rodar cada categoria
        for(i = 0; i < qtdFinal; i++){
            
            qtdMes = 0; //quantidade de transações no mês analisado no laço 
            
            //laço para rodar cada transação de cada categoria
            for(j = 0; j < qtdTransacoes ; j++){
                
                    /*compara se o nome da transação é igual ao nome da categoria cadastrada na transação
                     * se o mês cadastrado na transação é igual ao mês analisado no laço e se o ano 
                     * cadastrado na transação é igual ao ano informado pelo usuário*/
                    if(strcmp(transacoes[j].Categoria->Nome, user.Categoria[i].Nome) == 0 
                            && transacoes[j].Data[1] == mes+1 && transacoes[j].Data[2] == ano){
                            sprintf(detalhesTransacao, "\n%d/%d/%d as %d:%d - Total: R$ %.2f\nDescrição: %s\n", 
                            transacoes[j].Data[0], transacoes[j].Data[1], transacoes[j].Data[2],
                            transacoes[j].Horario[0], transacoes[j].Horario[1], transacoes[j].valor, 
                            transacoes[j].descricao);
                            qtdMes++;
                            
                            insereTextoTextarea(&dados->buffer, &dados->iter, detalhesTransacao);
                    }
            }
            
            //avisa caso não houver transação cadastrada no mês analisado
            if(qtdMes == 0){
                sprintf(msg, "\n   Nenhuma transação cadastrada no mês %d\n", mes+1);
                
                insereTextoTextarea(&dados->buffer, &dados->iter, msg);
            }
            
            //printa o nome da categoria analisada
            sprintf(nomeCategoria,"\n--------------------------------------------\n"
            "	                   %s\n"
            "--------------------------------------------", user.Categoria[i].Nome);
            
            insereTextoTextarea(&dados->buffer, &dados->iter, nomeCategoria);
            
            

        }
        
        //imprime o nome do mês analisado
        sprintf(nomeMes,"\n\n\n\n==================================\n"
            "                                 %s\n"
         "==================================", meses[mes].nome);
            
        insereTextoTextarea(&dados->buffer, &dados->iter, nomeMes);
    }
    
    //imprime o cabeçalho com o ano
    sprintf(cabecalho,"======================================\n"
                        "  RELATÓRIO ANUAL DE TRANSAÇÕES - %d\n"
                     "======================================", ano);
  
    insereTextoTextarea(&dados->buffer, &dados->iter, cabecalho);
    
    //OBS*
    //A ORDEM DE IMPRESSÃO É INVERTIDA PQ A INSERÇÃO DE TEXTO NO TEXTAREA É FEITA DE BAIXO PARA CIMA 

    
}


//caixa de diálogo pedindo ao usuário q informe o ano desejado
void RelatorioAnual(GtkWidget *btn, struct camposUser* dados){
  GtkWidget *caixaDialogo;
  GtkWidget *area, *combobox;
  GtkWidget *label;
  

  
  //Cria uma caixa de diálogo para o usuário inserir o mês
  caixaDialogo = gtk_dialog_new_with_buttons ("Atenção", 
                                        GTK_WINDOW(window), 
                                        GTK_DIALOG_MODAL, 
                                        GTK_STOCK_OK, 
                                        GTK_RESPONSE_OK, 
                                        NULL);

  /*Cria uma label e um combobox e adiciona à uma área de conteúdo, 
   * posteriormente adicionando esse área à caixa de diálogo*/
  area = gtk_dialog_get_content_area (GTK_DIALOG (caixaDialogo));
  label = gtk_label_new ("Insira o ano:");
  
  combobox = gtk_combo_box_text_new ();
            
  int i;
  gchar* n_ano[4]; //variável para o ano
  
  //laço para inserir cada ano no combobox (de 2000 a 2019)
  for (i = 2000; i <= 2019; i++){
    sprintf(n_ano, "%d", i);
    gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT(combobox), n_ano);
  }
    
  //seta a primeira posição ativa
  gtk_combo_box_set_active (GTK_COMBO_BOX (combobox), 0);
  
  gtk_container_add (GTK_CONTAINER (area), label);
  gtk_container_add (GTK_CONTAINER (area), combobox);
  

  //exibe a caixa de diálogo
  gtk_widget_show_all (caixaDialogo);
  
  //adiciona o combobox aos dados posteriormente utilizados
  dados->campoDialog = combobox;
  
  //Chama a função para exibir o relatório ao usuário clicar em OK
  g_signal_connect (GTK_DIALOG (caixaDialogo), 
                    "response", 
                    G_CALLBACK (imprimeRelatorioAnual), 
                    dados);
}



//imprime o relatório mensal de transações 
void imprimeRelatorioMensal(GtkDialog *caixaDialogo, gint response_id, struct camposUser* dados){
    //captura o mês informado pelo usuário
    int mes;
    sscanf(gtk_combo_box_text_get_active_text (GTK_COMBO_BOX_TEXT(dados->campoDialog)), "%d", &mes);
    //fecha a janela de diálogo
    gtk_widget_destroy (GTK_WIDGET (caixaDialogo));
    
    
    //variáveis a ser utilizada no laço
    int i, j, qtdMes;
    
    //limpa o buffer do textarea
    limpaBuffer(dados->buffer);
    
    //variáveis para impressão no textarea
    gchar cabecalho[MAX];
    gchar nomeCategoria[MAX];
    gchar detalhesTransacao[MAX];
    gchar msg[MAX];
    
    
    //laço para percorrer todas as categorias
    for(i = 0; i < qtdFinal; i++){
       
       //contador de transações no mês
       qtdMes = 0;
        
       //laço para percorrer todas as transações de cada categoria
       for(j = 0; j < qtdTransacoes; j++){
           
            //compara o nome da categoria com o nome da categoria registrada na transação
            if(strcmp(transacoes[j].Categoria->Nome, user.Categoria[i].Nome) == 0
               && transacoes[j].Data[1] == mes){
                    //imprime os detalhes da transação
                    sprintf(detalhesTransacao, "\n%d/%d/%d as %d:%d - Total: R$ %.2f\nDescrição: %s\n", 
                    transacoes[j].Data[0], transacoes[j].Data[1], transacoes[j].Data[2],
                    transacoes[j].Horario[0], transacoes[j].Horario[1], transacoes[j].valor, 
                    transacoes[j].descricao);
                    qtdMes++;

                    insereTextoTextarea(&dados->buffer, &dados->iter, detalhesTransacao);
            }
       }
       
       //imprime um aviso caso não hava nenhuma transação cadastrada no mês informado
       if(qtdMes == 0){
               sprintf(msg, "\n   Nenhuma transação cadastrada no mês %d\n", mes);
               insereTextoTextarea(&dados->buffer, &dados->iter, msg);
       }  

       
       //imprime o nome da categoria
       sprintf(nomeCategoria,"\n\n--------------------------------------------\n"
                                 "	              %s\n"
                              "--------------------------------------------", user.Categoria[i].Nome);
       insereTextoTextarea(&dados->buffer, &dados->iter, nomeCategoria);
            
    }
   
    //imprime o cabeçalho
     sprintf(cabecalho,"==========================================\n"
                                " RELATÓRIO MENSAL DE TRANSAÇÕES - %s\n"
                           "==========================================", meses[mes-1].nome);
  
    insereTextoTextarea(&dados->buffer, &dados->iter, cabecalho);
    
    //OBS*
    //A ORDEM DE IMPRESSÃO É INVERTIDA PQ A INSERÇÃO DE TEXTO NO TEXTAREA É FEITA DE BAIXO PARA CIMA 
    
}


//caixa de diálogo pedindo ao usuário que informe o mês desejado
void RelatorioMensal(GtkWidget *btn, struct camposUser* dados){
  //variáveis para a caixa de diálogo
  GtkWidget *caixaDialogo;
  GtkWidget *area, *combobox;
  GtkWidget *label;
  

  
  //Cria uma caixa de diálogo para o usuário inserir o mês
  caixaDialogo = gtk_dialog_new_with_buttons ("Atenção", 
                                        GTK_WINDOW(window), 
                                        GTK_DIALOG_MODAL, 
                                        GTK_STOCK_OK, 
                                        GTK_RESPONSE_OK, 
                                        NULL);

  /*Cria uma label e um combobox e adiociona à uma área de conteúdo, 
   * posteriormente adicionando esse área à caixa de diálogo*/
  area = gtk_dialog_get_content_area (GTK_DIALOG (caixaDialogo));
  label = gtk_label_new ("Insira mês:");
  
  combobox = gtk_combo_box_text_new ();
            
   
  gchar* n_mes[2]; //variável para os meses
  
  //laço para inserir cada mês em um combobox
  int i;
  for (i = 1; i <= 12; i++){
    sprintf(n_mes, "%d", i);
    gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT(combobox), n_mes);
  }
  
  //seta a primeira posição ativa
  gtk_combo_box_set_active (GTK_COMBO_BOX (combobox), 0);
    
  
  gtk_container_add (GTK_CONTAINER (area), label);
  gtk_container_add (GTK_CONTAINER (area), combobox);

  
  //exibe a caixa de diálogo
  gtk_widget_show_all (caixaDialogo);
  
  //insere o combobox aos dados posteriormente utilizados
  dados->campoDialog = combobox;
  
  //Chama a função para exibir o relatório ao usuário clicar em OK
  g_signal_connect (GTK_DIALOG (caixaDialogo), 
                    "response", 
                    G_CALLBACK (imprimeRelatorioMensal), 
                    dados);
}

//organiza as transações em ordem crescente
void organizaTransacoes(){
	
    //contadores
    int i, j;

    //struct auxiliar para que nenhuma posição do array dinâmico de transacoes seja perdida
    struct Transacao Aux;

    //Laço para fazer o sort em ordem crescente das transações (por ano, mês e dia)
    for(i = 0; i < qtdTransacoes; i++){ //analisa a posição superior
        for(j = i + 1; j < qtdTransacoes; j++){ //analisa a posição inferior

            Aux = transacoes[j]; //seta o struct auxiliar para  aposição superior

            //Verifica se o ano da posição superior é maior que o da inferior, se for, troca o conteúdo delas
            if(transacoes[i].Data[2] > transacoes[j].Data[2]){
                    transacoes[j] = transacoes[i];
                    transacoes[i] = Aux;

            //Verifica se o ano da posição superior é igual ao da posição inferior
            } else if (transacoes[i].Data[2] == transacoes[j].Data[2]){
                //Se o mês da posição superior for maior que o da transação inferior, troca o conteúdo delas
                if(transacoes[i].Data[1] > transacoes[j].Data[1]){
                        transacoes[j] = transacoes[i];
                        transacoes[i] = Aux;

                //Se o mês da posição superior for igual ao da posição inferior
                } else if(transacoes[i].Data[1] == transacoes[j].Data[1]){

                    //Se o dia da posição superior for maior que o da posição inferior, troca o conteúdo delas
                    if(transacoes[i].Data[0] > transacoes[j].Data[0]){
                            transacoes[j] = transacoes[i];
                            transacoes[i] = Aux;
                    }
                }
            }
        }
    }
	
}

//armazena todas as transações nos arquivos
void armazenaTransacoes(){
	
    // Organiza as transações por ano, mês e dia
    organizaTransacoes();

    //Inicializa a variável de arquivo (de categorias) bem como o nome deste
    FILE* categoriaArquivo;
    char enderecoArquivo[MAX];	

    /*laço para percorrer o array dinâmico de categorias contido no struct de usuário.
    Utilizado para que cada arquivo seja analisado através do seu respectivo nome (que se encontra no array citado)*/
    int i, j;					
    for(i = 0; i < qtdFinal; i++){

        //identifica o caminho do arquivo - "pasta/nomecategoria.txt" - formado pelo cpf do usuário e pelo nome da categoria analisada
        criaCaminhoArquivo(enderecoArquivo, user.Categoria[i].Nome);

        //abre o arquivo da categoria analisada no modo de escrita
        categoriaArquivo = fopen(enderecoArquivo, "w");

        //Laço para percorrer todas as transações contidas no array dinâmico de struct de transações 
        //(e posteriormente escrevê-las no arquivo analisado)
        
        for(j = 0; j < qtdTransacoes; j++){

                //variáveis para armazenar os dados da transação analisada (nomes mais curtos para não poluir o fprintf
                int dia = transacoes[j].Data[0], mes = transacoes[j].Data[1], ano = transacoes[j].Data[2];
                int hora = transacoes[j].Horario[0], minuto = transacoes[j].Horario[1];
                double valor = transacoes[j].valor;

                /*verifica se o nome da categoria analisada (pertencente ao array din. de struct de categorias) coincide 
                com o nome da categoria da transação analisada (pertencente ao array din. de struct de transações).
                Se coincidir, escreve no arquivo referente à categoria analisada todos os dados da transação*/

                if(strcmp(transacoes[j].Categoria->Nome, user.Categoria[i].Nome) == 0){
                        fprintf(categoriaArquivo, "%d / %d / %d as %d : %d - Total: R$ %.2f\n",dia, mes, ano, hora, minuto, valor);
                        fprintf(categoriaArquivo, "%s\n", transacoes[j].descricao);
                }
        }

        //fecha o arquivo referente à categoria analisada
        fclose(categoriaArquivo);
    }	
}


//cadastra a transação de acordo com os dados informados pelo usuário
void cadastraTransacao(GtkWidget *btn, struct camposTransacao* dados){
    //Soma uma unidade na variável global referente ao tamanho do array dinâmico de struct de transacoes
    qtdTransacoes++;

    //Realoca o array dinâmico de struct de transacoes para possuir uma posição a mais (já que um novo elemento será adicionado a ele)
    transacoes = realloc(transacoes, sizeof(struct Transacao)*qtdTransacoes);

    //variável para armazenar a opção de categoria escolhida pelo usuário
    int opc = gtk_combo_box_get_active(GTK_COMBO_BOX(comboboxTransacoes));

    //Atribui ao ponteiro de struct de categoria do struct transações o endereço da categoria informada pelo usuário
    //OBS* utiliza-se qtdTransacoes-1 como posição para que o array não seja estourado
    transacoes[qtdTransacoes-1].Categoria = &user.Categoria[opc-1];

    //Captura a data da transação informada
    int dia, mes, ano;
    sscanf(gtk_entry_get_text(GTK_ENTRY(dados->campoDia)), "%d", &dia);
    sscanf(gtk_entry_get_text(GTK_ENTRY(dados->campoMes)), "%d", &mes);
    sscanf(gtk_entry_get_text(GTK_ENTRY(dados->campoAno)), "%d", &ano);
    
    transacoes[qtdTransacoes-1].Data[0] = dia;
    transacoes[qtdTransacoes-1].Data[1] = mes;
    transacoes[qtdTransacoes-1].Data[2] = ano;

    //Captura o horário informado para a transação
    int hora, minuto;
    sscanf(gtk_entry_get_text(GTK_ENTRY(dados->campoHora)), "%d", &hora);
    sscanf(gtk_entry_get_text(GTK_ENTRY(dados->campoMinuto)), "%d", &minuto);
    transacoes[qtdTransacoes-1].Horario[0] = hora;
    transacoes[qtdTransacoes-1].Horario[1] = minuto;

    //Captura o valor gasto na transação
    double valor;
    sscanf(gtk_entry_get_text(GTK_ENTRY(dados->campoGasto)), "%lf", &valor);
    
    //Captura a descrição informada
    strcpy(transacoes[qtdTransacoes-1].descricao, gtk_entry_get_text(GTK_ENTRY(dados->campoDesc)));
    
    
    //Captura o radiobutton ativo (gasto ou receita)
    int btnAtivo = radioBtnAtivo(dados->tipo);

    
    //registra a transação nos struct
    gchar mensagem[MAX]; //msg para o usuário
    gchar saldoAux[MAX]; //msg para armazenar o novo saldo em string
    
    //se for gasto
    if(btnAtivo == 0){
        //imprime o aviso de sucesso
        sprintf(mensagem, "Transação cadastrada com sucesso!\nSeu saldo caiu de %.2f para %.2f", user.Saldo, user.Saldo - valor);
        mensagemAviso(window, mensagem);
        
        //altera o conteúdo da caixa de texto referente ao saldo
        sprintf(saldoAux, "%.2f", user.Saldo - valor);
        gtk_entry_set_text(GTK_ENTRY(campoSaldo), saldoAux);
        
        //altera o saldo no struct do usuário
        user.Saldo = user.Saldo - valor;
        
        //seta o valor da transação para um valor negativo
        transacoes[qtdTransacoes-1].valor = valor*(-1); 
        
        //altera o saldo do usuário no arquivo referente a ele
        redefineSaldo(saldoAux);
    }else{
        //imprime o aviso de sucesso
        sprintf(mensagem, "Transação cadastrada com sucesso!\nSeu saldo subiu de %.2f para %.2f", user.Saldo, user.Saldo + valor);
        mensagemAviso(window, mensagem);
        
        //altera o conteúdo da caixa de texto referente ao saldo
        sprintf(saldoAux, "%.2f", user.Saldo + valor);
        gtk_entry_set_text(GTK_ENTRY(campoSaldo), saldoAux);
        
        //altera o saldo no struct do usuário
        user.Saldo = user.Saldo + valor;
        
        //seta o valor da transação para um valor positivo
        transacoes[qtdTransacoes-1].valor = valor;
        
        //altera o saldo do usuário no arquivo referente a ele
        redefineSaldo(saldoAux);
    }

    
    //Armazena a transação - Alerta ao usuário se o processo obteve sucesso
    armazenaTransacoes();

}


