
#ifndef FUNCSTRANSACOES_H
#define FUNCSTRANSACOES_H

struct Meses{
	char nome[20];
};

void defineMeses();
void criaCaminhoArquivo(char* nomeArquivo, char* categoria);
void atualizaTransacoes();
void imprimeTransacoes(int opc, struct camposUser* dados);
void imprimeRelatorioAnual(GtkDialog *caixaDialogo, gint response_id, struct camposUser* dados);
void RelatorioAnual(GtkWidget *btn, struct camposUser* dados);
void imprimeRelatorioMensal(GtkDialog *caixaDialogo, gint response_id, struct camposUser* dados);
void RelatorioMensal(GtkWidget *btn, struct camposUser* dados);
void armazenaTransacoes();
void organizaTransacoes();
void cadastraTransacao(GtkWidget *btn, struct camposTransacao* dados);


#endif /* FUNCSTRANSACOES_H */

