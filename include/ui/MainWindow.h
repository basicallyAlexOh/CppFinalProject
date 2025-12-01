#pragma once

#include <QMainWindow>
#include "core/TrackManager.h"

class QListWidget;
class QPushButton;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr) ;
    ~MainWindow() ;

private slots:
    void openFile();        // File → Open…
    void reverseTrack();    // Reverse selected track
    void speedTrack();      // Change speed
    void pitchTrack();      // Change pitch
    void deleteTrack();     // Delete selected
    void mergeTracks();     // Merge two selected tracks

private:
    void setupUI();
    void createMenus();
    void createCentralWidget();
    void refreshTrackList();

    // Model
    TrackManager manager{44100};

    // Menu items
    QMenu*  fileMenu   = nullptr;
    QAction* openAction = nullptr;
    QAction* quitAction = nullptr;

    // Central widgets
    QWidget*    central    = nullptr;
    QListWidget* trackList = nullptr;
    QPushButton* reverseBtn = nullptr;
    QPushButton* speedBtn   = nullptr;
    QPushButton* pitchBtn   = nullptr;
    QPushButton* deleteBtn  = nullptr;
    QPushButton* mergeBtn   = nullptr;
};

