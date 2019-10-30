#include <gtk/gtk.h>
#include <string.h>
#include <locale.h>
#include <dir.h> 
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include "GTKformAbas.h"
#define MAX 100

struct usuarioLogado{
    int logged_in;
    char Nome[MAX];
    char CPF[MAX]; 
    double Saldo;
    struct CategoriasUsuario* Categoria;
};

struct CategoriasUsuario{
    char Nome[MAX];
};

struct usuarioLogado user;
int qtdFinal = 0;

void defineUsuarioLogado(char* postCpf, char* fileNomeUsuario, double fileSaldo, GtkWidget* windowForm){
    user.logged_in = 1;
    strcpy(user.CPF, postCpf);
    strcpy(user.Nome, fileNomeUsuario);
    user.Saldo = fileSaldo;
    
    
    /*======================PARA AS CATEGORIAS===============================
                    analisa os arquivos .txt criados 
     * (que, com exceção do arquivo "conta.txt", referenciam as categorias)*/
    /*=======================================================================*/
    
    DIR *dir; //inicializa o ponteiro dir
    struct dirent *lsdir; //struct referente aos conteúdos da pasta
    
    dir = opendir(postCpf); //abre a pasta referente ao usuário
    
    user.Categoria = malloc(sizeof(struct CategoriasUsuario)); //malloc com uma qtd inicial de no máximo 1 elemento de struct de categoria
		
    // Encontra todos os arquivos e diretórios contidos na pasta 
    while ((lsdir = readdir(dir)) != NULL ){ 
	if (strcmp(lsdir->d_name, "conta.txt") != 0 && //Desconsidera o arquivo 'conta.txt', pois ele não se refere a uma categoria
            //O struct vem com algumas sujeiras "." e "..", por isso as condições abaixo para desconsiderá-las
            strcmp(lsdir->d_name, ".") != 0 && 
            strcmp(lsdir->d_name, "..") != 0){
            
                 strcpy(user.Categoria[qtdFinal].Nome, lsdir->d_name);
                 int tam = strlen(user.Categoria[qtdFinal].Nome) - 4; //Captura a posição do tamanho da string - 4 (onde começa a extensão do arquivo ".txt")
                 user.Categoria[qtdFinal].Nome[tam] = '\0'; //Força a string a terminar na posição onde o primeiro caracter da extensão começa
                 qtdFinal++;
                 user.Categoria = realloc(user.Categoria, sizeof(struct CategoriasUsuario)*(qtdFinal+1)); //realoca a quantidade de elementos do array dinâmico para inserir todas as categorias
        }
    }
	    
	    
    closedir(dir); //fecha a pasta
    
    gtk_widget_destroy(windowForm);
}

void mensagemAviso(GtkWidget* windowForm, gchar* mensagem){
    //Janela de aviso para informar ao usuário que houve algum erro
    GtkWidget *janelaAviso;
    
    //Seta a janela de aviso
    janelaAviso = gtk_message_dialog_new (GTK_WINDOW(windowForm), GTK_DIALOG_MODAL, 
                                          GTK_MESSAGE_WARNING, 
                                          GTK_BUTTONS_OK, 
                                          mensagem);
    //Exibe a janela de aviso
    gtk_widget_show_all (janelaAviso);
    
    //Fecha a janela de aviso ao usuário clicar em OK
    g_signal_connect (GTK_DIALOG (janelaAviso), "response", G_CALLBACK (gtk_widget_destroy), janelaAviso);
    
    
}


int buscaPorCPF(GtkWidget* windowForm, FILE** arquivoUsuario, char* enderecoArquivo, char* postCpf){
    //Janela de aviso para informar ao usuário que houve algum erro
    GtkWidget *janelaAviso;
        
    sprintf(enderecoArquivo, "%s/conta.txt", postCpf);
		
    *arquivoUsuario = fopen(enderecoArquivo, "r");
		
    if(*arquivoUsuario){
	return 1; 
    }else{
        mensagemAviso(windowForm, "Erro, CPF inválido!");
        return 0;
    }
}

void validaLogin(GtkWidget *btn, struct dadosLogin* dados){
    user.logged_in = 0;
    char postCpf[MAX];
    char postSenha[MAX];
    char enderecoArquivo[MAX];
    
    FILE* arquivoUsuario;
    
    sprintf(postCpf, gtk_entry_get_text (GTK_ENTRY(dados->campoCPF)));
    
    
    if (buscaPorCPF(dados->janela, &arquivoUsuario, enderecoArquivo, postCpf)){
        sprintf(postSenha, gtk_entry_get_text (GTK_ENTRY(dados->campoSenha)));
        
        //Variáveis para armazenar os dados extraídos do arquivo
	char fileNomeUsuario[MAX], fileSenha[MAX];
	double fileSaldo;
        
        //Extrai os dados do arquivo e armazena nas variáveis criadas
	fscanf(arquivoUsuario,"%s %s %lf", fileNomeUsuario, fileSenha, &fileSaldo);
        
        //Se a senha presente no arquivo for idêntica à digitada pelo usuário, valida o login
        if (strcmp(fileSenha, postSenha) == 0){
            //Cria a "session" e informa os dados do usuário ao programa por meio do struct usuarioLogado
            //Invoca a função para definir os atributos do struct usuarioLogado
            defineUsuarioLogado(postCpf, fileNomeUsuario, fileSaldo, dados->janela);
            free(dados);
        } else{
            mensagemAviso(dados->janela, "Senha incorreta!");
        }
    }
}

