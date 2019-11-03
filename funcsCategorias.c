#include <locale.h>
#include <gtk/gtk.h>
#include <string.h>
#include "GTKAbas.h"
#include "funcsLogin.h"
#include "funcsGTK.h"
#include "funcsTransacoes.h"
#include "main.h"

/*=================================================================================
		              VARIÁVEIS GLOBAIS
==================================================================================*/
//de outro arquivos
extern struct usuarioLogado user; // captura os dados do usuário logado
extern int qtdFinal; //captura a quantidade de categorias cadastradas pelo usuário
extern struct Transacao* transacoes; /* será utilizada como array dinâmico (malloc), e reunirá todas as transações feitas pelo 							 usuário durante a execução do programa*/
extern int qtdTransacoes; //captura a quantidade de transacoes cadastradas pelo usuário. Inicialmente zerada.
extern GtkWidget *comboboxTransacoes;
extern GtkWidget *comboboxCategorias;
extern GtkWidget *comboboxConta;
extern GtkWidget *window;

//remove todos os itens do combobox
void limpaCombobox(GtkWidget* combobox){
    gtk_combo_box_text_remove_all (GTK_COMBO_BOX_TEXT(combobox));
}

//refaz todo o combobox, incluindo novamente as categorias
void refazCombobox(GtkWidget* combobox){
    //primeiro limpa o combobox
    limpaCombobox(combobox);
    
    //insere a primeira opção padrão
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT (combobox), "Clique aqui e veja as categorias cadastradas por você");
    
    //insere todas as categorias
    int i;
    for(i = 0; i < qtdFinal; i++){
        gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (combobox), user.Categoria[i].Nome);
    }
    
    //seta a primeira opção como ativa
    gtk_combo_box_set_active (GTK_COMBO_BOX (combobox), 0);  
}

//adiciona uma categoria à lsita do combobox
void adicionaCategoriaCombobox(GtkWidget* combobox){
    gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT(combobox),user.Categoria[qtdFinal-1].Nome);
}

//deleta uma categoria do combobox
void deletaCategoriaCombobox(GtkWidget* combobox, int pos){
    gtk_combo_box_text_remove (GTK_COMBO_BOX_TEXT(combobox), pos);
    gtk_combo_box_set_active (GTK_COMBO_BOX (combobox), pos-1);
}

//adiciona uma categoria ao struct de categorias pertencente ao struct do usuário logado
void adicionaCategoria(GtkDialog *caixaDialogo, gint response_id, struct camposUser* dados){
   
    //Soma uma unidade na variável global referente ao tamanho do array dinâmico de struct de categorias
    qtdFinal++;

    //Realoca o array dinâmico de struct de categorias para possuir uma posição a mais (já que um novo elemento será adicionado a ele)
    user.Categoria = realloc(user.Categoria, sizeof(struct CategoriasUsuario)*qtdFinal);

    //Pede ao usuário que informe o nome da nova categoria
    //OBS* utiliza-se qtdFinal-1 como posição para que o array não seja estourado
    strcpy(user.Categoria[qtdFinal-1].Nome, gtk_entry_get_text(GTK_ENTRY(dados->campoDialog)));


    //Variáveis para inicializar o arquivo da nova categoria e o endereco deste
    FILE* arquivoCategoria;
    char enderecoArquivo[MAX];
    criaCaminhoArquivo(enderecoArquivo, user.Categoria[qtdFinal-1].Nome);

    //Cria o arquivo da categoria
    arquivoCategoria = fopen(enderecoArquivo, "w");

    //Finaliza e fecha o arquivo - alerta se a criação foi concluída ou não
    if(arquivoCategoria != 0){
        //mensagem de sucesso ao usuário
        gchar msg[MAX];
        sprintf(msg, "Categoria %s cadastrada com sucesso!\n", user.Categoria[qtdFinal-1].Nome);
        mensagemAviso(window, msg);
        
        //fecha a caixa de diálogo
        gtk_widget_destroy(GTK_WIDGET(caixaDialogo));
        
        //fecha o arquivo da categoria
        fclose(arquivoCategoria);

        //adiciona as categorias ao combobx
        adicionaCategoriaCombobox(comboboxCategorias);
        adicionaCategoriaCombobox(comboboxConta);
        adicionaCategoriaCombobox(comboboxTransacoes);


    } else{
        //aviso se ocorrer algum erro
        mensagemAviso(window, "Ocorreu algum erro na criação da categoria!");
    }

}

//caixa de texto para o usuário inserir o nome da nova categoria
void addNomeCategoria(GtkWidget *btn, struct camposUser* dados){
    GtkWidget *caixaDialogo;
    GtkWidget *area, *campo;
    GtkWidget *label;



    //Cria uma caixa de diálogo para o usuário inserir o nome da categoria
    caixaDialogo = gtk_dialog_new_with_buttons ("Atenção", 
                                          GTK_WINDOW(window), 
                                          GTK_DIALOG_MODAL, 
                                          GTK_STOCK_OK, 
                                          GTK_RESPONSE_OK, 
                                          NULL);

    /*Cria uma label e um input e adiociona à uma área de conteúdo, 
     * posteriormente adicionando esse área à caixa de diálogo*/
    area = gtk_dialog_get_content_area (GTK_DIALOG (caixaDialogo));
    label = gtk_label_new ("Digite um nome para a categoria");
    campo = gtk_entry_new ();



    gtk_container_add (GTK_CONTAINER (area), label);
    gtk_container_add (GTK_CONTAINER (area), campo);


    //exibe a caixa de diálogo
    gtk_widget_show_all (caixaDialogo);

    dados->campoDialog = campo;

    //Chama a função para exibir o relatório ao usuário clicar em OK
    g_signal_connect (GTK_DIALOG (caixaDialogo), 
                      "response", 
                      G_CALLBACK (adicionaCategoria), 
                      dados);
    
}


//remoção da categoria selecionada
void deletaCategoria(GtkDialog *janelaAviso, gint response_id, struct camposUser* dados){
    
    //só deleta se a opção clicada for "OK"
    if(response_id == -6){
      //apenas fecha a janela se a opção não for OK
      gtk_widget_destroy(GTK_WIDGET(janelaAviso));  
      
    } else if(response_id == -5){
        
        //captura a opção de categoria escolhida pelo usuário no combobox
        int opc;
        opc = gtk_combo_box_get_active(GTK_COMBO_BOX(comboboxCategorias));
        
        //Cria um array dinâmico auxiliar de struct de categoria com uma posição a menos que o original (já que será feita a remoção de um elemento)
	struct CategoriasUsuario* userAux = malloc(sizeof(struct CategoriasUsuario)*(qtdFinal-1));
	
	//Laço para varrer o array dinâmico original de struct de categoria
	int j, k = 0;
	for(j = 0; j < qtdFinal; j++){
		// detecta os structs que possuem a categoria com nomes diferentes da categoria escolhida pelo usuário para remoção
		if(strcmp(user.Categoria[j].Nome, user.Categoria[opc-1].Nome) != 0){
			userAux[k] = user.Categoria[j]; // os que tiverem nomes diferentes serão armazenados no array dinâmico auxiliar
			k++;
		}
	}
	
	//Variáveis para formular o endereço do arquivo da categoria a ser removida
	char enderecoArquivo[MAX];
	criaCaminhoArquivo(enderecoArquivo, user.Categoria[opc-1].Nome);
	
	//Remoção do arquivo - alerta se for concluída ou não
	if (remove(enderecoArquivo) == 0){ 
            gchar aviso[MAX];
            deletaCategoriaCombobox(comboboxCategorias, opc);
            deletaCategoriaCombobox(comboboxConta, opc);
            deletaCategoriaCombobox(comboboxTransacoes, opc);
            sprintf(aviso, "A categoria '%s' foi removida com sucesso!\n", user.Categoria[opc-1].Nome);
            mensagemAviso(window, aviso);
            gtk_widget_destroy(GTK_WIDGET(janelaAviso));
        }else{
            mensagemAviso(window, "erro!");
        }
	

	//Subtrai uma unidade da variável global referente ao tamanho do array dinâmico original de struct de categorias
	qtdFinal--;
	
	//Realoca o array dinâmico original de struct de categorias para possuir uma posição a menos (já que um elemento foi removido)
	user.Categoria = realloc(user.Categoria, sizeof(struct CategoriasUsuario)*(qtdFinal));
	
	//Copia todo o conteúdo do array dinâmico auxiliar de struct de categorias para o original
	memcpy(user.Categoria, userAux, sizeof(struct CategoriasUsuario)*(qtdFinal));
        
    }
    

}


//janela para a confirmação de remoção da categoria
void confirmaDelCategoria(GtkWidget *btn, struct camposUser* dados){
    
  if(gtk_combo_box_get_active(GTK_COMBO_BOX(comboboxCategorias)) != 0){
        //Janela de aviso para informar ao usuário que houve algum erro
        GtkWidget *janelaAviso;
        gchar mensagem[MAX];
        
        sprintf(mensagem,"Atenção, a categoria '%s' será removida!", gtk_combo_box_text_get_active_text (GTK_COMBO_BOX_TEXT(comboboxCategorias)));
        //Seta a janela de aviso
        janelaAviso = gtk_message_dialog_new (GTK_WINDOW(window), GTK_DIALOG_MODAL, 
                                              GTK_MESSAGE_WARNING, 
                                              GTK_BUTTONS_OK_CANCEL, 
                                              mensagem);
        //Exibe a janela de aviso
        gtk_widget_show_all (janelaAviso);

        //Fecha a janela de aviso ao usuário clicar em OK
        g_signal_connect (GTK_DIALOG (janelaAviso), "response", G_CALLBACK (deletaCategoria), dados);
  }else{
      mensagemAviso(window, "Insira a categoria desejada!");
  }
}




//seta o novo nome da categoria - após o usuário informar seu novo nome
void setaNome(GtkDialog *caixaDialogo, gint response_id, struct camposUser* dados){
    
    //variável para armazenar a opção de categoria escolhida pelo usuário
    int opc = gtk_combo_box_get_active(GTK_COMBO_BOX(comboboxCategorias));
    

    //Variáveis para formulação do caminho do arquivo
    char enderecoArquivo[MAX];
    char novoNomeArquivo[MAX];
    char novoEndereco[MAX];

    //Formula o caminho do arquivo referente à categoria a ser renomeada
    criaCaminhoArquivo(enderecoArquivo, user.Categoria[opc-1].Nome);

    strcpy(novoNomeArquivo, gtk_entry_get_text(GTK_ENTRY(dados->campoDialog)));

    //Cria o novo endereço do arquivo referente À categoria escolhida (com o novo nome definido pelo usuário)
    criaCaminhoArquivo(novoEndereco, novoNomeArquivo);


    /*Renomeia o arquivo da categoria 
    Alerta ao usuário se tudo correu bem*/
    if (rename(enderecoArquivo, novoEndereco) == 0){
        char* aviso[MAX];
        
        sprintf(aviso, "A categoria '%s' foi renomeada para '%s' com sucesso!", user.Categoria[opc-1], novoNomeArquivo);
        mensagemAviso(GTK_WIDGET(window), aviso);
        gtk_widget_destroy(GTK_WIDGET(caixaDialogo));
        


        //Renomeia a categoria no array dinâmico de struct de categoria pertencente ao usuário
        strcpy(user.Categoria[opc-1].Nome, novoNomeArquivo);
        
        refazCombobox(comboboxCategorias);
        refazCombobox(comboboxConta);
        refazCombobox(comboboxTransacoes);
        
    }else{
        mensagemAviso(window, "Ocorreu algum erro ao renomear o arquivo.");
    }
}

//janela pedindo ao usuário o novo nome para a categoria
void renomearCategoria(GtkWidget *btn, struct camposUser* dados){
    
  if(gtk_combo_box_get_active(GTK_COMBO_BOX(comboboxCategorias)) != 0){
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

        /*Cria uma label e um combobox e adiociona à uma área de conteúdo, 
         * posteriormente adicionando esse área à caixa de diálogo*/
        area = gtk_dialog_get_content_area (GTK_DIALOG (caixaDialogo));
        sprintf(msgLabel, "Digite um novo nome para a categoria:¨%s", gtk_combo_box_text_get_active_text (GTK_COMBO_BOX_TEXT(comboboxCategorias)));
        label = gtk_label_new (msgLabel);
        campo = gtk_entry_new ();



        gtk_container_add (GTK_CONTAINER (area), label);
        gtk_container_add (GTK_CONTAINER (area), campo);


        //exibe a caixa de diálogo
        gtk_widget_show_all (caixaDialogo);

        dados->campoDialog = campo;

        //Chama a função para exibir o relatório ao usuário clicar em OK
        g_signal_connect (GTK_DIALOG (caixaDialogo), 
                          "response", 
                          G_CALLBACK (setaNome), 
                          dados);
  }else{
      mensagemAviso(window, "Insira a categoria desejada!");
  }
}

