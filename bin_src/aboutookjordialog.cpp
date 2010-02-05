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

#include "aboutookjordialog.h"
#include "ui_aboutookjordialog.h"
#include <QFileInfo>

aboutookjordialog::aboutookjordialog(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::aboutookjordialog)
{
    m_ui->setupUi(this);

    QString readmepath;
    readmepath += QFileInfo( QCoreApplication::argv()[0] ).absolutePath();
    readmepath += "/../README";

    QFile f( readmepath );
    if(f.open(QIODevice::ReadOnly))
    {
        QString s(f.readAll());
        m_ui->textBrowser->setFrameStyle(QFrame::Plain);
        m_ui->textBrowser->setText(s);
        f.close();
    }
    else
        qDebug("open README failed");
}

aboutookjordialog::~aboutookjordialog()
{
    delete m_ui;
}

void aboutookjordialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
