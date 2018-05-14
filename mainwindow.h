#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include<QMainWindow>
#include<QGraphicsView>
#include<QGraphicsScene>
#include<QVBoxLayout>
#include<QLabel>
#include<QtGui>
#include<QLineEdit>
#include<QCompleter>
#include<QStringList>
#include"constants.h"
class QGraphicsScene;
class QGraphicsView;
class GameController;


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void setLabel(QString s);
    void setTell(QString s);
private slots:
    void adjustViewSize();

private:
    void initScene();
    void initSceneBackground();

    QVBoxLayout *layout;
    QWidget* widget;
    QLabel* label;
    QLabel* tell;
    QGraphicsScene* scene;
    QGraphicsView* view;
    GameController* game;
};

#endif // MAINWINDOW_H
