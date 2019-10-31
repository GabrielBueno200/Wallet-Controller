#include <locale.h>
#include <gtk/gtk.h>
#include <string.h>
#include "funcsCadastro.h"
#include "funcsLogin.h"
#define MAX 100

struct dadosCad{
    GtkWidget*  janela;
    GtkWidget* campoNome;
    GtkWidget* campoCPF;
    GtkWidget* campoSaldo;
    GtkWidget* campoSenha;
    GtkTextBuffer *buffer;
};

struct dadosLogin{
    GtkWidget*  janela;
    GtkWidget*  campoCPF;
    GtkWidget*  campoSenha;
};

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
    struct dadosLogin *dados = malloc(sizeof(struct dadosLogin));
    
    //Cria uma label para exibir o título da aba
    ntAbaLabel = gtk_label_new (titulo);

   
    //Cria uma aba para a página
    ntAba = gtk_layout_new (NULL, NULL);
    
    //Label para o CPF do usuário
    label_CPF = gtk_label_new ("Seu CPF:");
    gtk_layout_put(GTK_LAYOUT(ntAba), label_CPF, 150, 95);
    
    //Label para a senha do usuário
    label_Senha = gtk_label_new ("Sua senha:");
    gtk_layout_put(GTK_LAYOUT(ntAba), label_Senha, 150, 170);
    
    //Campo para o CPF do usuário
    campoCPF = gtk_entry_new();
    gtk_entry_set_max_length(GTK_ENTRY(campoCPF), 11);
    gtk_entry_set_width_chars(GTK_ENTRY(campoCPF), 22);
    gtk_entry_set_placeholder_text (GTK_ENTRY(campoCPF), "Digite o seu CPF");
    gtk_layout_put(GTK_LAYOUT(ntAba), campoCPF, 100, 120);
    
    //Campo para a senha do usuário
    campoSenha = gtk_entry_new();
    gtk_entry_set_max_length(GTK_ENTRY(campoSenha), 11);
    gtk_entry_set_width_chars(GTK_ENTRY(campoSenha), 22);
    gtk_entry_set_visibility(GTK_ENTRY(campoSenha), FALSE);
    gtk_entry_set_placeholder_text (GTK_ENTRY(campoSenha), "Digite a sua senha");
    gtk_layout_put(GTK_LAYOUT(ntAba), campoSenha, 100, 195);
    
    
    //Botão paa enviar os dados do formulário
    btnLogin = gtk_button_new_with_label("Entrar");
    gtk_layout_put(GTK_LAYOUT(ntAba), btnLogin, 145, 250);
    
    
    //Recolhe os dados de entrada e armazena no struct dados
    dados->janela = windowForm;
    dados->campoCPF = campoCPF;
    dados->campoSenha = campoSenha;

    
    /*Quando o botão for clicado, iniciará o processo de login através da função validaLogin,
      que receberá como parâmetro os campos preenchidos do formulário*/
    g_signal_connect (btnLogin, "clicked", G_CALLBACK (validaLogin), dados);
    
    
    
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
              *scrolledwindow, *textarea;
    
    GtkTextBuffer *buffer;
    GtkTextIter iter;
    
    //variável para incluir o arquivo de estilização do programa (estilo.css)
    GtkCssProvider *cssProvider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(cssProvider, "estilo.css", NULL);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
                               GTK_STYLE_PROVIDER(cssProvider),
                               GTK_STYLE_PROVIDER_PRIORITY_USER);
    
    //struct para armazenar os campos preenchidos durante o cadastro
    struct dadosCad *dados = malloc(sizeof(struct dadosCad));
    
    //Cria uma label para exibir o título da aba
    ntAbaLabel = gtk_label_new (titulo);

   
    //Cria uma aba para a página
    ntAba = gtk_layout_new (NULL, NULL);
    
    //Label para o nome do usuário
    label_Nome = gtk_label_new ("Seu Nome:");
    gtk_layout_put(GTK_LAYOUT(ntAba), label_Nome, 70, 25);

    //Label para o nome do usuário
    label_Categorias = gtk_label_new ("Suas Categorias:");
    gtk_layout_put(GTK_LAYOUT(ntAba), label_Categorias, 200, 25);
    
    //Label para o saldo do usuário
    label_Saldo = gtk_label_new ("Seu Saldo:");
    gtk_layout_put(GTK_LAYOUT(ntAba), label_Saldo, 75, 95);
    
    //Label para o CPF do usuário
    label_CPF = gtk_label_new ("Seu CPF:");
    gtk_layout_put(GTK_LAYOUT(ntAba), label_CPF, 80, 165);
    
    //Label para a senha do usuário
    label_Senha = gtk_label_new ("Sua senha:");
    gtk_layout_put(GTK_LAYOUT(ntAba), label_Senha, 80, 240);
    
    
    //Input para o nome do usuário
    campoNome = gtk_entry_new();
    gtk_entry_set_max_length(GTK_ENTRY(campoNome), 11);
    gtk_entry_set_width_chars(GTK_ENTRY(campoNome), 22);
    gtk_entry_set_placeholder_text (GTK_ENTRY(campoNome), "Digite o seu nome");
    gtk_layout_put(GTK_LAYOUT(ntAba), campoNome, 30, 50);
    
    //Input para o saldo do usuário
    campoSaldo = gtk_entry_new();
    gtk_entry_set_max_length(GTK_ENTRY(campoSaldo), 11);
    gtk_entry_set_width_chars(GTK_ENTRY(campoSaldo), 22);
    gtk_entry_set_placeholder_text (GTK_ENTRY(campoSaldo), "Digite o seu saldo disponível");
    gtk_layout_put(GTK_LAYOUT(ntAba), campoSaldo, 30, 120);
    
    //Input para o CPF do usuário
    campoCPF = gtk_entry_new();
    gtk_entry_set_max_length(GTK_ENTRY(campoCPF), 11);
    gtk_entry_set_width_chars(GTK_ENTRY(campoCPF), 22);
    gtk_entry_set_placeholder_text (GTK_ENTRY(campoCPF), "Digite o seu CPF (apenas números)");
    gtk_layout_put(GTK_LAYOUT(ntAba), campoCPF, 30, 190);
    
    //Input para a senha do usuário
    campoSenha = gtk_entry_new();
    gtk_entry_set_max_length(GTK_ENTRY(campoSenha), 11);
    gtk_entry_set_width_chars(GTK_ENTRY(campoSenha), 22);
    gtk_entry_set_visibility(GTK_ENTRY(campoSenha), FALSE);
    gtk_entry_set_placeholder_text (GTK_ENTRY(campoSenha), "Digite a sua senha");
    gtk_layout_put(GTK_LAYOUT(ntAba), campoSenha, 30, 265);
    
    
    //TextArea para que o usuário digite as categorias desejadas
    textarea = gtk_text_view_new();
    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textarea));
    
    /*gtk_text_buffer_get_iter_at_offset(buffer, &iter, 0);
    gtk_text_buffer_insert(buffer, &iter, "Categoria 1\n", -1);
    gtk_text_buffer_get_iter_at_offset(buffer, &iter, 0);
    gtk_text_buffer_insert(buffer, &iter, "Categoria 2\n", -1);
    gtk_text_buffer_get_iter_at_offset(buffer, &iter, 0);
    gtk_text_buffer_insert(buffer, &iter, "Categoria 3\n", -1);*/
    
    //Torna o TextArea editável
    gtk_text_view_set_editable(GTK_TEXT_VIEW (textarea), TRUE);
     
    
    
    //Insere o TextArea em uma janela rolável
    scrolledwindow = gtk_scrolled_window_new(NULL, NULL); 
    
    
    //Adiciona o TextArea à janela rolável
     gtk_container_add(GTK_CONTAINER(scrolledwindow), textarea);
    
    //Seta as dimensões da janela rolável
     gtk_widget_set_size_request(scrolledwindow, 150, 250);
    
    //Adiciona uma classe à janela rolável
    gtk_style_context_add_class(gtk_widget_get_style_context(scrolledwindow),"textarea");
     
    //Adiciona a janela rolável à janela principal
    gtk_layout_put(GTK_LAYOUT(ntAba), scrolledwindow, 200, 50);
     
     
   
    //Botão para submeter os dados obtidos no formulário de cadastro
    btnCadastrar= gtk_button_new_with_label("Cadastrar");
    gtk_layout_put(GTK_LAYOUT(ntAba), btnCadastrar, 125, 320);
   
    

    //Recolhe os dados de entrada e armazena no struct dados
    dados->janela = windowForm;
    dados->campoNome = campoNome;
    dados->campoCPF = campoCPF;
    dados->campoSaldo = campoSaldo;
    dados->campoSenha = campoSenha;
    dados->buffer = buffer;
    
    /*Quando o botão for clicado, iniciará o processo de cadastro através da função definePessoa,
      que receberá como parâmetro os campos preenchidos do formulário*/
    g_signal_connect (btnCadastrar, "clicked", G_CALLBACK (definePessoa), dados);
    
    
    
     //Adiciona à janela a aba criada
    gtk_notebook_append_page (GTK_NOTEBOOK (formAbas), ntAba, ntAbaLabel);
} 

