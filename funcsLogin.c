#include <gtk/gtk.h>
#include <string.h>
#include <locale.h>
#include <dir.h> 
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include "GTKformAbas.h"
#include "funcsGTK.h"
#define MAX 100

/*=================================================================================
		              VARIÁVEIS GLOBAIS
==================================================================================*/
//deste arquivo
struct usuarioLogado user; //struct para o usuário logado
int qtdFinal = 0; //qtd de transações cadastradas pelo usuário

//de outros arquivos
extern struct dadosCad* cadFields;


//struct para o usuário logado
struct usuarioLogado{
    int logged_in;
    char Nome[MAX];
    char CPF[MAX];
    char Senha[MAX];
    double Saldo;
    struct CategoriasUsuario* Categoria;
};

//Struct para as categorias do usuário
struct CategoriasUsuario{
    char Nome[MAX];
};


//remove quebra de strings vindas das linhas do arquivo "conta.txt"
void removeQuebra(char* str){
    str[strlen(str)-1] = '\0';
}


//define os atributos do usuário logado
void defineUsuarioLogado(char* postCpf, char* fileNomeUsuario, double fileSaldo, char* fileSenha, GtkWidget* windowForm){
    user.logged_in = 1; //após validado muda o atributo logged_in para 1 (usuário logado)
    
    //seta os atributos do usuário (nome, senha e saldo)
    strcpy(user.CPF, postCpf);
    strcpy(user.Nome, fileNomeUsuario);
    strcpy(user.Senha, fileSenha);
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
    
    //fecha a janela principal e parte para o funcionamento do programa
    gtk_widget_destroy(windowForm);
}



//busca a pasta e arquivo referente ao usuário
int buscaPorCPF(GtkWidget* windowForm, FILE** arquivoUsuario, char* enderecoArquivo, char* postCpf){
    //Janela de aviso para informar ao usuário que houve algum erro
    GtkWidget *janelaAviso;
        
    sprintf(enderecoArquivo, "%s/conta.txt", postCpf);
		
    *arquivoUsuario = fopen(enderecoArquivo, "r");
    
    //se conseguir localizar o arquivo do usuário retorna 1, senão mostra msg de erro e retorna 0
    if(*arquivoUsuario){
	return 1; 
    }else{
        mensagemAviso(windowForm, "Erro, CPF inválido!");
        return 0;
    }
}

//validação do login do usuário
void validaLogin(GtkWidget *btn, struct dadosLogin* dados){
    
    //variáveis para o arquivo do usuário
    char postCpf[MAX];
    char postSenha[MAX];
    char enderecoArquivo[MAX];
    
    FILE* arquivoUsuario;
    
    //captura o valor do CPF do usuário
    sprintf(postCpf, gtk_entry_get_text (GTK_ENTRY(dados->campoCPF)));
    
    //tenta achar a pasta e arquivo referente ao usuário
    if (buscaPorCPF(dados->janela, &arquivoUsuario, enderecoArquivo, postCpf)){
        sprintf(postSenha, gtk_entry_get_text (GTK_ENTRY(dados->campoSenha)));
        
        
        //Variável para armazenar as linhas do arquivo "conta.txt" temporariamente
        char linha[MAX];
        int contLinha = 1; //captura os índices das linhas
        
        //Variáveis para armazenar os dados extraídos do arquivo
	char fileNomeUsuario[MAX], fileSenha[MAX];
	double fileSaldo;
        
        
        //Extrai os dados do arquivo e armazena nas variáveis criadas
        while (fgets(linha, MAX, arquivoUsuario) != NULL){
            if(contLinha == 1){
                sprintf(fileNomeUsuario, linha);
                removeQuebra(fileNomeUsuario);
            }
            
            else if(contLinha == 2){
                sprintf(fileSenha, linha);
                removeQuebra(fileSenha);
            }
            
            else{
                sscanf(linha,"%lf", &fileSaldo);
            }
            contLinha++;
        }
        
        //fecha o arquivo referente ao usuário
        fclose(arquivoUsuario);
        
        //Se a senha presente no arquivo for idêntica à digitada pelo usuário, valida o login
        if (strcmp(fileSenha, postSenha) == 0){
            //Cria a "session" e informa os dados do usuário ao programa por meio do struct usuarioLogado
            //Invoca a função para definir os atributos do struct usuarioLogado
            defineUsuarioLogado(postCpf, fileNomeUsuario, fileSaldo, fileSenha, dados->janela);
            
            //libera os espaçoes de memória destinandos aos mallocs de struct para reunião dos campos de logine cadastro
            g_free(dados);
            g_free(cadFields);
        } else{
            mensagemAviso(dados->janela, "Senha incorreta!");
        }
    }
}

