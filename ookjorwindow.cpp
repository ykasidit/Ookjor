/*
    Copyright (C) 2009 Kasidit Yusuf.

    This file is part of Ookjor.

    Ookjor is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Ookjor is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Ookjor.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "ookjorwindow.h"
#include "ui_ookjorwindow.h"
#include <QMessageBox>
#include "aboutookjordialog.h"
#include <QGraphicsItemAnimation>
#include <QTimeLine>
#include <QGraphicsTextItem>
#include <QMovie>

OokjorWindow::OokjorWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::OokjorWindow)
{
    ui->setupUi(this);
    iCOokjorEngine = NULL;

    iCOokjorEngine = new OokjorEngine(this);

    QObject::connect(iCOokjorEngine, SIGNAL(EngineStateChangeSignal(int)),this, SLOT (EngineStateChangeSlot(int)));
    QObject::connect(iCOokjorEngine, SIGNAL(EngineStatusMessageSignal(QString)),this, SLOT (EngineStatusMessageSlot(QString)));
    QObject::connect(iCOokjorEngine, SIGNAL(GotNewJpgSignal(void)),this, SLOT (GotNewJpgSlot(void)));

    //test load pic
    //iPixmap.load("gnu.jpg");
    /////////////

   iPixmapItem.setPixmap(iPixmap);
   iScene.addItem(&iPixmapItem);

   QMovie* gif = new QMovie(":/images/ajax-loader.gif");
   ui->connectLoadingLabel->setMovie(gif);
   gif->start();
   ///////////test
   /*QGraphicsItem *ball = new QGraphicsEllipseItem(0, 0, 20, 20);
   QGraphicsTextItem *ball = new QGraphicsTextItem ();
   ball->setHtml("<big><>Press Connect to Mobile</big>");

     QTimeLine *timer = new QTimeLine(5000);
     timer->setFrameRange(0, 100);

     QGraphicsItemAnimation *animation = new QGraphicsItemAnimation;
     animation->setItem(ball);
     animation->setTimeLine(timer);

     for (int i = 0; i < 200; ++i)
         animation->setPosAt(i / 200.0, QPointF(i, i));


     //iScene.setSceneRect(0, 0, 250, 250);
     iScene.addItem(ball);


     timer->start();
     */
   ////////


    ui->liveView->setScene(&iScene);
    //ui->liveView->hide();

    EngineStateChangeSlot(OokjorEngine::EBtIdle);

}

void OokjorWindow::GotNewJpgSlot()
{
bool loadsuccess = iPixmap.loadFromData(iCOokjorEngine->iNewJpgBuffer,"JPG");
if(loadsuccess)
{

        EngineStatusMessageSlot("img load ok");
        iPixmapItem.setPixmap(iPixmap);
        iScene.setSceneRect(iPixmapItem.boundingRect());
        //ui->liveView->update();

    }
    else
        EngineStatusMessageSlot("img load failed");


}



OokjorWindow::~OokjorWindow()
{
    delete ui;
    delete iCOokjorEngine;
}

void OokjorWindow::on_connectButton_clicked()
{
    if(!iCOokjorEngine->StartSearch())
    {
        ui->statusBar->showMessage("Start search failed...",3000);        
    }
}


void OokjorWindow::EngineStatusMessageSlot(QString str)
{
    ui->statusBar->showMessage(str);
}

void OokjorWindow::EngineStateChangeSlot(int aState)
{
    switch(aState)
    {
    case OokjorEngine::EBtIdle:

        ui->liveView->hide();
        ui->liveDisconnectButton->hide();

        ui->connectLoadingLabel->hide();
        ui->connectButton->setText("Connect to Mobile");
        ui->connectButton->setEnabled(true);

        ui->groupBox_1->show();
        ui->groupBox_2->show();

        this->adjustSize();
        //this->resize(this->minimumSize());

        break;
    case OokjorEngine::EBtSearching:
        ui->groupBox_1->hide();

        ui->connectButton->setEnabled(false);
        ui->connectButton->setText("Please wait...");
        ui->connectLoadingLabel->show();

        this->adjustSize();
        //this->resize(this->minimumSize());
        break;
    case OokjorEngine::EBtSelectingPhoneToSDP:

        break;
    case OokjorEngine::EBtSearchingSDP:

        break;
    case OokjorEngine::EBtSearchingSDPDone:

        break;
    case OokjorEngine::EBtConnectingRFCOMM:

        break;
    case OokjorEngine::EBtConnectionActive:

        ui->groupBox_2->hide();

    //ui->textBrowser->hide();
    ui->liveView->show();
    ui->liveDisconnectButton->setEnabled(true);
    ui->liveDisconnectButton->show();
    //this->resize(this->minimumSize());

    this->adjustSize();
        break;
    case OokjorEngine::EBtDisconnected:

        break;
    default:
        ui->statusBar->showMessage("WARNING: UNKNOWN ENGINE STATE");
        break;
    }
}

void OokjorWindow::OnMenuAbout()
{
//QMessageBox::information(this, tr("About"),tr(""));
      aboutookjordialog w(this);
      w.exec();
}

void OokjorWindow::OnMenuHelp()
{
//QMessageBox::information(this, tr("Help"),tr("Please visit www.ClearEvo.com for details."));
}

void OokjorWindow::on_liveDisconnectButton_clicked()
{
    ui->liveDisconnectButton->setEnabled(false);
    iCOokjorEngine->Disconnect();
}
