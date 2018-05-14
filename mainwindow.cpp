#include "mainwindow.h"
#include"constants.h"
#include"gamecontroller.h"
#include<QGraphicsView>
#include<QGraphicsScene>
#include<QStatusBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      scene(new QGraphicsScene(this)),
      view(new QGraphicsView(scene,this)),
      game(new GameController(this,*scene,this))
{

    label = new QLabel;
    label->setMinimumSize(label->sizeHint());
    label->setAlignment(Qt::AlignLeft);
    label->setText(">>");

    tell = new QLabel;
    tell->setMinimumSize(label->sizeHint());
    tell->setAlignment(Qt::AlignRight);
    statusBar()->addWidget(label);
    statusBar()->addWidget(tell);

    widget = new QWidget;
    layout = new QVBoxLayout;
    layout->addWidget(view);
    setCentralWidget(widget);
    widget->setLayout(layout);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    resize(820,640);

    initScene();
    initSceneBackground();

    QTimer::singleShot(0, this,&MainWindow::adjustViewSize);
}

void MainWindow::setLabel(QString s)
{
    label->setText(">>"+s);
}

void MainWindow::setTell(QString s)
{
    if(s!="")
    {
        tell->setText("!!"+s);
    }
    else
    {
        tell->setText("");
    }

}

MainWindow::~MainWindow()
{

}

void MainWindow::adjustViewSize()
{
    view->fitInView(scene->sceneRect(), Qt::KeepAspectRatioByExpanding);
}

void MainWindow::initScene()
{
    scene->setSceneRect(-200,-150,400,300);
}

void MainWindow::initSceneBackground()
{
    QPixmap bg(TILE_SIZE,TILE_SIZE);
    QPainter p(&bg);
    p.setBrush(QBrush(QColor("#A78D7A")));
    p.drawRect(0,0,TILE_SIZE, TILE_SIZE);

    view->setBackgroundBrush(QBrush(bg));
}
