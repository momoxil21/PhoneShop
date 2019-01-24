#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtXml>
#include <QFile>
#include <QDebug>
#include <iostream>
#include <QUuid>
#include "websocket.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void loadXML();
    void readPhonesXML();
    void readRepairsXML();
    void writeOrderXML();


    // el qdomdocumento representara un documento xml
    QDomDocument xmlBOM;
    QDomDocument xmlRepairs;

private slots:
    void go();

    void on_pushButton_2_clicked();

    void on_comboBox_currentTextChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;
    Websocket *m_webSocket;
};

#endif // MAINWINDOW_H
