#pragma once

#include <QMainWindow>
#include "core/TrackManager.h"

class QListWidget;
class QPushButton;
class QMenu;
class QAction;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    // UI helpers
    void setupUI();
    void createMenus();
    void createCentralWidget();
    void refreshTrackList();

    // Actions / slots
    void openFile();
    void saveSelectedTrack();
    void saveMix();
    void saveMixRange();

    void reverseTrack();
    void speedTrack();
    void speedResampleTrack();
    void pitchTrack();
    void deleteTrack();
    void mergeTracks();

    // Data
    TrackManager manager;

    // Widgets
    QListWidget *trackList = nullptr;

    QPushButton *reverseBtn = nullptr;
    QPushButton *speedBtn   = nullptr;
    QPushButton *resampleBtn = nullptr;
    QPushButton *pitchBtn   = nullptr;
    QPushButton *deleteBtn  = nullptr;
    QPushButton *mergeBtn   = nullptr;

    QWidget *central = nullptr;

    // Menus / actions
    QMenu   *fileMenu        = nullptr;
    QAction *openAction      = nullptr;
    QAction *saveTrackAction = nullptr;
    QAction *saveMixAction   = nullptr;
    QAction *saveRange       = nullptr;
    QAction *quitAction      = nullptr;
};
