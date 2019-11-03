#include <gtk/gtk.h>
#include "funcsLogin.h"

/*=================================================================================
		              VARIÁVEIS GLOBAIS
==================================================================================*/
//de outros arquivos
extern struct usuarioLogado user;
extern int qtdFinal;


void criaLabel(GtkWidget** label, gchar* texto, gchar* classe, gint x, gint y, GtkWidget *layout){
    *label = gtk_label_new(texto); //cria a label com o texto informado
    gtk_style_context_add_class(gtk_widget_get_style_context(*label),classe); //seta a classe informada para a label
    gtk_layout_put(GTK_LAYOUT(layout), *label , x, y); //coordenadas do campo
}

void criaCampo(GtkWidget**campo, gint nCarac, gint larguraCarac, gchar* placeholder, 
               gint x, gint y, gboolean edit, gboolean haveText, gchar* texto, GtkWidget*layout){
    *campo = gtk_entry_new(); //seta um novo campo
    gtk_entry_set_max_length(GTK_ENTRY(*campo), nCarac); //número máximo de caracteres do campo;
    gtk_entry_set_width_chars(GTK_ENTRY(*campo), larguraCarac); //seta a largura do campo
    gtk_entry_set_placeholder_text (GTK_ENTRY(*campo), placeholder); //seta o placeholder do campo
    gtk_editable_set_editable(GTK_EDITABLE(*campo),edit);
    if (haveText == TRUE)
        gtk_entry_set_text (GTK_ENTRY(*campo), texto); //seta o texto do campo
    gtk_layout_put(GTK_LAYOUT(layout), *campo , x, y); //coordenadas do campo
    
}

void criaCombobox(GtkWidget** combobox, GtkWidget* layout, gint x, gint y){
    
    /* Cria o combobox e adiciona as categorias a ele*/
    *combobox = gtk_combo_box_text_new ();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT (*combobox), "Clique aqui e veja as categorias cadastradas por você");
    
    //insere as categorias no combobox
    int i;
    for (i = 0; i < qtdFinal; i++){
      gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (*combobox), user.Categoria[i].Nome);
    }
    
    //seta a primeira posição como ativa
    gtk_combo_box_set_active (GTK_COMBO_BOX (*combobox), 0);
    
    //coordenadas do combobox
    gtk_layout_put(GTK_LAYOUT(layout), *combobox, x, y);
    
}

void criaBtn(GtkWidget** btn, gchar* texto, gint x, gint y, GtkWidget* layout){
    *btn = gtk_button_new_with_label(texto); //botão com texto acoplado
    gtk_layout_put(GTK_LAYOUT(layout), *btn , x, y); //coordenadas do botão
}

void criaTextArea(GtkWidget** textarea, GtkTextBuffer ** buffer, 
        gint largura, gint altura, gint x, gint y,
        gchar* classe, gboolean edit, GtkWidget* layout){
    
    GtkWidget* scrolledwindow; //janela rolável
    
    *textarea = gtk_text_view_new(); //seta um novo textarea
    *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(*textarea)); //buffer do textarea
    

    gtk_text_view_set_editable(GTK_TEXT_VIEW (*textarea), edit); //torna o textarea editável
     
    scrolledwindow = gtk_scrolled_window_new(NULL, NULL); //seta uma nova janela rolável
    gtk_container_add(GTK_CONTAINER(scrolledwindow), *textarea); //adiciona o textàrea à janela rolável
    gtk_widget_set_size_request(scrolledwindow, largura, altura); //define um tamanho para a janela rolável
    gtk_style_context_add_class(gtk_widget_get_style_context(scrolledwindow), classe); //classe da janela roável
    
    gtk_layout_put(GTK_LAYOUT(layout), scrolledwindow, x, y);//Coordenadas do textarea de transações
    
}


void insereTextoTextarea(GtkTextBuffer** buffer, GtkTextIter* iter, gchar* texto){
    gtk_text_buffer_get_iter_at_offset(*buffer, iter, 0); //itera o buffer do textarea
    gtk_text_buffer_insert(*buffer, iter, texto, -1);     //insere o conteúdo informado no buffer do textarea
}


int radioBtnAtivo(GtkWidget* radioBtn){
    
    int btnAtivo; //índice do radio button ativo
    GSList * tmp_list = gtk_radio_button_get_group (GTK_RADIO_BUTTON(radioBtn));//Armazena o grupo de radio buttons
    GtkToggleButton *tmp_button;//Cria um toggle button temporário
    
    while (tmp_list){//Laço para percorrer a lista de radio buttons
        tmp_button = tmp_list->data;//Botão temporário recebe o radio button a ser analisado da lista
        tmp_list = tmp_list->next;//Incrementa a lista para o próximo radio button

        if (gtk_toggle_button_get_active(tmp_button))//Verifica se o radio button analisado está ativo
          btnAtivo = g_slist_index (tmp_list, tmp_button); //captura a posição do radio button ativo
          g_print("%d",btnAtivo);
          break;//Se estiver, sai do laço
    }
    
    return btnAtivo;
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

//limpa o buffer do textarea
void limpaBuffer(GtkTextBuffer* buffer){
    GtkTextIter start, end;
    
    gtk_text_buffer_get_bounds(buffer, &start, &end);
    gtk_text_buffer_delete(buffer, &start, &end);
}

void importaCss(GtkCssProvider **cssProvider){
     gtk_css_provider_load_from_path(*cssProvider, "estilo.css", NULL);
     gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
                                               GTK_STYLE_PROVIDER(*cssProvider),
                                               GTK_STYLE_PROVIDER_PRIORITY_USER);
}