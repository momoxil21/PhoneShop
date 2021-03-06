#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QTimer::singleShot(0, this, SLOT(go()));
    loadXML();
    readPhonesXML();
    startConfig();
    ui->tabWidget->tabBar()->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::go()
{
    m_webSocket = new Websocket(QUrl("ws://localhost:3344"));
    connect(m_webSocket, SIGNAL(recibirmensaje(QString)), this, SLOT(recibirmensaje(QString)));
}

void MainWindow::startConfig()
{
    ui->actionLogin->setVisible(true);
    ui->actionLog_out->setVisible(false);
    ui->label_client1->setVisible(false);
    ui->label_client2->setVisible(false);
    ui->label_client3->setVisible(false);
    ui->tabWidget->tabBar()->setCurrentIndex(0);
    ordersSizeWindow.setHeight(305);
    ordersSizeWindow.setWidth(518);
    ordersSizeLabel.setHeight(81);
    ordersSizeLabel.setWidth(471);
    this->setFixedSize(ordersSizeWindow);
    ui->statusOfOrderLabel->setFixedSize(ordersSizeLabel);
    ui->tabWidget->tabBar()->hide();
    ui->tabWidget->setCurrentIndex(0);
    ui->tab_3->setEnabled(true);
}

void MainWindow::loadXML()
{
    // Cargar el XML como RAW Data
    QFile f("AvailablePhones.xml");
    if (!f.open(QIODevice::ReadOnly))
    {
        // Error
        std::cerr << "Error while loading file" << std::endl;
    }

    xmlBOM.setContent(&f);

    f.close();


    QFile repairs("AvailableRepairs.xml");
    if (!f.open(QIODevice::ReadOnly))
    {
        // Error
        std::cerr << "Error while loading repairs XML file" << std::endl;
    }


    xmlRepairs.setContent(&repairs);
    repairs.close();
}

void MainWindow::readPhonesXML()
{
    // Extraemos el root markup
    QDomElement root = xmlBOM.documentElement();

    // COgemos el primer hijo del root
    QDomElement Component = root.firstChild().toElement();


    // Loop mientras haya un hijo
    while (!Component.isNull())
    {


        if (Component.tagName() == "Phone")
        {

            // Cogemos el primer hijo de phone
            QDomElement Child = Component.firstChild().toElement();

            QString Name;

            // leemos cada hijo del componente nodo
            while (!Child.isNull())
            {

                // leemos el nombre
                if (Child.tagName() == "Name")
                    Name = Child.firstChild().toText().data();


                Child = Child.nextSibling().toElement();
            }

            // ponemos en el comboBox los moviles disponibles
            ui->comboBox->addItem(Name.toStdString().c_str());
        }


        // Siguiente componente
        Component = Component.nextSibling().toElement();
    }
}

void MainWindow::readRepairsXML()
{
    QDomElement root = xmlRepairs.documentElement();

    QDomElement Component = root.firstChild().toElement();

    while (!Component.isNull())
    {
        if (Component.tagName() == "Phone")
        {

            QDomElement Child = Component.firstChild().toElement();

            QString name;
            QString repair;
            while (!Child.isNull())
            {

                if (Child.tagName() == "Name")
                    name = Child.firstChild().toText().data();
                if (name == ui->comboBox->currentText())
                {
                    Child = Child.nextSibling().toElement();
                    if (Child.tagName() == "Repair")
                    {
                        repair = Child.firstChild().toText().data();
                        ui->comboBox_2->addItem(repair);
                    }
                }


                Child = Child.nextSibling().toElement();
            }
        }

        Component = Component.nextSibling().toElement();
    }
}

void MainWindow::readPricesXML()
{
    QDomElement root = xmlRepairs.documentElement();

    QDomElement Component = root.firstChild().toElement();

    while (!Component.isNull())
    {
        if (Component.tagName() == "Phone")
        {

            QDomElement Child = Component.firstChild().toElement();

            QString name;
            QString repair;
            while (!Child.isNull())
            {
                if (Child.tagName() == "Name")
                    name = Child.firstChild().toText().data();
                if (name == ui->comboBox->currentText())
                {
                    Child = Child.nextSibling().toElement();
                    if (Child.tagName() == "Repair")
                    {
                        repair = Child.firstChild().toText().data();
                        Child = Child.nextSibling().toElement();
                        if (repair == ui->comboBox_2->currentText())
                        {
                            ui->label_price->setText("Price: " + Child.firstChild().toText().data());
                        }
                    }
                }


                Child = Child.nextSibling().toElement();
            }
        }

        Component = Component.nextSibling().toElement();
    }
}

void MainWindow::readFoundedOrderXML()
{
    QFile file("FoundedOrder.xml");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "Failed to open file for writting";
    }
    else
    {
        QTextStream stream(&file);
        stream << xmlFoundedOrder.toString();
        file.close();
    }

    if (validatexml("FoundedOrder.xml", "FoundedOrder.xsd"))
    {
        QDomElement root = xmlFoundedOrder.documentElement();

        QDomElement Component = root.firstChild().toElement();

        QString status;

        while (!Component.isNull())
        {
            if (Component.tagName() == "Order")
            {
                QDomElement Child = Component.firstChild().toElement();

                while (!Child.isNull())
                {
                    if (Child.tagName() == "Status")
                        status = Child.firstChild().toText().data();

                    Child = Child.nextSibling().toElement();
                }
            }
            Component = Component.nextSibling().toElement();
        }
        if (status == "done")
        {
            ui->result_label->setStyleSheet({ "QLabel{ color:green;}" });
        }
        else
        {
            ui->result_label->setStyleSheet({ "" });
        }
        ui->result_label->setText(status);
    }
}

void MainWindow::readLoginXML()
{
    QFile file("Login2.xml");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "Failed to open file for writting";
    }
    else
    {
        QTextStream stream(&file);
        stream << xmlLogin.toString();
        file.close();
    }

    if (validatexml("Login2.xml", "Login2.xsd"))
    {
        QDomElement root = xmlLogin.documentElement();

        QDomElement Component = root.firstChild().toElement();

        QString user;

        while (!Component.isNull())
        {
            if (Component.tagName() == "Login")
            {
                QDomElement Child = Component.firstChild().toElement();

                while (!Child.isNull())
                {
                    if (Child.tagName() == "Username")
                        user = Child.firstChild().toText().data();

                    Child = Child.nextSibling().toElement();
                }
            }
            Component = Component.nextSibling().toElement();
        }
        if (user != "")
        {
            username = user;
            onLoginSuccessfully();
        }
    }
}

bool MainWindow::validatexml(QString xml, QString xsd)
{
    QFile file(xsd);
    file.open(QIODevice::ReadOnly);

    QXmlSchema schema;
    schema.load(&file, QUrl::fromLocalFile(file.fileName()));

    if (schema.isValid())
    {
        QFile file2(xml);
        file2.open(QIODevice::ReadOnly);

        QXmlSchemaValidator validator(schema);
        if (validator.validate(&file2, QUrl::fromLocalFile(file2.fileName())))
        {
            qDebug() << "instance document is valid";
            return true;
        }
        else
        {
            qDebug() << "instance document is invalid";
            return false;
        }
    }
    else
    {
        qDebug() << "schema is invalid";
    }
}

void MainWindow::writeOrderXML()

{

    QDomDocument document;
    QDomElement root = document.createElement("Orders");


    QDomElement order = document.createElement("Order");
    QDomElement idorder = document.createElement("IdOrder");
    QDomElement phone = document.createElement("Phone");
    QDomElement repair = document.createElement("repair");
    QDomElement action = document.createElement("Action");


    QUuid idordert = QUuid::createUuid();
    QString phonet = ui->comboBox->currentText();
    QString repairt = ui->comboBox_2->currentText();
    QString actiontxt = "order";

    QDomText idordertxt = document.createTextNode(idordert.toString());
    QDomText phonetxt = document.createTextNode(phonet);
    QDomText repairtxt = document.createTextNode(repairt);
    QDomText actionData = document.createTextNode(actiontxt);


    idorder.appendChild(idordertxt);
    repair.appendChild(repairtxt);
    phone.appendChild(phonetxt);
    action.appendChild(actionData);
    order.appendChild(idorder);
    order.appendChild(repair);
    order.appendChild(phone);
    order.appendChild(action);
    root.appendChild(order);
    document.appendChild(root);

    QString message = document.toString();
    m_webSocket->sendXML(message);



    QFile file("newOrder.xml");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "Failed to open file for writting";
    }
    else
    {
        QTextStream stream(&file);
        stream << document.toString();


        file.close();
    }
}

void MainWindow::writeFindOrderXML()
{

    QDomDocument document;
    QDomElement root = document.createElement("Orders");


    QDomElement order = document.createElement("Order");
    QDomElement idorder = document.createElement("IdOrder");
    QDomElement action = document.createElement("Action");

    QString actiontxt = "find";

    QDomText idordertxt = document.createTextNode(ui->lineEdit_2->text());
    QDomText actionData = document.createTextNode(actiontxt);


    idorder.appendChild(idordertxt);
    action.appendChild(actionData);
    order.appendChild(action);
    order.appendChild(idorder);
    root.appendChild(order);
    document.appendChild(root);

    QString message = document.toString();
    m_webSocket->sendXML(message);

    // escribimos en el file
    QFile file("FindOrder.xml");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "Failed to open file for writting";
    }
    else
    {
        QTextStream stream(&file);
        stream << document.toString();

        file.close();
    }
}

void MainWindow::writeLoginXML()
{


    QDomDocument document;
    QDomElement root = document.createElement("LoginInfo");


    QDomElement login = document.createElement("Login");
    QDomElement username = document.createElement("Username");
    QDomElement password = document.createElement("Password");
    QDomElement action = document.createElement("Action");


    QString usernamet = ui->lineEdit_username->text();
    QString passwordt = ui->lineEdit_password->text();
    QString actiontxt = "login";

    QDomText usernametxt = document.createTextNode(usernamet);
    QDomText passwordtxt = document.createTextNode(passwordt);
    QDomText actionData = document.createTextNode(actiontxt);


    username.appendChild(usernametxt);
    password.appendChild(passwordtxt);
    action.appendChild(actionData);
    login.appendChild(username);
    login.appendChild(password);
    login.appendChild(action);
    root.appendChild(login);
    document.appendChild(root);

    QString message = document.toString();
    m_webSocket->sendXML(message);


    QFile file("Login.xml");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "Failed to open file for writting";
    }
    else
    {
        QTextStream stream(&file);
        stream << document.toString();


        file.close();
    }
}


void MainWindow::recibirmensaje(QString message)
{
    QString xmlToValidate = xmlManager.xmlMessage(message);

    if (xmlToValidate == "findOrder")
    {
        QString fileName = xmlManager.makeFiles("find", message);
        if(validatexml(fileName, "find.xsd"))
        {
            xmlFoundedOrder.setContent(message);
            readFoundedOrderXML();
        }
    }
    else if (xmlToValidate == "login")
    {
        if (message == "No")
        {
            QMessageBox::information(this, tr("Error"), tr("Authentication failed"));
        }
        else
        {
            xmlLogin.setContent(message);
            readLoginXML();
        }
    }
    else if (xmlToValidate == "newOrderStatus")
    {
        if(conectado)
        {
            QString xmlName = xmlManager.makeFiles("newOrderStatus", message);
            if (validatexml(xmlName, "newOrderStatus.xsd"))
            {
                auto messages = xmlManager.readOrdersXML(message);
                QString mensaje = "La orden con numero: " + std::get<0>(messages) + ", Ha cambiado a " + std::get<1>(messages);
                QMessageBox::information(this, "Change", mensaje);
            }
        }
    }
}

void MainWindow::onLoginSuccessfully()
{
    conectado = true;
    ui->actionLog_out->setVisible(true);
    ui->actionLogin->setVisible(false);
    ui->tabWidget->tabBar()->show();
    ordersSizeWindow.setHeight(461);
    ordersSizeWindow.setWidth(518);
    ordersSizeLabel.setHeight(201);
    ui->label_client1->setVisible(true);
    ui->label_client2->setVisible(true);
    ui->label_client3->setVisible(true);
    ui->statusOfOrderLabel->setFixedSize(ordersSizeLabel);
    ui->tabWidget->tabBar()->show();
    ui->tabWidget->setCurrentIndex(0);
    ui->tab_3->setEnabled(false);
}

void MainWindow::onLogOutSuccessfully()
{
    conectado = false;
    startConfig();
}

void MainWindow::on_comboBox_currentTextChanged(const QString& arg1)
{
    ui->comboBox_2->clear();
    readRepairsXML();
    if (ui->comboBox_2->currentText() == "")
    {
        ui->comboBox_2->setEnabled(false);
        ui->label_price->setText("There're no repairs for this mobile");
        ui->btn_newRepair->setEnabled(false);
    } else {
        ui->comboBox_2->setEnabled(true);
        ui->btn_newRepair->setEnabled(true);
    }
}


void MainWindow::on_btn_newRepair_clicked()
{
    writeOrderXML();
}

void MainWindow::on_btn_findOrder_clicked()
{
    writeFindOrderXML();
    // ui->result_label->setText(Websocket::statusMessage);
}

void MainWindow::on_actionLogin_triggered()
{
    ui->tabWidget->tabBar()->setCurrentIndex(1);
}

void MainWindow::on_pushButton_2_clicked()
{
    ui->tabWidget->tabBar()->setCurrentIndex(0);
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    switch (index)
    {
        case 0:
            this->setFixedSize(ordersSizeWindow);
            qDebug() << ordersSizeWindow;
            break;
        case 1:
            this->setFixedSize(518, 345);
            break;
        case 2:
            this->setFixedSize(518, 375);
            break;
    }
}

void MainWindow::on_btn_login_clicked()
{
    writeLoginXML();
}

void MainWindow::on_actionLog_out_triggered()
{
    onLogOutSuccessfully();
}

void MainWindow::on_comboBox_2_currentIndexChanged(const QString& arg1)
{
    readPricesXML();
}
