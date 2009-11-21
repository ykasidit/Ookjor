#ifndef SELECTPHONEDIALOG_H
#define SELECTPHONEDIALOG_H

#include <QtGui/QDialog>

namespace Ui {
    class SelectPhoneDialog;
}

class SelectPhoneDialog : public QDialog {
    Q_OBJECT
    Q_DISABLE_COPY(SelectPhoneDialog)
public:
    explicit SelectPhoneDialog(QWidget *parent = 0);
    virtual ~SelectPhoneDialog();

protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::SelectPhoneDialog *m_ui;
};

#endif // SELECTPHONEDIALOG_H
