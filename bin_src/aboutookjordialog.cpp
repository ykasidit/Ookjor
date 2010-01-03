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
