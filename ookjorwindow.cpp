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



OokjorWindow::OokjorWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::OokjorWindow)
{
    ui->setupUi(this);
    iCOokjorEngine = NULL;

    iCOokjorEngine = new OokjorEngine(this);

    QObject::connect(iCOokjorEngine, SIGNAL(EngineStateChangeSignal(int)),this, SLOT (EngineStateChangeSlot(int)));
    QObject::connect(iCOokjorEngine, SIGNAL(EngineStatusMessageSignal(QString)),this, SLOT (EngineStatusMessageSlot(QString)));


    //test load pic
    iPixmap.load("gnu.jpg");
    /////////////

   iPixmapItem.setPixmap(iPixmap);
   iScene.addItem(&iPixmapItem);   

    ui->graphicsView->setScene(&iScene);
}

OokjorWindow::~OokjorWindow()
{
    delete ui;
    delete iCOokjorEngine;
}

void OokjorWindow::on_pushButton_clicked()
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
        ui->pushButton->setText("Connect to Mobile");
        ui->pushButton->setEnabled(true);
        break;
    case OokjorEngine::EBtSearching:
        ui->pushButton->setEnabled(false);
        ui->pushButton->setText("Please wait...");

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

        break;
    case OokjorEngine::EBtDisconnected:

        break;
    default:
        ui->statusBar->showMessage("WARNING: UNKNOWN ENGINE STATE");
        break;
    }
}
