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

    void SetList(QList<OokjorEngine::TBtDevInfo>& aList, int* aReturnSelectedIndex);

protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::SelectPhoneDialog *m_ui;
    int* iReturnSelectedIndex;

private slots:
    void on_listWidget_clicked(QModelIndex index);
};

#endif // SELECTPHONEDIALOG_H
