#include "aboutookjordialog.h"
#include "ui_aboutookjordialog.h"

aboutookjordialog::aboutookjordialog(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::aboutookjordialog)
{
    m_ui->setupUi(this);
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
