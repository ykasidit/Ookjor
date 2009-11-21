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
#include "selectphonedialog.h"


OokjorWindow::OokjorWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::OokjorWindow)
{
    ui->setupUi(this);
    iCOokjorEngine = NULL;

    iCOokjorEngine = new OokjorEngine();

      QObject::connect(iCOokjorEngine, SIGNAL(SearchCompleteSignal(int)),this, SLOT (SearchCompleteSlot(int)));

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
    ui->statusBar->showMessage("Searching...",3000);
    ui->pushButton->setEnabled(false);

    if(!iCOokjorEngine->StartSearch())
    {
        ui->statusBar->showMessage("Start search failed...",3000);
        ui->pushButton->setEnabled(true);
    }

}


void OokjorWindow::SearchCompleteSlot(int res)
{

    QList<OokjorEngine::TBtDevInfo> devlist;
    iCOokjorEngine->GetDevListClone(devlist);
//    QString str;
  //  str = str.number(devlist.count());
//ui->statusBar->showMessage(str,3000);

    SelectPhoneDialog w;
    w.exec();

    ui->pushButton->setEnabled(true);

}
