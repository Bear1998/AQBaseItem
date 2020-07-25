#include <QGraphicsView>
#include <QGraphicsScene>

#include "mainwindow.h"
#include "graphics_obj.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QGraphicsView *main_view = new QGraphicsView;
    QGraphicsScene *main_scene = new QGraphicsScene;
    main_scene->setSceneRect(0,0,600,600);
    main_view->setScene(main_scene);
    this->setCentralWidget(main_view);

    main_scene->addItem(new GraphicsRectItem(QRect(100, 100, 100, 100), false));
    main_scene->addItem(new GraphicsRectItem(QRect(300, 300, 80, 50), false));
}


MainWindow::~MainWindow()
{

}
