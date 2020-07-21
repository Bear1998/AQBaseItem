#include "mainwindow.h"
#include "aqbaseitem.h"
#include <QGraphicsView>
#include <QGraphicsScene>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QGraphicsView *main_view = new QGraphicsView;
    QGraphicsScene *main_scene = new QGraphicsScene;
    main_scene->setSceneRect(0,0,600,600);
    main_view->setScene(main_scene);
    this->setCentralWidget(main_view);

    PathItem *item = new PathItem();
    main_scene->addItem(item);

}


MainWindow::~MainWindow()
{

}
