# SerialPlotDemo
This project run an example of graph plot to draw one real and one virtual channel and display on this app.

Serial connect to COM Port at Bauld Rate 115200.

When the 'connect' button is clicked, the process starts to draw data.

<details>
  <summary>User Interface of the app</summary>

  ![Description](https://github.com/LongNguyen1984/SerialPlotDemo/blob/main/GUIdemo.png)

</details>

File 'On_OFF_Serial.ino' contains sample code for reading an analog value of the resistor and sending it to the PC.

# Compiled to an app on Windows.
* Step 1. We used the customPlot library, therefore in the project file, we need adding:

<code>
QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets **printsupport**
CONFIG += c++17
</code>
* Step 2. In file main.cpp we would like to keep style of QT Theme, hence we add the following code:
<code>
    **QApplication::setStyle("fusion");**
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
</code>

* Step 3. We need a config file for qt named 'qt.conf'
We must copy all libraries from QT creator to ensure the App can run on Windows OS
<code>
_%QT Installed Folder%/6.5.2/msvc2019_64/plugins/styles/_
List lib files in 
_%QT Installed Folder%/QT/Tools/QTCreator/bin_
 QT6Core.dll
 Qt6Gui.dll
 QT6SerialPort.dll
 Qt6Widgets.dll
_%QT Installed Folder%/QT/Tools/QTCreator/bin/plugins/platforms_
</code>
