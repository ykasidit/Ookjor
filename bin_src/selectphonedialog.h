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

#ifndef SELECTPHONEDIALOG_H
#define SELECTPHONEDIALOG_H

#include <QtGui/QDialog>
#include "ookjorengine.h"
#include <QModelIndex>

namespace Ui {
    class SelectPhoneDialog;
}

class SelectPhoneDialog : public QDialog {
    Q_OBJECT
    Q_DISABLE_COPY(SelectPhoneDialog)
public:
    explicit SelectPhoneDialog(QWidget *parent = 0);
    virtual ~SelectPhoneDialog();

    void SetList(QList<TBtDevInfo>& aList, int* aReturnSelectedIndex);

protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::SelectPhoneDialog *m_ui;
    int* iReturnSelectedIndex;

private slots:
    void on_listWidget_clicked(QModelIndex index);
};

#endif // SELECTPHONEDIALOG_H
