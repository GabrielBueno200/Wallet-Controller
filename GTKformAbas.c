#include <locale.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <string.h>
#include "funcsCadastro.h"
#include "funcsLogin.h"
#include "funcsGTK.h"
#define MAX 100

//Reúne os dados do formulário de cadastro
struct dadosCad{
    GtkWidget*  janela;
    GtkWidget* campoNome;
    GtkWidget* campoCPF;
    GtkWidget* campoSaldo;
    GtkWidget* campoSenha;
    GtkTextBuffer *buffer;
};

//Reúne os dados do formulário de login
struct dadosLogin{
    GtkWidget*  janela;
    GtkWidget*  campoCPF;
    GtkWidget*  campoSenha;
};


/*==============================================
		VARIÁVEIS GLOBAIS
===============================================*/
struct dadosCad* cadFields;
struct dadosLogin* loginFields;


/*======================================================*/
/*               ABA DE LOGIN DO USUÁRIO*/
/*======================================================*/

void adicionaAbaLogin(GtkWidget* windowForm, 
                      GtkWidget* formAbas, 
                      char* titulo, GtkWidget* ntAba){
    
    //Variáveis para as labels
    GtkWidget *ntAbaLabel, *label_CPF, *label_Senha;
    
    //Variáveis para os campos
    GtkWidget *campoCPF, *campoSenha, *btnLogin;
    
    //struct para armazenar os campos preenchidos durante o cadastro
    struct dadosLogin *loginFields = malloc(sizeof(struct dadosLogin));
    
    //===================================================================================
                                       //ABA
    //===================================================================================  
    
    //Cria uma label para exibir o título da aba
    ntAbaLabel = gtk_label_new (titulo);

   
    //Cria uma aba para a página
    ntAba = gtk_layout_new (NULL, NULL);
    
    
    
    //===================================================================================
                                      //LABELS
    //===================================================================================
    criaLabel(&label_CPF, "Seu CPF:", "ignore", 150, 95, ntAba);
    criaLabel(&label_Senha, "Sua senha:", "ignore", 150, 170, ntAba);
    criaCampo(&campoCPF, 11, 22, "Digite o seu CPF", 100, 120, TRUE, FALSE, NULL, ntAba);
    criaCampo(&campoSenha, 11, 22, "Digite a sua senha", 100, 195, TRUE, FALSE, NULL, ntAba);
    gtk_entry_set_visibility(GTK_ENTRY(campoSenha), FALSE);

    
    //===================================================================================
                              //BOTÕES TRADICIONAIS
    //===================================================================================
    criaBtn(&btnLogin, "Entrar",  145, 250, ntAba);
    
    
    //===================================================================================
                  //EMPACOTAMENTO DOS WIDGETS E CALLBACKS + FUNÇÕES
    //===================================================================================
    //Recolhe os dados de entrada e armazena no struct dados
    loginFields->janela = windowForm;
    loginFields->campoCPF = campoCPF;
    loginFields->campoSenha = campoSenha;

    
    /*Quando o botão for clicado, iniciará o processo de login através da função validaLogin,
      que receberá como parâmetro os campos preenchidos do formulário*/
    g_signal_connect (btnLogin, "clicked", G_CALLBACK (validaLogin), loginFields);
    
    
    
     //Adiciona à janela a aba criada
    gtk_notebook_append_page (GTK_NOTEBOOK (formAbas), ntAba, ntAbaLabel);
} 



/*======================================================*/
/*               ABA DE CADASTRO DO USUÁRIO*/
/*======================================================*/
void adicionaAbaCadastro(GtkWidget* windowForm, 
                         GtkWidget* formAbas, 
                         char* titulo, GtkWidget* ntAba){
    
    //Variáveis para as labels
    GtkWidget *ntAbaLabel, *label_Nome, *label_Saldo, 
              *label_CPF, *label_Categorias, *label_Senha;
    
    //Variáveis para os campos
    GtkWidget *campoNome, *campoSaldo, *campoSenha, 
              *campoCPF, *btnCadastrar,
              *textarea;
    
    GtkTextBuffer *buffer;

    //importa o css
    GtkCssProvider *cssProvider = gtk_css_provider_new();
    importaCss(&cssProvider);
    
    
    //struct para armazenar os campos preenchidos durante o cadastro
    struct dadosCad *cadFields = malloc(sizeof(struct dadosCad));
    
    //===================================================================================
                                       //ABA
    //===================================================================================  
    
    //Cria uma label para exibir o título da aba
    ntAbaLabel = gtk_label_new (titulo);

   
    //Cria uma aba para a página
    ntAba = gtk_layout_new (NULL, NULL);
    
    
    //===================================================================================
                                      //LABELS
    //===================================================================================
    criaLabel(&label_Nome, "Seu Nome:", "ignore", 70, 25, ntAba);
    criaLabel(&label_Categorias, "Suas Categorias\n(separadas por Enter):", "ignore", 200, 25, ntAba);
    criaLabel(&label_Saldo, "Seu Saldo:", "ignore", 75, 95, ntAba);
    criaLabel(&label_CPF, "Seu CPF:", "ignore", 80, 165, ntAba);
    criaLabel(&label_Senha, "Sua senha:", "ignore", 80, 240, ntAba);
    
    
    //===================================================================================
                                      //CAMPOS
    //===================================================================================
    criaCampo(&campoNome, MAX, 22, "Digite o seu nome",30, 50, TRUE, FALSE, NULL, ntAba);
    criaCampo(&campoSaldo, 11, 22, "Digite o seu saldo disponível",30, 120, TRUE, FALSE, NULL, ntAba);
    criaCampo(&campoCPF, 11, 22, "Digite o seu CPF (apenas números)",30, 190, TRUE, FALSE, NULL, ntAba);

    criaCampo(&campoSenha, 11, 22, "Digite a sua senha",30, 265, TRUE, FALSE, NULL, ntAba);
    gtk_entry_set_visibility(GTK_ENTRY(campoSenha), FALSE);

    
    //===================================================================================
                                //TEXTAREA
    //===================================================================================
    //TextArea para que o usuário digite as categorias desejadas
    criaTextArea(&textarea, &buffer, 150, 250, 200, 60,"textarea", TRUE, ntAba);
    
    //===================================================================================
                              //BOTÕES TRADICIONAIS
    //===================================================================================
    //Botão para submeter os dados obtidos no formulário de cadastro
    btnCadastrar= gtk_button_new_with_label("Cadastrar");
    gtk_layout_put(GTK_LAYOUT(ntAba), btnCadastrar, 125, 320);
   
    
    //===================================================================================
                  //EMPACOTAMENTO DOS WIDGETS E CALLBACKS + FUNÇÕES
    //===================================================================================
    
    //Recolhe os dados de entrada e armazena no struct dados
    cadFields->janela = windowForm;
    cadFields->campoNome = campoNome;
    cadFields->campoCPF = campoCPF;
    cadFields->campoSaldo = campoSaldo;
    cadFields->campoSenha = campoSenha;
    cadFields->buffer = buffer;
    
    
    /*Quando o botão for clicado, iniciará o processo de cadastro através da função definePessoa,
      que receberá como parâmetro os campos preenchidos do formulário*/
    g_signal_connect (btnCadastrar, "clicked", G_CALLBACK (definePessoa), cadFields);
    
    
     //Adiciona à janela a aba criada
    gtk_notebook_append_page (GTK_NOTEBOOK (formAbas), ntAba, ntAbaLabel);
} 

