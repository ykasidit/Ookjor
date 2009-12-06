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

#ifndef OOKJORWINDOW_H
#define OOKJORWINDOW_H

#include <QtGui/QMainWindow>
#include "ookjorengine.h"
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QPixmap>

namespace Ui
{
    class OokjorWindow;
}

class OokjorWindow : public QMainWindow
{
    Q_OBJECT

public:
    OokjorWindow(QWidget *parent = 0);
    ~OokjorWindow();


 public slots:    
 void EngineStateChangeSlot(int aState);
 void EngineStatusMessageSlot(QString str);
 void GotNewJpgSlot();
 void OnMenuHelp();
 void OnMenuAbout();


private:
    Ui::OokjorWindow *ui;
    OokjorEngine* iCOokjorEngine;

     QGraphicsScene iScene;     
     QGraphicsPixmapItem iPixmapItem;
     QPixmap iPixmap;


private slots:
    void on_connectPrevCheckBox_clicked();
    void on_startSendButton_clicked();
    void on_liveDisconnectButton_clicked();
    void on_connectButton_clicked();

   protected:
    QByteArray iPrevDevAddr;
};

#endif // OOKJORWINDOW_H
