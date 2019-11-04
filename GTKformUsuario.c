#include <locale.h>
#include <gtk/gtk.h>
#include <string.h>
#include "GTKformAbas.h"

/*=================================================================================
		              VARIÁVEIS GLOBAIS
==================================================================================*/
//de outros arquivos
extern struct dadosCad* cadFields;
extern struct dadosLogin* loginFields;


//fecha a janela, liberando os espaços de memória reservado para o formulário de login e cadastro
void fechaJanela(){
    g_free(cadFields);
    g_free(loginFields);
    gtk_main_quit();
}


//Cria uma janela que suporta abas (Notebook)
void criaAbas (GtkWidget* windowForm, GtkWidget* layoutLogin, GtkWidget* layoutCadastro){
    GtkWidget *formAbas, *vbox;
    
    //Box com orientação vertical para empacotar todos os widgets
    vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
    


    //Cria a janela com abas
    formAbas = gtk_notebook_new ();
    
    //Permite que a janela seja rolável
    gtk_notebook_set_scrollable (GTK_NOTEBOOK (formAbas), TRUE);

    //Cria as abas na posição superior da janela
    gtk_notebook_set_tab_pos (GTK_NOTEBOOK (formAbas), GTK_POS_LEFT);

    
  
   
    //Adiciona abas à janela
    adicionaAbaCadastro (windowForm, formAbas, "Cadastro", layoutCadastro);  
    adicionaAbaLogin (windowForm, formAbas, "Login", layoutLogin);
    
    //adiciona o box para a janela principal
    gtk_container_add (GTK_CONTAINER (windowForm), vbox);
    
    // Adiciona a janela ao vbox
    gtk_box_pack_start (GTK_BOX (vbox), formAbas, TRUE, TRUE, 0);

}


void formFrame(){
    GtkWidget *windowForm, *vboxForm, *layoutLogin, *layoutCadastro;
    
    //Cria a janela principal
    windowForm = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    

    
    gtk_window_set_resizable(GTK_WINDOW(windowForm), FALSE);
    
     //Seta o título da janela
    gtk_window_set_title (GTK_WINDOW (windowForm), "Formulário");

    //Declara o tamanho da janela
    gtk_window_set_default_size (GTK_WINDOW (windowForm), 450, 400);
    
    criaAbas (windowForm, layoutLogin, layoutCadastro);
    
    g_signal_connect (windowForm, "destroy", G_CALLBACK (fechaJanela), NULL);
    
    // Mostra todos os widgets da janela e faz com que ela rode em loop
    gtk_widget_show_all (windowForm);
    gtk_main ();
}