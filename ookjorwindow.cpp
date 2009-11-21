#include "ookjorwindow.h"
#include "ui_ookjorwindow.h"

OokjorWindow::OokjorWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::OokjorWindow)
{
    ui->setupUi(this);
    iCOokjorEngine = NULL;

    iCOokjorEngine = new OokjorEngine();

      QObject::connect(iCOokjorEngine, SIGNAL(SearchCompleteSignal(int)),this, SLOT (SearchCompleteSlot(int)));

    //test load pic
    iPixmap.load("gnu.jpg");
    /////////////

   iPixmapItem.setPixmap(iPixmap);
   iScene.addItem(&iPixmapItem);   

    ui->graphicsView->setScene(&iScene);
}

OokjorWindow::~OokjorWindow()
{
    delete ui;
    delete iCOokjorEngine;
}

void OokjorWindow::on_pushButton_clicked()
{
    ui->statusBar->showMessage("Searching...",3000);
    ui->pushButton->setEnabled(false);

    if(!iCOokjorEngine->StartSearch())
    {
        ui->statusBar->showMessage("Start search failed...",3000);
        ui->pushButton->setEnabled(true);
    }

}


void OokjorWindow::SearchCompleteSlot(int res)
{
    ui->statusBar->showMessage("Search finished",3000);
    ui->pushButton->setEnabled(true);

}
