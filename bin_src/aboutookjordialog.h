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

#ifndef ABOUTOOKJORDIALOG_H
#define ABOUTOOKJORDIALOG_H

#include <QtGui/QDialog>

namespace Ui {
    class aboutookjordialog;
}

class aboutookjordialog : public QDialog {
    Q_OBJECT
public:
    aboutookjordialog(QWidget *parent = 0);
    ~aboutookjordialog();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::aboutookjordialog *m_ui;
};

#endif // ABOUTOOKJORDIALOG_H
