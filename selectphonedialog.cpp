#include "selectphonedialog.h"
#include "ui_selectphonedialog.h"

SelectPhoneDialog::SelectPhoneDialog(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::SelectPhoneDialog)
{
    m_ui->setupUi(this);
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
