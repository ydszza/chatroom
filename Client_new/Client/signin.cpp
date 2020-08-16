#include "signin.h"
#include "ui_signin.h"
#include <QDebug>
#include <QMessageBox>
#include <cstring>

SignIn::SignIn(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SignIn)
{
    ui->setupUi(this);

    this->setWindowTitle("Signin");
    this->setFixedSize(400, 300);

    this->home = nullptr;

    isCreate = false;
    ui->labelNickname->hide();
    ui->lineEditNickname->hide();

    connect(ui->pushButtonSignin, SIGNAL(clicked(bool)), this, SLOT(slotBtnSigninClick()));
    connect(ui->pushButtonCreate, SIGNAL(clicked(bool)), this, SLOT(slotBtnCreateClick()));

    this->msgManage = new MsgManage;
    //connect signin button and msgmanage
    connect(this, SIGNAL(signalSignin(QString, QString)), msgManage, SLOT(slotSignin(QString, QString)));
    connect(this, SIGNAL(signalCreate(QString, QString, QString)), msgManage, SLOT(slotCreate(QString, QString, QString)));
    connect(msgManage, SIGNAL(signalSignResult(QString)), this, SLOT(slotSigninResult(QString)));
    connect(msgManage, SIGNAL(signalCreateResult(char)), this, SLOT(slotCreateResult(char)));
}

SignIn::~SignIn()
{
    delete ui;
    if(!home) delete home;
    delete msgManage;
}

void SignIn::slotBtnSigninClick()
{
    //disable button
    ui->pushButtonSignin->setEnabled(false);
    ui->pushButtonCreate->setEnabled(false);

    //check which btn click
    account = ui->lineEditAccount->text();
    password = ui->lineEditPassword->text();
    nickname = ui->lineEditNickname->text();

    //check param is invail
    if(account.isEmpty() && password.isEmpty()) {
        QMessageBox::information(this, tr("error"), tr("account or password invaild"));
        ui->pushButtonSignin->setEnabled(true);
        ui->pushButtonCreate->setEnabled(true);
        return;
    }
    if(isCreate) {
        if(nickname.isEmpty()) {
            QMessageBox::information(this, tr("error"), tr("nickname invaild"));
            ui->pushButtonSignin->setEnabled(true);
            ui->pushButtonCreate->setEnabled(true);
        }
        else
            emit signalCreate(account, password, nickname);
        return;
    }
    //emit signal
    emit signalSignin(account, password);
}

void SignIn::slotBtnCreateClick()
{ 
    //disable button
    ui->pushButtonCreate->setEnabled(false);

    //clear edit text
    ui->lineEditAccount->clear();
    ui->lineEditNickname->clear();
    ui->lineEditPassword->clear();

    //change the btn text and show or hide the lineEditNickname
    if(!isCreate) {
        isCreate = true;
        ui->pushButtonSignin->setText("Create");
        ui->pushButtonCreate->setText("Signin");
        ui->labelNickname->show();
        ui->lineEditNickname->show();
    } else{
        isCreate = false;
        ui->pushButtonSignin->setText("Signin");
        ui->pushButtonCreate->setText("Create");
        ui->labelNickname->hide();
        ui->lineEditNickname->hide();
    }
    ui->pushButtonCreate->setEnabled(true);
}

void SignIn::slotSigninResult(QString nickname)
{
    if(nickname.isEmpty()) { //sign in error
        QMessageBox::information(this, tr("error"), tr("sign in error"));
        msgManage->closeConnect();
    } else { //sign in success
        qDebug() << nickname;

        this->home = new Home(account, nickname);
        connect(msgManage, SIGNAL(signalClientState(ClientInfo *)), this->home, SLOT(slotClientInfo(ClientInfo *)));
        connect(msgManage, SIGNAL(signalGetNewMsg(Message *)), this->home, SLOT(slotGetNewMsg(Message *)));
        connect(msgManage, SIGNAL(signalUpdateResult(char)), this->home, SLOT(slotUpDate(char)));
        connect(this->home, SIGNAL(signalHomeSendMsg(QString*)), msgManage, SLOT(slotSendData(QString*)));
        this->hide();
        home->show();
    }
    ui->pushButtonSignin->setEnabled(true);
    ui->pushButtonCreate->setEnabled(true);
}

void SignIn::slotCreateResult(char flag)
{
    if(flag) { //create success
        QMessageBox::information(this, tr("success"), tr("create success"));

        //back to signin page
        slotBtnCreateClick();

    } else { //others case
        QMessageBox::information(this, tr("error"), tr("create error"));
    }
    ui->pushButtonSignin->setEnabled(true);
    ui->pushButtonCreate->setEnabled(true);
    //change to signin
    //...
}
