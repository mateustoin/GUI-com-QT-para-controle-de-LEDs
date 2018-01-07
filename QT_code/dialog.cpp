#include "dialog.h"
#include "ui_dialog.h"

//Includes das bibliotecas que serão utilizadas para a comunicação serial
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>

//Include relacionado as mensagens que serão exibidas na tela
#include <QtWidgets>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    arduino_is_available = false; //Iniciando variável
    arduino_port_name = ""; //Iniciando variável com nada
    arduino = new QSerialPort; //Adicionado de acordo com o vídeo
/*
 * CODIGO FEITO PARA DEBUGAR E DESCOBRIR VENDOR ID E PRODUCT ID DAS PORTAS DISPONÍVEIS.
 * IDEM PARA IDENTIFICAR QUANTOS DISPOSITIVOS ESTÃO DISPONÍVEIS NAS PORTAS SERIAIS
    //Para descobrir quantas portas estão conectadas na máquina
    qDebug() << "Número de portas disponíveis: " << QSerialPortInfo::availablePorts().length();

    //Verifica o arduino da porta (se for um arduino mesmo)
    foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()){ //Vendor e Product ID está relacionado a venda do hardware ou algo relacionado á identificação dos produtos
        //PELO PRODUCT ID POR EXEMPLO CONSEGUIMOS DIFERENCIAR E IDENTIFICAR QUAL PLACA ESTÁ CONECTADA (UNO = 67, MEGA = 66)
        qDebug() << "Tem vendor ID: " << serialPortInfo.hasVendorIdentifier();
        if (serialPortInfo.hasVendorIdentifier()){
            qDebug() << "Vendor ID: " << serialPortInfo.vendorIdentifier();
        }

        qDebug() << "Tem product ID: " << serialPortInfo.hasProductIdentifier();
        if (serialPortInfo.hasProductIdentifier()){
            qDebug() << "Product ID: " << serialPortInfo.productIdentifier();
        }
    }
*/

    foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()){
        if(serialPortInfo.hasVendorIdentifier() && serialPortInfo.hasProductIdentifier()){
            if(serialPortInfo.vendorIdentifier() == arduino_mega_vendor_id){
                if(serialPortInfo.productIdentifier() == arduino_mega_product_id){
                    arduino_port_name = serialPortInfo.portName(); // Se passar por todas as verificações recebe o nome da porta
                    arduino_is_available = true; // Se passar por todas as verificações recebe true na verificação
                }
            }
        }
    }

    //Agora que passou as verificações, vamos ao trabalho
    if (arduino_is_available){
        // Temos que abrir e configurar a porta serial
        /*
         * A SEGUIR ENCONTRAMOS AS CONFIGURAÇÕES RECOMENDADAS
         * LEMBRAR DE FAZÊ-LAS DEPOIS QUE ABRIR O ARDUINO (USAR ARDUINO->OPEN()), POIS FAZER ANTES NÃO FAZ SENTIDO
         * VISTO QUE ELES RETORNAM TRUE OU FALSE E SÓ SERVIRÁ DE ALGO QUANDO O ARDUINO ESTIVER ACESSÍVEL
         *
         * */
        arduino->setPortName(arduino_port_name); // Mudando o nome da porta para o que encontramos do arduino
        arduino->open(QSerialPort::WriteOnly); //Acessando arduino em modo de escrita para a placa, pois só iremos controlar os leds
        arduino->setBaudRate(QSerialPort::Baud9600);
        arduino->setDataBits(QSerialPort::Data8); // Arduino, 8 bits, dãã
        arduino->setParity(QSerialPort::NoParity); // Descobrir depois o que está relacionado a isso!!!! <<<<<<<<<<<
        arduino->setStopBits(QSerialPort::OneStop); // A opção half and stop é só para o windows
        arduino->setFlowControl(QSerialPort::NoFlowControl); // Descobrir pra que diachos isso serve também!!! <<<<<<<

    }else{
        // Exibe uma mensagem de erro se arduino não estiver disponível
        QMessageBox::warning(this, "Erro na porta", "Não foi possível localizar o Arduino...");
    }

}

Dialog::~Dialog()
{
    delete ui;

    // Configurando destrutor para fechar comunicação serial quando finalizar
    if (arduino->isOpen()){
        arduino->close();
    }
}

void Dialog::on_led1_barra_valueChanged(int value)
{
    //Primeiro ele entra na interface gráfica, muda o texto do label para o valor da barra
    //Para tanto, é necessário converter o valor para string, por isso o "%1"
        //ui->led1_valor_label->setText(QString("%1").arg(value));

    //Agora para o valor ficar fixo com a formatação tamanho 18 e bold, foi necessárior ir na formatação e copiar o spam
    ui->led1_valor_label->setText(QString("<span style=\" font-size:18pt; font-weight:600;\">%1</span>").arg(value));
    Dialog::updateLED(QString("a%1").arg(value)); // Imprime letra "a" antes do número, como esperado para mudar led individualmente
    qDebug() << "Valor led1: " << value;
}

void Dialog::on_led2_barra_valueChanged(int value)
{
    //Agora para o valor ficar fixo com a formatação tamanho 18 e bold, foi necessárior ir na formatação e copiar o spam
    ui->led2_valor_label->setText(QString("<span style=\" font-size:18pt; font-weight:600;\">%1</span>").arg(value));
    Dialog::updateLED(QString("b%1").arg(value)); // Imprime letra "b" antes do número, como esperado para mudar led individualmente
    qDebug() << "Valor led2: " << value;
}

void Dialog::on_led3_barra_valueChanged(int value)
{
    //Agora para o valor ficar fixo com a formatação tamanho 18 e bold, foi necessárior ir na formatação e copiar o spam
    ui->led3_valor_label->setText(QString("<span style=\" font-size:18pt; font-weight:600;\">%1</span>").arg(value));
    Dialog::updateLED(QString("c%1").arg(value)); // Imprime letra "c" antes do número, como esperado para mudar led individualmente
    qDebug() << "Valor led3: " << value;
}

// Adicionando mais um método para a classe Dialog
void Dialog::updateLED (QString comando)
{
    if(arduino->isWritable()){
        arduino->write(comando.toStdString().c_str());
    }else{
        qDebug() << "Não foi possível escrever na porta serial!";
    }
}
