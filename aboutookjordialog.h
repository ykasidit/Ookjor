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
