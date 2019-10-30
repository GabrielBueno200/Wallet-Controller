#include <locale.h>
#include <gtk/gtk.h>
#include <string.h>
#include "GTKformUsuario.h"
#include "funcsLogin.h"

extern struct usuarioLogado user;





//Cria uma janela que suporta abas (Notebook)
void criaJanelaAbas (GtkWidget* window, GtkWidget* layoutUser, 
                     GtkWidget* layoutSobreNos, GtkWidget* layoutTransacoes,
                     GtkWidget* layoutCategoria){
    
    GtkWidget *window_Abas;
    GtkWidget *vbox;
    
    
    //Box com orientação vertical para empacotar todos os widgets
    vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
    
    //adiciona o box para a janela principal
    gtk_container_add (GTK_CONTAINER (window), vbox);

    //Cria a janela com abas
    window_Abas = gtk_notebook_new ();
    
    //Permite que a janela seja rolável
    gtk_notebook_set_scrollable (GTK_NOTEBOOK (window_Abas), TRUE);

    //Cria as abas na posição superior da janela
    gtk_notebook_set_tab_pos (GTK_NOTEBOOK (window_Abas), GTK_POS_TOP);

    
    
    // Adiciona a janela ao vbox
    gtk_box_pack_start (GTK_BOX (vbox), window_Abas, TRUE, TRUE, 0);

  
   
    //Adiciona abas à janela
    AdicionaAbaUsuario (window_Abas, "Minha Conta", layoutUser);
    AdicionaAbaTransacoes (window_Abas, "Cadastrar Transação", layoutTransacoes, window);   
    AdicionaAbaCategorias (window_Abas, "Categorias", layoutCategoria, window);
    AdicionaAbaSobreNos (window_Abas, "Sobre Nós", layoutSobreNos, window);

}




/*
 * main
 *
 * Program begins here
 */
int main (int argc, char *argv[]){
    
    /* inicializa o GTK*/
    gtk_init (&argc, &argv);
    
    setlocale(LC_ALL, "");
    
    user.logged_in = 0;
    
    formFrame();
    
    if(user.logged_in == 1){
    
        GtkWidget *window;


        GtkWidget* layoutUser, *layoutSobreNos, *layoutTransacoes, *layoutCategoria;



        GtkCssProvider *cssProvider = gtk_css_provider_new();
        gtk_css_provider_load_from_path(cssProvider, "estilo.css", NULL);
        gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
                                   GTK_STYLE_PROVIDER(cssProvider),
                                   GTK_STYLE_PROVIDER_PRIORITY_USER);



        //Cria a janela principal
        window = gtk_window_new (GTK_WINDOW_TOPLEVEL);



        gtk_window_set_resizable(GTK_WINDOW(window), FALSE);


        //Seta o título da janela
        gtk_window_set_title (GTK_WINDOW (window), "Gerenciador Financeiro");

        //Declara o tamanho da janela
        gtk_window_set_default_size (GTK_WINDOW (window), 600, 500);


        //Cria janela que suporta abas (Notebook)
        criaJanelaAbas (window, layoutUser, layoutSobreNos, layoutTransacoes, layoutCategoria);




        


        // Mostra todos os widgets da janela e faz com que ela rode em loop
        gtk_widget_show_all (window);
        
        /*Identifica os signals handlers, que são responsáveis pelas funções que
        cada objeto executará ao sofrer interação (callback)*/
        g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);
        
        gtk_main ();
    }
}
