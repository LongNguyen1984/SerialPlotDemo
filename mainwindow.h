#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QComboBox>
#include <QFile>
#include <QTextStream>
#include <QString>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_dial_valueChanged(int value);

    void on_pushButton_scan_clicked();

    void on_pushButton_Connect_clicked();

    void on_comboBox_currentTextChanged(const QString &arg1);

    void on_comboBox_BauldRate_currentTextChanged(const QString &arg1);

    void on_pushButton_Send_clicked();

    void Read_Data();

    void on_pushButton_Save_clicked();

    void realtimeDataSlot();

private:
    Ui::MainWindow *ui;
    QSerialPort* COMPORT;
    QString Data_from_SerialPort;
    bool is_data_received = false;
    int resistor = 0;
    int scroll_window = 0;
    QTimer* dataTimer;
};
#endif // MAINWINDOW_H
