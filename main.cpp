#include "mainwindow.h"

#include <QApplication>
#include <QSerialPortInfo>
#include <QComboBox>
int main(int argc, char *argv[])
{
    QApplication::setStyle("fusion");
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    /*QComboBox box(&w);
    Q_FOREACH(QSerialPortInfo port, QSerialPortInfo::availablePorts()) {
        box.addItem(port.portName());
    }
    box.move(100 - box.width() / 2,100 - box.height() / 2);
    box.show();
    */
    return a.exec();
}
