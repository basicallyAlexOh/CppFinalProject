#include "ui/MainWindow.h"

#include <QApplication>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QLabel>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("My Qt6 Application");

    createMenus();
    createCentralWidget();
}

MainWindow::~MainWindow() = default;

void MainWindow::setupUI()
{
    // ---- DEFAULT WINDOW SIZE ----
    resize(800, 600);   // Or setFixedSize(), or setMinimumSize()
}


void MainWindow::createMenus() {
    // File menu
    fileMenu = menuBar()->addMenu("&File");

    quitAction = new QAction("Quit", this);
    fileMenu->addAction(quitAction);

    connect(quitAction, &QAction::triggered,
            qApp, &QApplication::quit);
}

void MainWindow::createCentralWidget() {
    // Simple label inside a layout
    auto *label = new QLabel("Hello from Qt6!", this);
    label->setAlignment(Qt::AlignCenter);

    auto *layout = new QVBoxLayout;
    layout->addWidget(label);

    // Final widget
    central = new QWidget(this);
    central->setLayout(layout);

    setCentralWidget(central);
}
