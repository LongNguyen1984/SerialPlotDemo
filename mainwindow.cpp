#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSerialPortInfo>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->dial->setMaximum(499);
    ui->dial->setMinimum(0);
    ui->dial->setValue(0);
    //ui->pushButton_Connect->setText("Disconnect");
    COMPORT =  new QSerialPort();

    connect(COMPORT, SIGNAL(readyRead()),this,SLOT(Read_Data()));
    //setup windows graph

    dataTimer = new QTimer;

    ui->customPlot->addGraph(); // blue line
    ui->customPlot->graph(0)->setPen(QPen(QColor(40, 110, 255)));
    ui->customPlot->addGraph(); // red line
    ui->customPlot->graph(1)->setPen(QPen(QColor(255, 110, 40)));

    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%h:%m:%s");
    ui->customPlot->xAxis->setTicker(timeTicker);
    ui->customPlot->axisRect()->setupFullAxesBox();
    ui->customPlot->yAxis->setRange(-1.2, 1.2);

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(ui->customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->yAxis2, SLOT(setRange(QCPRange)));
    // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
    connect(dataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
    //dataTimer->start(30); // Interval 0 means to refresh as fast as possible
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_dial_valueChanged(int value)
{

    ui->lcdNumber_dial->display(value + 1);
    scroll_window = value;
}


void MainWindow::on_pushButton_scan_clicked()
{
    ui->comboBox->clear();
    Q_FOREACH(QSerialPortInfo port, QSerialPortInfo::availablePorts()) {
        ui->comboBox->addItem(port.portName());
    }


}


void MainWindow::on_pushButton_Connect_clicked()
{
    if (ui->pushButton_Connect->text()=="Connect"){
        COMPORT->setPortName(ui->comboBox->currentText());//set portName
        qDebug()<<"COM PORT:"<<ui->comboBox->currentText();

        COMPORT->setBaudRate(ui->comboBox_BauldRate->currentText().toInt());
        qDebug()<<"bauld rate:"<< ui->comboBox_BauldRate->currentText().toInt();


        //COMPORT->setBaudRate(QSerialPort::BaudRate::Baud115200);
        COMPORT->setParity(QSerialPort::Parity::NoParity);
        COMPORT->setDataBits(QSerialPort::DataBits::Data8);
        COMPORT->setStopBits(QSerialPort::StopBits::OneStop);
        COMPORT->setFlowControl(QSerialPort::FlowControl::NoFlowControl);
        COMPORT->open(QIODevice::ReadWrite);

        if(COMPORT->isOpen())
        {
            qDebug() << "Serial Port Is Connected";
            ui->pushButton_Connect->setText("Disconnect");
            dataTimer->start(30); // Interval 0 means to refresh as fast as possible
            //qDebug()<<"Timer start:"<<ui->pushButton_Connect->text();
            qDebug()<<"Timer star...";
        }
        else
        {
            qDebug() << "Serial Port Is Disconnected";
            qDebug() << COMPORT->error();
        }

    }
    else //Disconnect the serial comport
    {
        COMPORT->close();
        ui->pushButton_Connect->setText("Connect");
        dataTimer->stop();
    }

    /*
    COMPORT->setPortName(ui->comboBox->ite);
    COMPORT->setBaudRate(QSerialPort::BaudRate::Baud115200);
    */
}


void MainWindow::on_comboBox_currentTextChanged(const QString &arg1)
{
    qDebug() <<"Current port:"<<arg1;

}


void MainWindow::on_comboBox_BauldRate_currentTextChanged(const QString &arg1)
{
    qDebug() <<"Current bauld:"<<arg1;
}


void MainWindow::on_pushButton_Send_clicked()
{
    if (COMPORT->isOpen()){

        COMPORT->write(ui->Send_data->text().toLatin1() + char(10));

    }
}
void MainWindow::Read_Data()
{
    if(COMPORT->isOpen())
    {

        while(COMPORT->bytesAvailable())
        {
            Data_from_SerialPort += COMPORT->readAll();
        }
        if (Data_from_SerialPort.at(Data_from_SerialPort.length()-1)==char(10))
        {
            is_data_received = true;
        }
        if (is_data_received==true)
        {
            //qDebug() << "Data from Serial Port:" << Data_from_SerialPort;

            Data_from_SerialPort = Data_from_SerialPort.trimmed(); //remove \n

            ui->textEdit_Received_Data->append(Data_from_SerialPort);
            if (Data_from_SerialPort.contains("Resistor:"))
            {
                //QString subString = Data_from_SerialPort
                static QRegularExpression rx("\\s+");
                //Data_from_SerialPort.indexOf(rx);
                resistor = Data_from_SerialPort.split(rx).at(1).toInt(); //access the 2nd elements
                ui->progressBar_resistor->setValue(resistor/6.85); //690 is maximum of resistor
            }

            Data_from_SerialPort ="";
            is_data_received = false;
        }


    }
}


void MainWindow::on_pushButton_Save_clicked()
{
    //QIODevice::Truncate = Delete file data and add new data
    //QIODevice::Append = Add new data with out delete old data

    QFile File(ui->lineEdit_filename->displayText());
    if (File.open(QIODevice::Truncate| QIODevice::ReadWrite))
    {
        QTextStream stream(&File);
        stream << ui->textEdit_Received_Data->toPlainText();
    }
    File.close();
    ui->textEdit_Received_Data->clear();
    ui->statusbar->showMessage("Saving Done!");
}

void MainWindow::realtimeDataSlot()
{
    //static QTime time(QTime::currentTime());
    static QElapsedTimer time;
    // calculate two new data points:
    double key = time.elapsed()/1000.0; // time elapsed since start of demo, in seconds
    static double lastPointKey = 0;
    if (key-lastPointKey > 0.002) // at most add point every 2 ms
    {
        // add data to lines:
        ui->customPlot->graph(0)->addData(key, resistor);//+rand()/(double)RAND_MAX*1*qSin(key/0.3843)
        ui->customPlot->graph(1)->addData(key, qCos(key));//+rand()/(double)RAND_MAX*0.5*qSin(key/0.4364)
        // rescale value (vertical) axis to fit the current data:
        //ui->customPlot->graph(0)->rescaleValueAxis();
        //ui->customPlot->graph(1)->rescaleValueAxis(true);
        //ui->customPlot->graph(0)->rescaleAxes();
        //ui->customPlot->graph(1)->rescaleAxes(true);
        lastPointKey = key;
    }
    // make key axis range scroll with the data (at a constant range size of 8):
    ui->customPlot->xAxis->setRange(key, 1 + scroll_window, Qt::AlignRight);
    //double minrangeLine = *std::min(ui->customPlot->graph(0)->data()->begin(),ui->customPlot->graph(0)->data()->end());
    //double maxrangeLine = *std::max(ui->customPlot->graph(0)->data()->begin(),ui->customPlot->graph(0)->data()->end());
    //QCPGraphDataContainer::const_iterator begin;
    //QCPGraphDataContainer::const_iterator end;
    //QCPDataRange range = ui->customPlot->graph(0)->getValueRange(0,100);
    //qDebug()<<"Range of Data"<<ui->customPlot->graph(0)->getValueRange();

    //ui->customPlot->yAxis->setRange(key, 16, Qt::AlignCenter);
    //ui->customPlot->graph(0)->getVisibleDataBounds(begin,end,range);
    //ui->customPlot->rescaleAxes(true); //compact data
    //ui->customPlot->graph(1)->rescaleKeyAxis(true);

    //calculate min, max of y values in visible window
    double Min, Max;
    Min = Max = 0;
    for(int k = 0; k <2; k++)
    {
        auto plotData = ui->customPlot->graph(k)->data();
        int Size = plotData->size();
        const int WIDTH = 550;
        double min, max;
        if (Size <= WIDTH){
            min = max = 0;
            for (int i = 0 ; i < Size ; ++i){
                double temp = plotData->at(i)->value;
                if (temp > max) max = temp;
                if (temp < min) min = temp;

            }
        }
        else{
            min = max = 0;
            for (int i = Size-1 ; i >= Size - WIDTH ; --i){
                double temp = plotData->at(i)->value;
                if (temp > max) max = temp;
                if (temp < min) min = temp;

            }
        }

        if (min < Min) Min = min;
        if (max > Max) Max = max;
    }
    ui->customPlot->yAxis->setRange(Min, Max);
    ui->customPlot->replot();

    // calculate frames per second:
    static double lastFpsKey;
    static int frameCount;
    ++frameCount;
    if (key-lastFpsKey > 2) // average fps over 2 seconds
    {
        ui->statusbar->showMessage(
            QString("%1 FPS, Total Data points: %2")
                .arg(frameCount/(key-lastFpsKey), 0, 'f', 0)
                .arg(ui->customPlot->graph(0)->data()->size()+ui->customPlot->graph(1)->data()->size())
            , 0);
        lastFpsKey = key;
        frameCount = 0;
    }
}
