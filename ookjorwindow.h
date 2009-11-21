#ifndef OOKJORWINDOW_H
#define OOKJORWINDOW_H

#include <QtGui/QMainWindow>
#include "ookjorengine.h"
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QPixmap>

namespace Ui
{
    class OokjorWindow;
}

class OokjorWindow : public QMainWindow
{
    Q_OBJECT

public:
    OokjorWindow(QWidget *parent = 0);
    ~OokjorWindow();


 public slots:    
    void SearchCompleteSlot(int res);


private:
    Ui::OokjorWindow *ui;
    OokjorEngine* iCOokjorEngine;

     QGraphicsScene iScene;     
     QGraphicsPixmapItem iPixmapItem;
     QPixmap iPixmap;

private slots:
    void on_pushButton_clicked();
};

#endif // OOKJORWINDOW_H
