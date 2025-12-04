#include "ui/MainWindow.h"

#include <QApplication>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QListWidget>
#include <QPushButton>
#include <QFileDialog>
#include <QInputDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QDebug>

#include <algorithm>
#include <stdexcept>

//Small helper for single selection
static bool getSingleSelection(QListWidget *list, int &index)
{
    index = list->currentRow();
    return index >= 0;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , manager(44100)    //or whatever sample rate you want
{
    setWindowTitle("Simple DAW");
    setupUI();
    createMenus();
    createCentralWidget();
}

MainWindow::~MainWindow() = default;

void MainWindow::setupUI()
{
    resize(1000, 600);
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu("&File");

    openAction      = new QAction("Open", this);
    saveTrackAction = new QAction("Save Selected Track", this);
    saveMixAction   = new QAction("Save Mix (All Tracks)", this);
    saveRange       = new QAction("Save Mix Range (All Tracks)", this);
    quitAction      = new QAction("Quit", this);

    fileMenu->addAction(openAction);
    fileMenu->addAction(saveTrackAction);
    fileMenu->addAction(saveMixAction);
    fileMenu->addAction(saveRange);
    fileMenu->addSeparator();
    fileMenu->addAction(quitAction);

    connect(openAction,      &QAction::triggered, this, &MainWindow::openFile);
    connect(saveTrackAction, &QAction::triggered, this, &MainWindow::saveSelectedTrack);
    connect(saveMixAction,   &QAction::triggered, this, &MainWindow::saveMix);
    connect(saveRange,       &QAction::triggered, this, &MainWindow::saveMixRange);
    connect(quitAction,      &QAction::triggered, qApp, &QApplication::quit);
}

void MainWindow::createCentralWidget()
{
    trackList = new QListWidget(this);

    trackList->setSelectionMode(QAbstractItemView::ExtendedSelection);

    reverseBtn = new QPushButton("Reverse", this);
    speedBtn   = new QPushButton("Speed", this);
    resampleBtn= new QPushButton("Speed Resample", this);
    nodistortBtn=new QPushButton("Speed Resample No Distort", this);
    pitchBtn   = new QPushButton("Pitch", this);
    deleteBtn  = new QPushButton("Delete", this);
    mergeBtn   = new QPushButton("Merge 2", this);

    auto *buttonLayout = new QVBoxLayout;
    buttonLayout->addWidget(reverseBtn);
    buttonLayout->addWidget(speedBtn);
    buttonLayout->addWidget(resampleBtn);
    buttonLayout->addWidget(nodistortBtn);
    buttonLayout->addWidget(pitchBtn);
    buttonLayout->addWidget(deleteBtn);
    buttonLayout->addWidget(mergeBtn);
    buttonLayout->addStretch();

    auto *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(trackList, /*stretch*/ 3);
    mainLayout->addLayout(buttonLayout, /*stretch*/ 1);

    central = new QWidget(this);
    central->setLayout(mainLayout);
    setCentralWidget(central);

    // Connect buttons to actions
    connect(reverseBtn, &QPushButton::clicked, this, &MainWindow::reverseTrack);
    connect(speedBtn,   &QPushButton::clicked, this, &MainWindow::speedTrack);
    connect(resampleBtn,&QPushButton::clicked, this, &MainWindow::speedResampleTrack);
    connect(nodistortBtn,&QPushButton::clicked, this, &MainWindow::speedNoDistort);
    connect(pitchBtn,   &QPushButton::clicked, this, &MainWindow::pitchTrack);
    connect(deleteBtn,  &QPushButton::clicked, this, &MainWindow::deleteTrack);
    connect(mergeBtn,   &QPushButton::clicked, this, &MainWindow::mergeTracks);
}

void MainWindow::refreshTrackList()
{
    trackList->clear();
    for (std::size_t i = 0; i < manager.size(); ++i) {
        QString name = QString::fromStdString(manager.trackName(i));
        double start = manager.startTime(i);
        double duration = manager.duration(i);

        QString label = QString("%1 (start = %2s, duration = %3s)")
                            .arg(name)
                            .arg(start, 0, 'f', 2)
                            .arg(duration, 0, 'f', 2);
        trackList->addItem(label);
    }
}

// File operations

void MainWindow::openFile()
{
    QStringList paths = QFileDialog::getOpenFileNames(
        this,
        "Open audio files",
        QString(),
        "Audio Files (*.wav *.aiff *.mp3);;All Files (*)"
    );
    if (paths.isEmpty())
        return;

    for (const QString &path : paths) {
        try {
            manager.addTrack(path.toStdString());
        } catch (const std::exception &e) {
            // a warning per failed file
            QMessageBox::warning(
                this,
                "Error",
                QString("Failed to open file:\n%1\nReason:\n%2")
                    .arg(path)
                    .arg(e.what())
            );
        }
    }

    refreshTrackList();
}


void MainWindow::saveSelectedTrack()
{
    // Require exactly one selected item
    if (trackList->selectedItems().size() != 1) {
        QMessageBox::information(
            this,
            "Selection error",
            "Save one file at a time!"
        );
        return;
    }
    int idx;
    if (!getSingleSelection(trackList, idx))
        return;
    if (idx < 0 || static_cast<std::size_t>(idx) >= manager.size())
        return;

    QString path = QFileDialog::getSaveFileName(
        this,
        "Save selected track as WAV",
        QString(),
        "WAV files (*.wav);;All Files (*)"
    );
    if (path.isEmpty())
        return;

    try {
        manager.track(static_cast<std::size_t>(idx)).saveToWav(path.toStdString());
    } catch (const std::exception &e) {
        QMessageBox::warning(
            this,
            "Error",
            QString("Failed to save track:\n%1").arg(e.what())
        );
    }
}

void MainWindow::saveMix()
{
    if (manager.size() == 0)
        return;

    QString path = QFileDialog::getSaveFileName(
        this,
        "Save mix (all tracks) as WAV",
        QString(),
        "WAV files (*.wav);;All Files (*)"
    );
    if (path.isEmpty())
        return;

    try {
        //combineAll returns std::unique_ptr<AudioTrack>
        std::unique_ptr<AudioTrack> mix = manager.combineAll();
        if (!mix) {
            QMessageBox::warning(this, "Error", "combineAll() returned null.");
            return;
        }
        mix->saveToWav(path.toStdString());
    } catch (const std::exception &e) {
        QMessageBox::warning(
            this,
            "Error",
            QString("Failed to save mix:\n%1").arg(e.what())
        );
    }
}

void MainWindow::saveMixRange()
{
    if (manager.size() == 0)
        return;

    bool okStart = false, okEnd = false;
    double start = QInputDialog::getDouble(
        this,
        "Start time",
        "Start (seconds):",
        0.0,
        0.0,
        1e9,
        3,
        &okStart
    );
    if (!okStart)
        return;

    double end = QInputDialog::getDouble(
        this,
        "End time",
        "End (seconds):",
        start,
        0.0,
        1e9,
        3,
        &okEnd
    );
    if (!okEnd || end <= start)
        return;

    QString path = QFileDialog::getSaveFileName(
        this,
        "Save time range (all tracks) as WAV",
        QString(),
        "WAV files (*.wav);;All Files (*)"
    );
    if (path.isEmpty())
        return;

    try {
        std::unique_ptr<AudioTrack> mix(manager.combineTimeRange(start, end));
        if (!mix) {
            QMessageBox::warning(this, "Error", "combineTimeRange() returned null.");
            return;
        }

        mix->saveToWav(path.toStdString());
    } catch (const std::exception &e) {
        QMessageBox::warning(
            this,
            "Error",
            QString("Failed to save time-range mix:\n%1").arg(e.what())
        );
    }
}


// diting operations

void MainWindow::reverseTrack()
{
    int idx;
    if (!getSingleSelection(trackList, idx)) return;
    if (idx < 0 || static_cast<std::size_t>(idx) >= manager.size()) return;

    manager.track(static_cast<std::size_t>(idx)).reverse();
    refreshTrackList();
}

void MainWindow::speedTrack()
{
    int idx;
    if (!getSingleSelection(trackList, idx)) return;
    if (idx < 0 || static_cast<std::size_t>(idx) >= manager.size()) return;

    bool ok = false;
    double ratio = QInputDialog::getDouble(
        this,
        "Change speed",
        "Speed ratio (e.g. 0.5 = half, 2.0 = double):",
        1.0,      // default
        0.1,      // min
        8.0,      // max
        2,        // decimals
        &ok
    );
    if (!ok) return;

    manager.track(static_cast<std::size_t>(idx)).adjustSpeed(ratio);
    refreshTrackList();
}

void MainWindow::speedResampleTrack()
{
    int idx;
    if (!getSingleSelection(trackList, idx)) return;
    if (idx < 0 || static_cast<std::size_t>(idx) >= manager.size()) return;

    bool ok = false;
    double ratio = QInputDialog::getDouble(
        this,
        "Change speed and resample",
        "Speed ratio (e.g. 0.5 = half, 2.0 = double):",
        1.0,      // default
        0.1,      // min
        8.0,      // max
        2,        // decimals
        &ok
    );
    if (!ok) return;

    manager.track(static_cast<std::size_t>(idx)).adjustSpeed_resample(ratio);
    refreshTrackList();
}

void MainWindow::speedNoDistort()
{
    int idx;
    if (!getSingleSelection(trackList, idx)) return;
    if (idx < 0 || static_cast<std::size_t>(idx) >= manager.size()) return;

    bool ok = false;
    double ratio = QInputDialog::getDouble(
        this,
        "Change speed and resample",
        "Speed ratio (e.g. 0.5 = half, 2.0 = double):",
        1.0,      // default
        0.1,      // min
        8.0,      // max
        2,        // decimals
        &ok
    );
    if (!ok) return;

    manager.track(static_cast<std::size_t>(idx)).adjustSpeed_nodistort(ratio);
    refreshTrackList();
}

void MainWindow::pitchTrack()
{
    int idx;
    if (!getSingleSelection(trackList, idx)) return;
    if (idx < 0 || static_cast<std::size_t>(idx) >= manager.size()) return;

    bool ok = false;
    double semitones = QInputDialog::getDouble(
        this,
        "Change pitch",
        "Semitones (+12 = one octave up, -12 = down):",
        0.0,     // default
        -24.0,   // min
        24.0,    // max
        1,       // decimals
        &ok
    );
    if (!ok) return;

    manager.track(static_cast<std::size_t>(idx)).repitch(semitones);
    refreshTrackList();
}

void MainWindow::deleteTrack()
{
    int idx;
    if (!getSingleSelection(trackList, idx)) return;
    if (idx < 0 || static_cast<std::size_t>(idx) >= manager.size()) return;

    manager.deleteTrack(static_cast<std::size_t>(idx));
    refreshTrackList();
}

void MainWindow::mergeTracks()
{
    auto selected = trackList->selectedItems();
    if (selected.size() != 2) return;

    int idx1 = trackList->row(selected[0]);
    int idx2 = trackList->row(selected[1]);
    if (idx1 == idx2) return;

    if (idx1 > idx2)
        std::swap(idx1, idx2);

    if (idx1 < 0 || idx2 < 0) return;
    if (static_cast<std::size_t>(idx2) >= manager.size()) return;

    manager.mergeTrack(static_cast<std::size_t>(idx1),
                       static_cast<std::size_t>(idx2));
    refreshTrackList();
}
