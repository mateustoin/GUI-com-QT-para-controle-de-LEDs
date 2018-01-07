#include "dialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Dialog w;

    w.setFixedSize(550, 125); // TAMANHO DA JANELA DE DIÁLOGO ESTÁ EM 550/125 (TAMANHO FIXO)
    w.setWindowTitle("Controle de LED");

    w.show(); // DANDO UM NOME PARA A JANELA DO PROGRAMA


    return a.exec();
}
