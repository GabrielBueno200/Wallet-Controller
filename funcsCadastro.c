#include <gtk/gtk.h>
#include <string.h>
#include <locale.h>
#include <dir.h> 
#include "GTKformAbas.h"
#define MAX 100

int err;

//Struct para armazenar os dados recebidos dos campos
struct Pessoa{
    char Nome[MAX];
    char CPF[MAX]; 
    char Senha[MAX];
    double Saldo;
};


int criaPastaUsuario(char* CPF){
	return mkdir(CPF);
}

void armazenaCategoria(GtkTextBuffer* buffer, char* CPF){
    
    //Variáveis para o recolhimento dos dados informados no textarea de categorias
    gchar *textoTextArea; //armazena o texto obtido do textarea
    GtkTextIter iterInicial, iterFinal; //iteradores para varrer o buffer de texto
    
    //Variável para nomear o arquivo a ser criado (terá o nome da categoria)
    char enderecoArquivo[MAX]; 
    
    //Relaciona o iterador inicial ao buffer
    gtk_text_buffer_get_iter_at_offset (buffer, &iterInicial, 0);
    
    /*Recupera o primeiro e o último iterador do buffer, 
      relacionando ao iterador inicial e final, respectivamente*/
    gtk_text_buffer_get_bounds (buffer, &iterInicial, &iterFinal);
    
    //Recupera todo o texto do buffer (intervalo do iterador inicial ao final)
    textoTextArea = gtk_text_buffer_get_text (buffer, &iterInicial, &iterFinal, FALSE);
    
    /*Cria uma matriz para armazenar as strings referentes ao nome das categorias,
     onde esta é definida por cada palavra separada por \n do texto do textarea*/
    gchar** listaCategorias = g_strsplit(textoTextArea, "\n", MAX);
    
    //libera o espaço de memória ocupado pelo texto retirado do buffer
    g_free(textoTextArea);
    
    /*Laço para criar cada arquivo de categoria 
     (indo de 0 até o número de linhas da matriz listaCategorias)*/
    int i;            
    for(i = 0; i < g_strv_length (listaCategorias); i++){
        sprintf(enderecoArquivo, "%s/%s.txt", CPF, listaCategorias[i]);
        FILE* arquivoCategoria = fopen(enderecoArquivo , "w");
        fclose(arquivoCategoria);
    }
    
}

void armazenaPessoa(struct Pessoa pessoa, GtkWidget* windowForm){
    //Janela de aviso para informar ao usuário se o cadastro foi concluído ou se houve algum erro
    GtkWidget *janelaAviso;
    gchar aviso[MAX];
    
    //Tenta criar uma para para o usuário
    
    //se conseguir:
    if (criaPastaUsuario(pessoa.CPF) != -1){ //pasta para o usuário (nomeada pelo cpf)
        FILE* arquivoUser; //arquivo para armazenar as informações de login/saldo do usuário (conta.txt)

        char enderecoArquivo[MAX]; //endereço onde o arquivo será armazenado
        sprintf(enderecoArquivo, "%s/conta.txt", pessoa.CPF); //o endereço concatena o nome da pasta (cpf) com o nome do arquivo (conta.txt")

        arquivoUser = fopen(enderecoArquivo, "w"); //Cria o arquivo no endereço desejado

        //Escreve as informações no arquivo (nome, senha e saldo)
        fprintf(arquivoUser, "%s\n", pessoa.Nome);
        fprintf(arquivoUser, "%s\n", pessoa.Senha);
        fprintf(arquivoUser, "%.2f", pessoa.Saldo);

        //fecha o arquivo
        fclose(arquivoUser);
        
        //Seta a mensagem de sucesso para a janela de aviso
        strcpy(aviso, "Cadastrado com sucesso!");

        //Declara que não houve nenhum erro
        err = 0;
    } else{
        //Seta a mensagem de erro para a janela de aviso
        strcpy(aviso, "Erro: Já existe usuário cadastrado com o CPF informado!");
        
        //Declara que houve  erro
        err = 1;
    }
    
    
    //Seta a janela de aviso
    janelaAviso = gtk_message_dialog_new (GTK_WINDOW(windowForm), GTK_DIALOG_MODAL, 
                                            GTK_MESSAGE_WARNING, 
                                            GTK_BUTTONS_OK, 
                                            aviso);
    //Exibe a janela de aviso
    gtk_widget_show_all (janelaAviso);
    
    //Fecha a janela de aviso ao usuário clicar em OK
    g_signal_connect (GTK_DIALOG (janelaAviso), "response", G_CALLBACK (gtk_widget_destroy), janelaAviso);
          
}


void definePessoa(GtkWidget *btn, struct dadosCad* dados) {
    
    //Struct para armazenar os dados recebidos dos campos
    struct Pessoa pessoa;
    
    //Transfere os valores dos campos preenchidos para o struct pessoa
    sprintf(pessoa.Nome, gtk_entry_get_text (GTK_ENTRY(dados->campoNome)));
    sprintf(pessoa.CPF, gtk_entry_get_text (GTK_ENTRY(dados->campoCPF)));
    sprintf(pessoa.Senha, gtk_entry_get_text (GTK_ENTRY(dados->campoSenha)));
    sscanf(gtk_entry_get_text (GTK_ENTRY(dados->campoSaldo)), "%lf", &pessoa.Saldo);
    
    //Função para cadastrar o usuário
    armazenaPessoa(pessoa, dados->janela);
   
    
    //Função para cadastrar as categorias
    armazenaCategoria(dados->buffer, pessoa.CPF);
    


    
    
}