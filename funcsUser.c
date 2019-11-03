#include <locale.h>
#include <gtk/gtk.h>
#include <string.h>
#include "GTKAbas.h"
#include "funcsGTK.h"
#include "funcsLogin.h"
#include "main.h"
#define MAX 100

/*================================================
		  VARIÁVEIS GLOBAIS
==================================================*/
//de outros arquivos
extern struct usuarioLogado user;
extern struct Transacoes* transacoes;
extern GtkWidget *campoNome, *campoCPF;
extern GtkWidget *window;

extern int qtdFinal;

extern struct camposUser* malloc_Categoria;
extern struct camposTransacao* malloc_Transacao;
extern struct camposUser* malloc_User;


//finaliza a sessão do usuário, liberando todos os espaços de memória
void logout(){
    g_free(user.Categoria);
    g_free(transacoes);
    g_free(malloc_Categoria);
    g_free(malloc_Transacao);
    g_free(malloc_User);
    
    
    gtk_main_quit();
}

//Redefine o saldo do usuário (quando alguma categoria é registrada)
void redefineSaldo(gchar* novoDado){
    
    //variáveis para o arquivo do usuário ("conta.txt")
    char caminhoArquivo[MAX];
    criaCaminhoArquivo(caminhoArquivo, "conta");
    FILE* arquivoUser = fopen(caminhoArquivo, "w");
    
    //altera os dados
    fprintf(arquivoUser, "%s\n", user.Nome);
    fprintf(arquivoUser, "%s\n", user.Senha);
    
    double saldoAux;
    sscanf(novoDado, "%lf", &saldoAux); //transforma string em double
    fprintf(arquivoUser, "%.2f", saldoAux);
    
    //fecha o arquivo
    fclose(arquivoUser);
}



//redefine o nome do usuário
void redefineNome(GtkDialog *caixaDialogo, gint response_id, GtkWidget* campoNovoNome){

    //variáveis para o arquivo do usuário ("conta.txt")
    char caminhoArquivo[MAX];
    criaCaminhoArquivo(caminhoArquivo, "conta");
    FILE* arquivoUser = fopen(caminhoArquivo, "w");
    
    //altera o arquivo do usuário 
    fprintf(arquivoUser, "%s\n", gtk_entry_get_text(GTK_ENTRY(campoNovoNome)));
    fprintf(arquivoUser, "%s\n", user.Senha);
    fprintf(arquivoUser, "%.2f", user.Saldo);
    
    //fecha o arquivo
    fclose(arquivoUser);
    
    
    //captura o dado da caixa de texto preenchida pelo usuário e seta ela ao atributo nome do struct do usuário
    sprintf(user.Nome, gtk_entry_get_text(GTK_ENTRY(campoNovoNome)));
    gtk_entry_set_text(GTK_ENTRY(campoNome), gtk_entry_get_text(GTK_ENTRY(campoNovoNome)));
    
    //mensagem de aviso - sucesso - ao usuário
    mensagemAviso(window,"Seu nome foi alterado com sucesso!");
    gtk_widget_destroy(GTK_WIDGET(caixaDialogo)); //fecha a caixa de diálogo
}

//caixa de diálogo para o usuário inserir seu novo nome
void mudarNome(GtkWidget* btn){
  
    GtkWidget *caixaDialogo;
    GtkWidget *area, *campo;
    GtkWidget *label;
    gchar msgLabel[MAX];



    //Cria uma caixa de diálogo para o usuário inserir o mês
    caixaDialogo = gtk_dialog_new_with_buttons ("Atenção", 
                                          GTK_WINDOW(window), 
                                          GTK_DIALOG_MODAL, 
                                          GTK_STOCK_OK, 
                                          GTK_RESPONSE_OK, 
                                          NULL);

    /*Cria uma label e um entry e adiociona à uma área de conteúdo, 
     * posteriormente adicionando esse área à caixa de diálogo*/
    area = gtk_dialog_get_content_area (GTK_DIALOG (caixaDialogo));
    label = gtk_label_new ("Digite o seu novo nome:");
    campo = gtk_entry_new ();



    gtk_container_add (GTK_CONTAINER (area), label);
    gtk_container_add (GTK_CONTAINER (area), campo);


    //exibe a caixa de diálogo
    gtk_widget_show_all (caixaDialogo);


    //Chama a função para exibir o relatório ao usuário clicar em OK
    g_signal_connect (GTK_DIALOG (caixaDialogo), 
                      "response", 
                      G_CALLBACK (redefineNome), 
                      campo);
   
}



void deletaConta(GtkDialog *janelaAviso, gint response_id){
    //só deleta se a opção clicada for "OK"
    if(response_id == -6){
      //apenas fecha a janela se a opção não for OK
      gtk_widget_destroy(GTK_WIDGET(janelaAviso));  
    } else if(response_id == -5){
        
        //variável para o caminho dos arquivos de categoria
        char caminhoArquivo[MAX];
        
        //laço para varrer todas as categorias
        int i;
        for(i = 0; i < qtdFinal; i++){
            //cria o caminho para a categoria analisada no laço
            criaCaminhoArquivo(caminhoArquivo, user.Categoria[i].Nome);	
            //remove o arquivo da categoria analisada no laço
            remove(caminhoArquivo);
        }
        
        //cria o caminho para o arquivo principal da conta ("conta.txt")
        criaCaminhoArquivo(caminhoArquivo, "conta");
        remove(caminhoArquivo);
        
        //caso consiga excluir a pasta(conta) referente ao usuário
        if (rmdir(user.CPF) != -1){
              mensagemAviso(window,"Conta excluída com sucesso!");
              gtk_widget_destroy(GTK_WIDGET(janelaAviso));
              logout();
        } else{
              mensagemAviso(window,"Erro ao deletar a sua conta!");
              gtk_widget_destroy(GTK_WIDGET(janelaAviso));
        }
    }
}

//janela para a confirmação de remoção da conta
void confirmaDelAcc(GtkWidget *btn, struct camposUser* dados){

    //Janela de aviso para informar ao usuário que houve algum erro
    GtkWidget *janelaAviso;

    //Seta a janela de aviso
    janelaAviso = gtk_message_dialog_new (GTK_WINDOW(window), GTK_DIALOG_MODAL, 
                                          GTK_MESSAGE_WARNING, 
                                          GTK_BUTTONS_OK_CANCEL, 
                                          "Atenção, a sua conta será removida!\nVocê tem certeza que deseja continuar?");
    
    
    //Exibe a janela de aviso
    gtk_widget_show_all (janelaAviso);

    //Fecha a janela de aviso ao usuário clicar em OK
    g_signal_connect (GTK_DIALOG (janelaAviso), "response", G_CALLBACK (deletaConta), NULL);
  
  
}





