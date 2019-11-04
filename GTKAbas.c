#include <locale.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <string.h>
#include "funcsLogin.h"
#include "funcsTransacoes.h"
#include "funcsCategorias.h"
#include "funcsUser.h"
#include "funcsGTK.h"
#define ENTRADA 100
#define LARGURA 40

/*======================================================================================================================
		                                 VARIÁVEIS GLOBAIS
=======================================================================================================================*/
//de outros arquivos
extern struct usuarioLogado user; // captura os dados do usuário loga
extern int qtdFinal; //captura a quantidade de categorias cadastradas pelo usuário

//deste arquivo
struct Transacao* transacoes; /* será utilizada como array dinâmico (malloc), e reunirá todas as transações feitas pelo 
								 usuário durante a execução do programa*/
int qtdTransacoes = 0; //captura a quantidade de transacoes cadastradas pelo usuário. Inicialmente zerada.

GtkWidget *comboboxTransacoes;
GtkWidget *comboboxCategorias;
GtkWidget *comboboxConta;
GtkWidget *campoSaldo, *campoNome, *campoCPF;


//struct para envios dos dados alterados pelo usuário (informações pessoais + categiruas + relatório)
struct camposUser{
  GtkTextBuffer* buffer;
  GtkTextIter iter;
  GtkWidget* label_Transacoes;
  GtkWidget* campoDialog;
};

//struct para envios dos dados sobre a transação cadastrada
struct camposTransacao{
    GtkWidget* campoDia; 
    GtkWidget* campoMes; 
    GtkWidget* campoAno; 
    GtkWidget* campoMinuto; 
    GtkWidget* campoHora;
    GtkWidget* campoGasto;
    GtkWidget* campoDesc;
    GtkWidget* tipo;
};

/*Struct de transações, armazenará cada transação
feita pelo usuário durante a execução do programa*/
struct Transacao{
    int Data[3];
    int Horario[2];
    double valor;
    char descricao[MAX];
    struct CategoriasUsuario* Categoria; //Ponteiro, para atualizar simultaneamente todas as categorias dos structs de transação 
    									 //Caso seja feita alguma modificação no struct de CategoriasUsuario
};

struct camposUser* malloc_Categoria;
struct camposTransacao* malloc_Transacao;
struct camposUser* malloc_User;





//Retorna a aba Sobre Nós
GtkWidget *AdicionaAbaSobreNos (GtkWidget* window_Abas, char *titulo, GtkWidget* ntAba){
    
    //VARIÁVEIS PARA OS LABELS
    GtkWidget *tituloAba, *label_Desenvolvedores, *label_Nomes, *bg;
    
    //===================================================================================
                                       //ABA
    //=================================================================================== 
    //Cria uma label para exibir o título da aba
    tituloAba = gtk_label_new (titulo);

    //Cria uma aba para a página
    ntAba = gtk_layout_new (NULL, NULL);
    
    //Fundo da aba
    bg = gtk_label_new(NULL);
    gtk_widget_set_size_request(bg, 1000, 1000);
    gtk_layout_put(GTK_LAYOUT(ntAba), bg, 0, 0);
    gtk_style_context_add_class(gtk_widget_get_style_context(bg),"bgSobreNos");
    
    //===================================================================================
                                      //LABELS
    //===================================================================================
    criaLabel(&label_Desenvolvedores, "Desenvolvido por:", "labelDesenvolvedores", 170, 160, ntAba);

    
    criaLabel(&label_Nomes, "Gabriel Bueno - RA: 22.119.077-0\n"
                                "Thiago Soares - RA: 22.119.044-0", 
                                "labelNomes", 100, 210, ntAba);

    //Adiciona à janela a aba criada
    gtk_notebook_append_page (GTK_NOTEBOOK (window_Abas), ntAba, tituloAba);
    

    return (ntAba);
}





//Retorna a aba de categorias
GtkWidget *AdicionaAbaCategorias (GtkWidget* window_Abas, char *titulo, GtkWidget* ntAba){
    
    //VARIÁVEIS PARA OS LABELS
    GtkWidget *tituloAba, *label_Categoria, *bg;
    
    //VARIÁVEIS PARA OS BOTÕES
    GtkWidget *btnAdiciona, *btnRenomeia, *btnRemove;
    
    
    //malloc de struct camposUser  para envio dos dados para os callbacks
    malloc_Categoria = malloc(sizeof(struct camposUser));

    //===================================================================================
                                       //ABA
    //===================================================================================  

    //Cria uma label para exibir o título da aba
    tituloAba = gtk_label_new (titulo);

   
    //Cria uma aba para a página
    ntAba = gtk_layout_new (NULL, NULL);
    
    //Fundo da aba
    bg = gtk_label_new(NULL);
    gtk_widget_set_size_request(bg, 600, 500);
    gtk_layout_put(GTK_LAYOUT(ntAba), bg ,0, 0);
    gtk_style_context_add_class(gtk_widget_get_style_context(bg),"bgTransacoes");
    
    //===================================================================================
                                      //LABELS
    //===================================================================================
    criaLabel(&label_Categoria, "Categoria:", "labelCategoria", 80, 140, ntAba);
    
    
    //===================================================================================
                                    //COMBOBOX
    //===================================================================================
    criaCombobox(&comboboxCategorias, ntAba, 200, 140);
    
    //===================================================================================
                              //BOTÕES TRADICIONAIS
    //===================================================================================
    criaBtn(&btnAdiciona, "Adicionar categoria",  80, 200, ntAba);
    criaBtn(&btnRenomeia, "Renomear categoria",  240, 200, ntAba);
    criaBtn(&btnRemove, "Excluir categoria",  405, 200, ntAba);
    
    
    //===================================================================================
                  //EMPACOTAMENTO DOS WIDGETS E CALLBACKS + FUNÇÕES
    //===================================================================================
    
    //CALLBACKS
    g_signal_connect (btnRenomeia, "clicked", G_CALLBACK (renomearCategoria), malloc_Categoria);
    g_signal_connect (btnRemove, "clicked", G_CALLBACK (confirmaDelCategoria), malloc_Categoria);
    g_signal_connect (btnAdiciona, "clicked", G_CALLBACK (addNomeCategoria), malloc_Categoria);

    
    //Adiciona à janela a aba criada
    gtk_notebook_append_page (GTK_NOTEBOOK (window_Abas), ntAba, tituloAba);
    

    return (ntAba);
}



//Retorna a aba de transações
GtkWidget *AdicionaAbaTransacoes (GtkWidget* window_Abas, char *titulo, GtkWidget* ntAba){
    
    //VARIÁVEIS PARA AS LABELS
    GtkWidget *tituloAba, *label_Data, *label_Hora, 
            *label_Categoria, *label_Gasto, *label_inf, 
            *label_Tipo, label_Desc, *bg;
    
    //VARIÁVEIS PARA OS CAMPOS
    GtkWidget *campoDia, *campoMes, *campoAno, 
              *campoMinuto, *campoHora, *campoGasto,
              *campoDesc;
    
    //VARIÁVEIS PARA OS BOTÕES
    GtkWidget *btnCadastra;
    GtkWidget  *radioBtnGasto, *radioBtnReceita;
    
    /*Faz com que a variável global de transações receba um array dinâmico 
    ( armazenando inicialmente o equivalente a uma transação (1 struct de transação) )*/
    transacoes = malloc(sizeof(struct Transacao));
    
    
    //malloc de struct camposTransacao para envio dos dados para oscallbacks
    malloc_Transacao = malloc(sizeof(struct camposTransacao));
    
    //Passa as transações do arquivo do usuário para o array de struct dinâmico de transações
    atualizaTransacoes();
    
    
    //===================================================================================
                                       //ABA
    //===================================================================================
    //Cria uma label para exibir o título da aba
    tituloAba = gtk_label_new (titulo);

   
    //Cria uma aba para a página
    ntAba = gtk_layout_new (NULL, NULL);
    
    
    //Cria um fundo para a aba
    bg = gtk_label_new(NULL);
    gtk_widget_set_size_request(bg, 600, 500);
    gtk_layout_put(GTK_LAYOUT(ntAba), bg, 0, 0);
    gtk_style_context_add_class(gtk_widget_get_style_context(bg),"bgTransacoes");
    
    
    //===================================================================================
                                      //LABELS
    //===================================================================================
    criaLabel(&label_Data, "Data:               /             /", "labelData", 66, 125, ntAba);
    criaLabel(&label_Hora, "Hora:                   :            ", "labelData", 360, 125, ntAba);
    criaLabel(&label_Categoria, "Categoria:", "labelData", 120, 170, ntAba);
    criaLabel(&label_Tipo, "Tipo:", "labelData", 165, 205, ntAba);
    criaLabel(&label_Gasto, "Valor R$:", "labelData", 70, 245, ntAba);
    criaLabel(&label_Desc, "Descrição:", "labelData", 270, 245, ntAba);
    
    //===================================================================================
                                      //CAMPOS
    //===================================================================================
    criaCampo(&campoDia, 2, 4, "dd", 115, 120, TRUE, FALSE, NULL, ntAba);
    criaCampo(&campoMes, 2, 4, "mm", 170, 120, TRUE, FALSE, NULL, ntAba);
    criaCampo(&campoAno, 4, 8, "aaaa", 230, 120, TRUE, FALSE, NULL, ntAba);
    criaCampo(&campoHora, 2, 4, "hh", 415, 120, TRUE, FALSE, NULL, ntAba);
    criaCampo(&campoMinuto, 2, 4, "mm", 490, 120, TRUE, FALSE, NULL, ntAba);
    criaCampo(&campoGasto, 12, 12, "0000,00", 140, 240, TRUE, FALSE, NULL, ntAba);
    criaCampo(&campoDesc, MAX, 24, "Descreva a transação", 350, 240, TRUE, FALSE, NULL, ntAba);
    
    
    //===================================================================================
                          //RADIO BUTTONS E BOTÕES TRADICIONAIS
    //===================================================================================
    radioBtnGasto = gtk_radio_button_new_with_label (NULL, "Gasto");
    gtk_style_context_add_class(gtk_widget_get_style_context(radioBtnGasto),"labelData");
    gtk_layout_put(GTK_LAYOUT(ntAba), radioBtnGasto, 220, 205);
    
    radioBtnReceita = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(radioBtnGasto),"Receita");
    gtk_style_context_add_class(gtk_widget_get_style_context(radioBtnReceita),"labelData");
    gtk_layout_put(GTK_LAYOUT(ntAba), radioBtnReceita, 330, 205);

    criaBtn(&btnCadastra, "Cadastrar transação",  230, 285, ntAba);
    
    
    //===================================================================================
                                    //COMBOBOX
    //===================================================================================
    criaCombobox(&comboboxTransacoes, ntAba, 200, 165);

    
    //===================================================================================
                  //EMPACOTAMENTO DOS WIDGETS E CALLBACKS + FUNÇÕES
    //===================================================================================
    
    //Prepara os campos para serem enviados às funções
    malloc_Transacao->campoDia = campoDia;
    malloc_Transacao->campoMes = campoMes;
    malloc_Transacao->campoAno = campoAno;
    malloc_Transacao->campoMinuto = campoMinuto;
    malloc_Transacao->campoHora = campoHora;
    malloc_Transacao->campoGasto = campoGasto;
    malloc_Transacao->campoDesc = campoDesc;
    malloc_Transacao->tipo = radioBtnGasto; 
    

    //CALLBACK para o click no botão
    g_signal_connect (btnCadastra, "clicked", G_CALLBACK (cadastraTransacao), malloc_Transacao);

    
    //Adiciona à janela a aba criada
    gtk_notebook_append_page (GTK_NOTEBOOK (window_Abas), ntAba, tituloAba);
    

    return (ntAba);
}


//Função executada para quando o item selecionado no combobox for alterado
void mudaCategoria(GtkComboBox *comboboxConta, struct camposUser* dados){
    
    //variável para substituir o texto da label de categorias
    gchar labelTransacoesText[MAX];
    
    //verifica se a categoria ativa no combobox é difente da primeira posição (se for, não há categoria selecionada
    if(gtk_combo_box_get_active(comboboxConta) != 0){
        
        //Altera a label de categorias
        sprintf(labelTransacoesText, "Transações em \n '%s'",gtk_combo_box_text_get_active_text (GTK_COMBO_BOX_TEXT(comboboxConta)));
        gtk_label_set_text(GTK_LABEL(dados->label_Transacoes), labelTransacoesText);
        
        //imprime as transações da categoria selecionada no textarea
        imprimeTransacoes(gtk_combo_box_get_active(GTK_COMBO_BOX(comboboxConta)), dados);
    } else{
        gtk_label_set_text(GTK_LABEL(dados->label_Transacoes), "Selecione\numa categoria:");
    }
    
}



//Adiciona abas à janela criada
GtkWidget *AdicionaAbaUsuario (GtkWidget* window_Abas, char *titulo, GtkWidget* ntAba){
    
    //Variáveis para as labels
    GtkWidget *tituloAba;
    GtkWidget *label_Nome, *label_CPF, *label_Saldo, 
               *label_Categorias, *label_Transacoes, *bg;
    
    //Variáveis para os campos
    GtkWidget *textarea;
    
    //Variáveis para o textarea
    GtkTextBuffer *bufferTA;
    GtkTextIter iterTA;
    
    //Variáveis para os botões
    GtkWidget *btnNome, *btnRMensal, 
              *btnRAnual, *btnLogout, *btnDelAcc;
    
    //malloc de struct camposUser  para envio dos dados para os callbacks
    malloc_User = malloc(sizeof(struct camposUser));
    
    //define o nome dos meses pelos seus índices (vetor de meses0
    defineMeses();
    
    
    //===================================================================================
                                       //ABA
    //===================================================================================

    //Cria uma label para exibir o título da aba
    tituloAba = gtk_label_new (titulo);

   
    //Cria uma aba para a página
    ntAba = gtk_layout_new (NULL, NULL);
    
    
    //Label para o fundo da aba
    bg = gtk_label_new(NULL);
    gtk_style_context_add_class(gtk_widget_get_style_context(bg),"bgUser");
    gtk_widget_set_size_request(bg, 600, 500);
    gtk_layout_put(GTK_LAYOUT(ntAba), bg, 0, 0);
    
    
    //===================================================================================
                                    //COMBOBOX
    //===================================================================================
    criaCombobox(&comboboxConta, ntAba, 220, 170);

    
    //===================================================================================
                                      //LABELS
    //===================================================================================

    criaLabel(&label_Nome, "Seu Nome:", "labelUser", 120, 25, ntAba);
    criaLabel(&label_CPF, "Seu CPF:", "labelUser", 135, 65, ntAba);
    criaLabel(&label_Saldo, "Saldo Disponível: R$", "labelUser", 60, 105, ntAba);
    criaLabel(&label_Categorias, "Categorias Cadastradas:", "labelUser", 40, 155, ntAba);

    criaLabel(&label_Transacoes, "Selecione\numa categoria:", "labelUser", 100, 200, ntAba);
    gtk_label_set_justify (GTK_LABEL(label_Transacoes),GTK_JUSTIFY_RIGHT);

    
    //===================================================================================
                                      //CAMPOS
    //===================================================================================

    criaCampo(&campoNome, ENTRADA, LARGURA, "Seu nome", 220, 40, FALSE, TRUE, user.Nome, ntAba);
    gtk_style_context_add_class(gtk_widget_get_style_context(campoNome),"inputUser");
    

    criaCampo(&campoCPF, ENTRADA, LARGURA+9, "Seu CPF", 220, 80, FALSE, TRUE, user.CPF, ntAba);
    gtk_style_context_add_class(gtk_widget_get_style_context(campoCPF),"inputUser");
    

    gchar saldoAux[MAX]; //transformação do saldo (de double para string)
    sprintf(saldoAux, "%.2lf", user.Saldo);
    criaCampo(&campoSaldo, ENTRADA, LARGURA+9, "Seu saldo", 220, 120, FALSE, TRUE, saldoAux, ntAba);
    gtk_style_context_add_class(gtk_widget_get_style_context(campoSaldo),"inputUser");
    
    
    //===================================================================================
                              //BOTÕES TRADICIONAIS
    //===================================================================================
    //Botão para modificar o nome do usuário
    criaBtn(&btnNome, NULL,  490, 40, ntAba);
    gtk_style_context_add_class(gtk_widget_get_style_context(btnNome),"btnEdita");
    
    
    //Botão para exibir o relatório mensal de transações
    criaBtn(&btnRMensal, "Relatório Mensal",  65, 270, ntAba);

    
    //Botão para exibir o relatório anual de transações
    criaBtn(&btnRAnual, "Relatório Anual",  69, 310, ntAba);
    
    //Botão para o logout do usuário
    criaBtn(&btnLogout, "Sair",  100, 350, ntAba);
    gtk_style_context_add_class(gtk_widget_get_style_context(btnLogout),"btnLogout");
    
    //Botão para exclusão de conta
    criaBtn(&btnDelAcc, "Excluir a minha conta",  48, 395, ntAba);
    gtk_style_context_add_class(gtk_widget_get_style_context(btnDelAcc),"btnDelAcc");
    
    
    //===================================================================================
                                //TEXTAREA
    //===================================================================================
    criaTextArea(&textarea, &bufferTA, 320, 212, 220, 220,"textarea", FALSE, ntAba);
    
    
    
    //===================================================================================
                  //EMPACOTAMENTO DOS WIDGETS E CALLBACKS + FUNÇÕES
    //===================================================================================
    
    //Prepara os campos para serem enviados às funções
    malloc_User->buffer = bufferTA;
    malloc_User->iter = iterTA;
    malloc_User->label_Transacoes = label_Transacoes;
    
    //CALLBACKS
    g_signal_connect (comboboxConta, "changed", G_CALLBACK (mudaCategoria), malloc_User);
    g_signal_connect (btnNome, "clicked", G_CALLBACK (mudarNome), NULL); 
    g_signal_connect (btnDelAcc, "clicked", G_CALLBACK (confirmaDelAcc), NULL); 
    g_signal_connect (btnRMensal, "clicked", G_CALLBACK (RelatorioMensal), malloc_User); 
    g_signal_connect (btnRAnual, "clicked", G_CALLBACK (RelatorioAnual), malloc_User); 
    g_signal_connect (btnLogout, "clicked", G_CALLBACK (logout), NULL);
    

    //Adiciona à janela a aba criada
    gtk_notebook_append_page (GTK_NOTEBOOK (window_Abas), ntAba, tituloAba);
    
    
    return (ntAba);
}

