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

#include "selectphonedialog.h"
#include "ui_selectphonedialog.h"

SelectPhoneDialog::SelectPhoneDialog(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::SelectPhoneDialog)
{
    m_ui->setupUi(this);
    iReturnSelectedIndex = NULL;
}

void SelectPhoneDialog::SetList(QList<TBtDevInfo>& aList, int* aReturnSelectedIndex)
{
    iReturnSelectedIndex = aReturnSelectedIndex;

    for(int i=0;i<aList.count();i++)
    {
        QString str;
        str = aList[i].iName;
        str += "  (";
        str += aList[i].iAddrStr;
        str += " )";
        m_ui->listWidget->addItem(str);
        //m_ui->listWidget->selectedIndexes(
    }
}

SelectPhoneDialog::~SelectPhoneDialog()
{
    delete m_ui;
}

void SelectPhoneDialog::changeEvent(QEvent *e)
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

void SelectPhoneDialog::on_listWidget_clicked(QModelIndex index)
{
  *iReturnSelectedIndex = index.row();
  this->close();
}
