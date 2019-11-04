#include <locale.h>
#include <gtk/gtk.h>
#include <string.h>
#include "GTKformUsuario.h"
#include "funcsUser.h"
#include "funcsLogin.h"

/*==============================================
		VARIÁVEIS GLOBAIS
===============================================*/
//de outros arquivos
extern struct usuarioLogado user;

//deste arquivo
GtkWidget *window;


//Cria uma janela que suporta abas (Notebook)
void criaJanelaAbas (GtkWidget* window, GtkWidget* layoutUser, 
                     GtkWidget* layoutSobreNos, GtkWidget* layoutTransacoes,
                     GtkWidget* layoutCategoria){
    
    
    GtkWidget *window_Abas; //variável para a janela de abas (notebook)
    GtkWidget *vbox; //vbox para armazenar a janela de abas
    
    
    //Box com orientação vertical para empacotar todos os widgets
    vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
    

    //Cria a janela com abas
    window_Abas = gtk_notebook_new ();
    
    //Permite que a janela seja rolável
    gtk_notebook_set_scrollable (GTK_NOTEBOOK (window_Abas), TRUE);

    //Cria as abas na posição superior da janela
    gtk_notebook_set_tab_pos (GTK_NOTEBOOK (window_Abas), GTK_POS_TOP);

    
   
    //Adiciona abas à janela de abas
    AdicionaAbaUsuario (window_Abas, "Minha Conta", layoutUser);
    AdicionaAbaTransacoes (window_Abas, "Cadastrar Transação", layoutTransacoes, window);   
    AdicionaAbaCategorias (window_Abas, "Categorias", layoutCategoria, window);
    AdicionaAbaSobreNos (window_Abas, "Sobre Nós", layoutSobreNos, window);
    
    
    // Adiciona a janela ao vbox
    gtk_box_pack_start (GTK_BOX (vbox), window_Abas, TRUE, TRUE, 0);
    
    //adiciona o box para a janela principal
    gtk_container_add (GTK_CONTAINER (window), vbox);

}




/*
 * main
 * O programa começa aqui
*/

int main (int argc, char *argv[]){
    
    // inicializa o GTK
    gtk_init (&argc, &argv);
    
    //seta a localização do pc
    setlocale(LC_ALL, "");
    
    //seta o usuário como deslogado, inicialmente
    user.logged_in = 0;
    
    //executa o formulário (para login e cadastro)
    formFrame();
    
    //após o usuário ter feito o login, o seu atributo logged_in mudará para 1, e então ele poderá 
    //desfrutar de todas as funcionalidades do programa
    if(user.logged_in == 1){
    
        //variáveis para os layouts (suportam coordenadas x e y para os elementos)
        GtkWidget* layoutUser, *layoutSobreNos, *layoutTransacoes, *layoutCategoria;


        //importa o CSS (responsável pelo estilo de algumas labels, caixas de texto e dos backgrounds do programa)
        GtkCssProvider *cssProvider = gtk_css_provider_new();
        importaCss(&cssProvider);

        //Cria a janela principal do programa
        window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
        
        //seta a janela principal como não redimensionável
        gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

        //Seta o título da janela principal
        gtk_window_set_title (GTK_WINDOW (window), "Gerenciador Financeiro");

        //Declara o tamanho da janela principal
        gtk_window_set_default_size (GTK_WINDOW (window), 600, 500);


        //Cria janela que suporta abas (Notebook)
        criaJanelaAbas (window, layoutUser, layoutSobreNos, layoutTransacoes, layoutCategoria);


        // Mostra todos os widgets da janela principal
        gtk_widget_show_all (window);
        
        //Identifica os signals handlers(CALLBACK) para finalizar o programa (logout do usuário)
        g_signal_connect (window, "destroy", G_CALLBACK (logout), NULL);
        
        //faz com que o programa rode em loop
        gtk_main ();
    }
}
