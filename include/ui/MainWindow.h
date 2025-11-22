#pragma once

#include <QMainWindow>

class QAction;
class QMenu;
class QWidget;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    // Private helper functions
    void setupUI();
    void createMenus();
    void createCentralWidget();



    // Menu items
    QMenu* fileMenu = nullptr;
    QAction* quitAction = nullptr;

    // Central widget example
    QWidget* central = nullptr;
};
