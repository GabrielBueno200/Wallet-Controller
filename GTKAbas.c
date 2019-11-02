#include <locale.h>
#include <gtk/gtk.h>
#include <string.h>
#include "funcsLogin.h"
#include "funcsTransacoes.h"
#include "funcsCategorias.h"
#include "funcsUser.h"

extern struct usuarioLogado user; // captura os dados do usuário loga
extern int qtdFinal; //captura a quantidade de categorias cadastradas pelo usuário

struct Transacao* transacoes; /* será utilizada como array dinâmico (malloc), e reunirá todas as transações feitas pelo 
								 usuário durante a execução do programa*/
int qtdTransacoes = 0; //captura a quantidade de transacoes cadastradas pelo usuário. Inicialmente zerada.

GtkWidget *comboboxTransacoes;
GtkWidget *comboboxCategorias;
GtkWidget *comboboxConta;
GtkWidget *campoSaldo, *campoNome, *campoCPF;

#define ENTRADA 100
#define LARGURA 40

struct camposUser{
  GtkTextBuffer* buffer;
  GtkTextIter iter;
  GtkWidget* label_Transacoes;
  GtkWidget* campoDialog;
};

struct camposTransacao{
    GtkWidget* campoDia; 
    GtkWidget* campoMes; 
    GtkWidget* campoAno; 
    GtkWidget* campoMinuto; 
    GtkWidget* campoHora;
    GtkWidget* campoGasto;
    GtkWidget* tipo;
};


/*Struct de transações, armazenará cada transação
feita pelo usuário durante a execução do programa*/
struct Transacao{
    int Data[3];
    int Horario[2];
    double valor;
    struct CategoriasUsuario* Categoria; //Ponteiro, para atualizar simultaneamente todas as categorias dos structs de transação 
    									 //Caso seja feita alguma modificação no struct de CategoriasUsuario
};


//Adiciona abas à janela criada
GtkWidget *AdicionaAbaSobreNos (GtkWidget* window_Abas, char *titulo, GtkWidget* ntAba){
    GtkWidget *tituloAba, *label_Desenvolvedores, *label_Nomes, *bg;
    
    GtkCssProvider *cssProvider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(cssProvider, "estilo.css", NULL);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
                               GTK_STYLE_PROVIDER(cssProvider),
                               GTK_STYLE_PROVIDER_PRIORITY_USER);
    

    //Cria uma label para exibir o título da aba
    tituloAba = gtk_label_new (titulo);

   
    //Cria uma aba para a página
    ntAba = gtk_layout_new (NULL, NULL);
    
    
    bg = gtk_label_new(NULL);
    gtk_widget_set_size_request(bg, 1000, 1000);
    gtk_layout_put(GTK_LAYOUT(ntAba), bg, 0, 0);
    gtk_style_context_add_class(gtk_widget_get_style_context(bg),"bgSobreNos");
    
    
    label_Desenvolvedores = gtk_label_new("Desenvolvido por:");
    gtk_layout_put(GTK_LAYOUT(ntAba), label_Desenvolvedores, 170, 160);
    gtk_style_context_add_class(gtk_widget_get_style_context(label_Desenvolvedores),"labelDesenvolvedores");
    
    
    label_Nomes = gtk_label_new("Gabriel Bueno - RA: 22.119.077-0\n"
                                "Thiago Soares - RA: 22.119.XXX-X");
    gtk_layout_put(GTK_LAYOUT(ntAba), label_Nomes , 100, 210);
    gtk_style_context_add_class(gtk_widget_get_style_context(label_Nomes),"labelNomes");
    
  
    
    //Adiciona à janela a aba criada
    gtk_notebook_append_page (GTK_NOTEBOOK (window_Abas), ntAba, tituloAba);
    

    return (ntAba);
}









//Adiciona abas à janela criada
GtkWidget *AdicionaAbaCategorias (GtkWidget* window_Abas, char *titulo, GtkWidget* ntAba, GtkWidget* window){
    GtkWidget *tituloAba, *label_Categoria, *bg;
    GtkWidget *btnAdiciona, *btnRenomeia, *btnRemove;
    

        
    GtkCssProvider *cssProvider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(cssProvider, "estilo.css", NULL);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
                               GTK_STYLE_PROVIDER(cssProvider),
                               GTK_STYLE_PROVIDER_PRIORITY_USER);
    
    
    struct camposUser* dados = malloc(sizeof(struct camposUser));

  

    //Cria uma label para exibir o título da aba
    tituloAba = gtk_label_new (titulo);

   
    //Cria uma aba para a página
    ntAba = gtk_layout_new (NULL, NULL);
    
    //Fundo da aba
    bg = gtk_label_new(NULL);
    gtk_widget_set_size_request(bg, 600, 500);
    gtk_layout_put(GTK_LAYOUT(ntAba), bg ,0, 0);
    gtk_style_context_add_class(gtk_widget_get_style_context(bg),"bgTransacoes");
    

    //label para a categoria
    label_Categoria = gtk_label_new("Categoria:");
    gtk_style_context_add_class(gtk_widget_get_style_context(label_Categoria),"labelData");
    gtk_layout_put(GTK_LAYOUT(ntAba), label_Categoria , 80, 140);
    
    
    

     /* Cria o combobox e adiciona as categorias a ele*/
    comboboxCategorias = gtk_combo_box_text_new ();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT (comboboxCategorias), "Clique aqui e veja as categorias cadastradas por você");
            
    int i;
    for (i = 0; i < qtdFinal; i++){
      gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (comboboxCategorias), user.Categoria[i].Nome);
    }
    
    gtk_combo_box_set_active (GTK_COMBO_BOX (comboboxCategorias), 0);
    
    gtk_layout_put(GTK_LAYOUT(ntAba), comboboxCategorias, 220, 170);
    
    gtk_layout_put(GTK_LAYOUT(ntAba), comboboxCategorias, 200, 140);
    
    
    
    
    btnAdiciona = gtk_button_new_with_label("Adicionar categoria");
    gtk_layout_put(GTK_LAYOUT(ntAba), btnAdiciona , 80, 200);
    
    btnRenomeia = gtk_button_new_with_label("Renomear categoria");
    gtk_layout_put(GTK_LAYOUT(ntAba), btnRenomeia , 240, 200);
    
    btnRemove = gtk_button_new_with_label("Excluir categoria");
    gtk_layout_put(GTK_LAYOUT(ntAba), btnRemove , 405, 200);
    
    
    
    g_signal_connect (btnRenomeia, "clicked", G_CALLBACK (renomearCategoria), dados);
    g_signal_connect (btnRemove, "clicked", G_CALLBACK (confirmaDelCategoria), dados);
    g_signal_connect (btnAdiciona, "clicked", G_CALLBACK (addNomeCategoria), dados);

    
    //Adiciona à janela a aba criada
    gtk_notebook_append_page (GTK_NOTEBOOK (window_Abas), ntAba, tituloAba);
    

    return (ntAba);
}












//Adiciona abas à janela criada
GtkWidget *AdicionaAbaTransacoes (GtkWidget* window_Abas, char *titulo, GtkWidget* ntAba, GtkWidget* window){
    GtkWidget *tituloAba, *label_Data, *label_Hora, *label_Categoria, *label_Gasto, *label_inf, *label_Tipo, *bg;
    
    GtkWidget *campoDia, *campoMes, *campoAno, *campoMinuto, *campoHora, *campoGasto;
    GtkWidget *btnCadastra;
    
    GtkWidget  *radioBtnGasto, *radioBtnReceita;
    
    gint btnAtivo;
    
    struct camposTransacao* dados = malloc(sizeof(struct camposTransacao));

    
    GtkCssProvider *cssProvider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(cssProvider, "estilo.css", NULL);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
                               GTK_STYLE_PROVIDER(cssProvider),
                               GTK_STYLE_PROVIDER_PRIORITY_USER);
    
    /*Faz com que a variável global de transações receba um array dinâmico 
    ( armazenando inicialmente o equivalente a uma transação (1 struct de transação) )*/
    transacoes = malloc(sizeof(struct Transacao));
    
    atualizaTransacoes();
    

    //Cria uma label para exibir o título da aba
    tituloAba = gtk_label_new (titulo);

   
    //Cria uma aba para a página
    ntAba = gtk_layout_new (NULL, NULL);
    
    bg = gtk_label_new(NULL);
    gtk_widget_set_size_request(bg, 600, 500);
    gtk_layout_put(GTK_LAYOUT(ntAba), bg ,0, 0);
    gtk_style_context_add_class(gtk_widget_get_style_context(bg),"bgTransacoes");
    
    
    
    label_Data = gtk_label_new("Data:           /          /");
    gtk_style_context_add_class(gtk_widget_get_style_context(label_Data),"labelData");
    gtk_layout_put(GTK_LAYOUT(ntAba), label_Data , 60, 120);
    
    label_Hora = gtk_label_new("Hora:             :            ");
    gtk_style_context_add_class(gtk_widget_get_style_context(label_Hora),"labelData");
    gtk_layout_put(GTK_LAYOUT(ntAba), label_Hora , 360, 120);
    
    label_Categoria = gtk_label_new("Categoria:");
    gtk_style_context_add_class(gtk_widget_get_style_context(label_Categoria),"labelData");
    gtk_layout_put(GTK_LAYOUT(ntAba), label_Categoria , 80, 165);
    
    label_Tipo = gtk_label_new("Tipo:");
    gtk_style_context_add_class(gtk_widget_get_style_context(label_Tipo),"labelData");
    gtk_layout_put(GTK_LAYOUT(ntAba), label_Tipo , 140, 205);
    
    label_Gasto = gtk_label_new("Valor R$:");
    gtk_style_context_add_class(gtk_widget_get_style_context(label_Gasto),"labelData");
    gtk_layout_put(GTK_LAYOUT(ntAba), label_Gasto , 180, 240);
    
    bg = gtk_label_new(NULL);
    gtk_widget_set_size_request(bg, 600, 500);
    gtk_layout_put(GTK_LAYOUT(ntAba), bg, 0, 0);
    
    
    radioBtnGasto = gtk_radio_button_new_with_label (NULL, "Gasto");
    gtk_style_context_add_class(gtk_widget_get_style_context(radioBtnGasto),"labelData");
    gtk_layout_put(GTK_LAYOUT(ntAba), radioBtnGasto, 220, 202);
    
   
    radioBtnReceita = gtk_radio_button_new_with_label_from_widget(radioBtnGasto,"Receita");
    gtk_style_context_add_class(gtk_widget_get_style_context(radioBtnReceita),"labelData");
    gtk_layout_put(GTK_LAYOUT(ntAba), radioBtnReceita, 330, 202);
    
    
    

    
    campoDia = gtk_entry_new();
    gtk_entry_set_max_length(GTK_ENTRY(campoDia ), 2);
    gtk_entry_set_width_chars(GTK_ENTRY(campoDia), 4);
    gtk_entry_set_placeholder_text (GTK_ENTRY(campoDia), "dd");
    gtk_layout_put(GTK_LAYOUT(ntAba), campoDia , 115, 120);
    
    campoMes = gtk_entry_new();
    gtk_entry_set_max_length(GTK_ENTRY(campoMes), 2);
    gtk_entry_set_width_chars(GTK_ENTRY(campoMes), 4);
    gtk_entry_set_placeholder_text (GTK_ENTRY(campoMes), "mm");
    gtk_layout_put(GTK_LAYOUT(ntAba), campoMes , 170, 120);
    
    campoAno = gtk_entry_new();
    gtk_entry_set_max_length(GTK_ENTRY(campoAno), 4);
    gtk_entry_set_width_chars(GTK_ENTRY(campoAno), 8);
    gtk_entry_set_placeholder_text (GTK_ENTRY(campoAno), "aaaa");
    gtk_layout_put(GTK_LAYOUT(ntAba), campoAno , 230, 120); 

    campoHora = gtk_entry_new();
    gtk_entry_set_max_length(GTK_ENTRY(campoHora), 2);
    gtk_entry_set_width_chars(GTK_ENTRY(campoHora), 4);
    gtk_entry_set_placeholder_text (GTK_ENTRY(campoHora), "hh");
    gtk_layout_put(GTK_LAYOUT(ntAba), campoHora , 415, 120);
    
    campoMinuto = gtk_entry_new();
    gtk_entry_set_max_length(GTK_ENTRY(campoMinuto), 2);
    gtk_entry_set_width_chars(GTK_ENTRY(campoMinuto), 4);
    gtk_entry_set_placeholder_text (GTK_ENTRY(campoMinuto), "mm");
    gtk_layout_put(GTK_LAYOUT(ntAba), campoMinuto , 490, 120);
    
    campoGasto = gtk_entry_new();
    gtk_entry_set_max_length(GTK_ENTRY(campoGasto), 12);
    gtk_entry_set_width_chars(GTK_ENTRY(campoGasto), 24);
    gtk_entry_set_placeholder_text (GTK_ENTRY(campoGasto), "0000,00");
    gtk_layout_put(GTK_LAYOUT(ntAba), campoGasto , 280, 240);
    
    
    

    
    /* Cria o combobox e adiciona as categorias a ele*/
    comboboxTransacoes = gtk_combo_box_text_new ();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT (comboboxTransacoes), "Clique aqui e veja as categorias cadastradas por você");
            
    int i;
    for (i = 0; i < qtdFinal; i++){
      gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (comboboxTransacoes), user.Categoria[i].Nome);
    }
    
    gtk_combo_box_set_active (GTK_COMBO_BOX (comboboxTransacoes), 0);
    
    //gtk_layout_put(GTK_LAYOUT(ntAba), comboboxTransacoes, 220, 170);
    
    gtk_layout_put(GTK_LAYOUT(ntAba), comboboxTransacoes, 200, 165);
    
    
    dados->campoDia = campoDia;
    dados->campoMes = campoMes;
    dados->campoAno = campoAno;
    dados->campoMinuto = campoMinuto;
    dados->campoHora = campoHora;
    dados->campoGasto = campoGasto;
    dados->tipo = radioBtnGasto; 
    
    
    btnCadastra = gtk_button_new_with_label("Cadastrar transação");
    gtk_layout_put(GTK_LAYOUT(ntAba), btnCadastra , 240, 280);

    
    g_signal_connect (btnCadastra, "clicked", G_CALLBACK (cadastraTransacao), dados);

    
    //Adiciona à janela a aba criada
    gtk_notebook_append_page (GTK_NOTEBOOK (window_Abas), ntAba, tituloAba);
    

    return (ntAba);
}












void mudaCategoria(GtkComboBox *comboboxConta, struct camposUser* dados){
    gchar labelTransacoesText[MAX];
    
    if(gtk_combo_box_get_active(comboboxConta) != 0){
        sprintf(labelTransacoesText, "Transações em \n '%s'",gtk_combo_box_text_get_active_text (GTK_COMBO_BOX_TEXT(comboboxConta)));
        gtk_label_set_text(dados->label_Transacoes, labelTransacoesText);
        imprimeTransacoes(gtk_combo_box_get_active(comboboxConta), dados);
    } else{
        gtk_label_set_text(dados->label_Transacoes, "Selecione\numa categoria:");
    }
    
}



//Adiciona abas à janela criada
GtkWidget *AdicionaAbaUsuario (GtkWidget* window_Abas, char *titulo, 
                               GtkWidget* ntAba, GtkWidget* window){
    
    //Variáveis para as labels
    GtkWidget *tituloAba;
    GtkWidget *label_Nome, *label_CPF, *label_Saldo, 
               *label_Categorias, *label_Transacoes, *bg;
    
    //Variáveis para os campos
    GtkWidget *scrolledwindow, *textarea;
    
    //Variáveis para o textarea
    GtkTextBuffer *bufferTA;
    GtkTextIter iterTA;
    
    //Variáveis para os botões
    GtkWidget *btnNome, *btnSaldo, *btnCPF, *btnRMensal, 
              *btnRAnual, *btnLogout, btnDelAcc;
    
    //Variáveis para o combobox

    
    
    
    struct camposUser* dados = malloc(sizeof(struct camposUser));
    
    
    defineMeses();
    
    

    //Cria uma label para exibir o título da aba
    tituloAba = gtk_label_new (titulo);

   
    //Cria uma aba para a página
    ntAba = gtk_layout_new (NULL, NULL);
    
    
    //Label para o fundo da aba
    bg = gtk_label_new(NULL);
    gtk_style_context_add_class(gtk_widget_get_style_context(bg),"bgUser");
    gtk_widget_set_size_request(bg, 600, 500);
    gtk_layout_put(GTK_LAYOUT(ntAba), bg, 0, 0);
    
    
    
    /* 
    //PARA O COMBOBOX DE TRANSAÇÕES

     /* Cria o combobox e adiciona as categorias a ele*/
    comboboxConta = gtk_combo_box_text_new ();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT (comboboxConta), "Clique aqui e veja as categorias cadastradas por você");
            
    int i;
    for (i = 0; i < qtdFinal; i++){
      gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (comboboxConta), user.Categoria[i].Nome);
    }
    
    gtk_combo_box_set_active (GTK_COMBO_BOX (comboboxConta), 0);
    
    gtk_layout_put(GTK_LAYOUT(ntAba), comboboxConta, 220, 170);
    
   
    
    
    
    
    //Label para o nome do usuário
    label_Nome = gtk_label_new("Seu Nome:");
    gtk_style_context_add_class(gtk_widget_get_style_context(label_Nome),"labelUser");
    gtk_layout_put(GTK_LAYOUT(ntAba), label_Nome, 120, 25);
    
    //Label para o CPF do usuário
    label_CPF = gtk_label_new("Seu CPF:");
    gtk_style_context_add_class(gtk_widget_get_style_context(label_CPF),"labelUser");
    gtk_layout_put(GTK_LAYOUT(ntAba), label_CPF, 135, 65);
    
    
    //Label para o  saldo do usuário
    label_Saldo = gtk_label_new("Saldo Disponível: R$");
    gtk_style_context_add_class(gtk_widget_get_style_context(label_Saldo),"labelUser");
    gtk_layout_put(GTK_LAYOUT(ntAba), label_Saldo, 60, 105);
    
    //Label para as categorias do usuário
    label_Categorias = gtk_label_new("Categorias Cadastradas:");
    gtk_style_context_add_class(gtk_widget_get_style_context(label_Categorias ),"labelUser");
    gtk_layout_put(GTK_LAYOUT(ntAba), label_Categorias, 40, 155);
    
    
    //Label para as transações do usuário em uma dada categoria 
    label_Transacoes = gtk_label_new("Selecione\numa categoria:");
    gtk_style_context_add_class(gtk_widget_get_style_context(label_Transacoes ),"labelUser");
    gtk_label_set_justify (GTK_LABEL(label_Transacoes),GTK_JUSTIFY_RIGHT);
    gtk_layout_put(GTK_LAYOUT(ntAba), label_Transacoes, 100, 200);
    
    
    


    

    //Campo para o nome do usuário
    campoNome = gtk_entry_new();
    gtk_entry_set_max_length(GTK_ENTRY(campoNome ), ENTRADA);
    gtk_entry_set_width_chars(GTK_ENTRY(campoNome), LARGURA);
    gtk_entry_set_text (GTK_ENTRY(campoNome), user.Nome);
    gtk_editable_set_editable(GTK_EDITABLE(campoNome), FALSE);gtk_layout_put(GTK_LAYOUT(ntAba), campoNome, 220, 40);
    gtk_style_context_add_class(gtk_widget_get_style_context(campoNome),"inputUser");
    gtk_layout_put(GTK_LAYOUT(ntAba), campoNome, 220, 40);
    
    
    //Campo para o CPF do usuário
    campoCPF  = gtk_entry_new();
    gtk_entry_set_max_length(GTK_ENTRY(campoCPF), ENTRADA);
    gtk_entry_set_width_chars(GTK_ENTRY(campoCPF), LARGURA);
    gtk_entry_set_text (GTK_ENTRY(campoCPF), user.CPF);
    gtk_editable_set_editable(GTK_EDITABLE(campoCPF), FALSE);
    gtk_style_context_add_class(gtk_widget_get_style_context(campoCPF),"inputUser");
    gtk_layout_put(GTK_LAYOUT(ntAba), campoCPF, 220, 80);
    
    
    //Campo para o saldo do usuário
    gchar saldoAux[MAX];
    sprintf(saldoAux, "%.2lf", user.Saldo);
    campoSaldo = gtk_entry_new();
    gtk_entry_set_max_length(GTK_ENTRY(campoSaldo), ENTRADA);
    gtk_entry_set_width_chars(GTK_ENTRY(campoSaldo), LARGURA);
    gtk_entry_set_text (GTK_ENTRY(campoSaldo), saldoAux);
    gtk_editable_set_editable(GTK_EDITABLE(campoSaldo), FALSE);
    gtk_style_context_add_class(gtk_widget_get_style_context(campoSaldo),"inputUser");
    gtk_layout_put(GTK_LAYOUT(ntAba), campoSaldo, 220, 120);
    
    
       
    //PARA O TEXTAREA DE TRANSAÇÕES
    textarea = gtk_text_view_new();
    bufferTA = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textarea));
    

    gtk_text_view_set_editable(GTK_TEXT_VIEW (textarea), FALSE);
     
    scrolledwindow = gtk_scrolled_window_new(NULL, NULL); 
    gtk_container_add(GTK_CONTAINER(scrolledwindow), textarea);
    gtk_widget_set_size_request(scrolledwindow, 320, 200);
    gtk_style_context_add_class(gtk_widget_get_style_context(scrolledwindow),"textarea");
    
    
    
    
    //Botão para modificar o nome do usuário
    btnNome = gtk_button_new();
    gtk_style_context_add_class(gtk_widget_get_style_context(btnNome),"btnEdita");
    gtk_layout_put(GTK_LAYOUT(ntAba), btnNome, 490, 40);
    
    
    //Botão para modificar o CPF do usuário
    btnCPF = gtk_button_new();
    gtk_style_context_add_class(gtk_widget_get_style_context(btnCPF),"btnEdita");
    gtk_layout_put(GTK_LAYOUT(ntAba), btnCPF, 490, 82);
    
    //Botão para modificar o saldo do usuário
    btnSaldo = gtk_button_new();
    gtk_style_context_add_class(gtk_widget_get_style_context(btnSaldo),"btnEdita");
    gtk_layout_put(GTK_LAYOUT(ntAba), btnSaldo, 490, 122);
        
    //Botão para exibir o relatório mensal de transações
    btnRMensal = gtk_button_new_with_label("Relatório Mensal");
    gtk_layout_put(GTK_LAYOUT(ntAba), btnRMensal, 65, 280);
    
    //Botão para exibir o relatório anual de transações
    btnRAnual = gtk_button_new_with_label ("Relatório Anual");
    gtk_layout_put(GTK_LAYOUT(ntAba), btnRAnual, 69, 320);
    
    //Botão para o logout do usuário
    btnLogout = gtk_button_new_with_label ("Sair");
    gtk_layout_put(GTK_LAYOUT(ntAba), btnLogout, 100, 360);
    
    

   //Coordenadas do textarea de transações
    gtk_layout_put(GTK_LAYOUT(ntAba), scrolledwindow, 220, 220);
    
   
    dados->buffer = bufferTA;
    dados->iter = iterTA;
    dados->label_Transacoes = label_Transacoes;
    
    g_signal_connect (comboboxConta, "changed", G_CALLBACK (mudaCategoria), dados);
    g_signal_connect (btnNome, "clicked", G_CALLBACK (mudarNome), NULL); 
    g_signal_connect (btnRMensal, "clicked", G_CALLBACK (RelatorioMensal), dados); 
    g_signal_connect (btnRAnual, "clicked", G_CALLBACK (RelatorioAnual), dados); 
    g_signal_connect (btnLogout, "clicked", G_CALLBACK (gtk_main_quit), NULL);
    

    //Adiciona à janela a aba criada
    gtk_notebook_append_page (GTK_NOTEBOOK (window_Abas), ntAba, tituloAba);
    
    
    return (ntAba);
}

