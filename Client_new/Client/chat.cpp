#include "chat.h"
#include <QDebug>

Chat::Chat(QWidget *parent) : QWidget(parent)
{
    this->setFixedSize(600, 450);
    this->setWindowTitle("Chat");

    this->content = new QTextBrowser(this);
    this->sendMsg = new QTextEdit(this);
    this->sendBtn = new QPushButton(this);
    this->clearBtn = new QPushButton(this);

    this->content->setGeometry(0, 0, 600, 280);
    this->sendMsg->setGeometry(0, 280, 600, 120);
    this->sendBtn->setText("Send");
    this->sendBtn->setGeometry(this->width()-20-60, this->height()-10-24, 60, 24);
    this->clearBtn->setText("Clear");
    this->clearBtn->setGeometry(this->width()-20-70-60, this->height()-10-24, 60, 24);

    connect(clearBtn, SIGNAL(clicked()), this, SLOT(slotClearContent()));
    connect(sendBtn, SIGNAL(clicked()), this, SLOT(slotSendBtn()));
}

Chat::~Chat()
{
    delete content;
    delete sendMsg;
    delete sendBtn;
    delete clearBtn;
}

void Chat::slotClearContent()
{
    qDebug("clear");
    this->content->clear();
}

void Chat::slotSendBtn()
{
    qDebug() << sendMsg->toPlainText();
    QString *msg = new QString(sendMsg->toPlainText());
    if(msg->isEmpty()) return;
    qDebug() << *msg;
    emit signalSendMsg(msg);
    sendMsg->clear();
}

void Chat::appendMsg(QString msg)
{
    content->append(msg);
}
