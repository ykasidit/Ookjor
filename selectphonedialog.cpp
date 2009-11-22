#include "selectphonedialog.h"
#include "ui_selectphonedialog.h"

SelectPhoneDialog::SelectPhoneDialog(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::SelectPhoneDialog)
{
    m_ui->setupUi(this);
    iReturnSelectedIndex = NULL;
}

void SelectPhoneDialog::SetList(QList<OokjorEngine::TBtDevInfo>& aList, int* aReturnSelectedIndex)
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
  *iReturnSelectedIndex = index.column();
  this->close();
}
