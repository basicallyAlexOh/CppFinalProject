#include "ui/MainWindow.h"

#include <QApplication>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListWidget>
#include <QPushButton>
#include <QFileDialog>
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
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


void MainWindow::createMenus() {
    fileMenu = menuBar()->addMenu("&File");

    openAction = new QAction("Open…", this);
    quitAction = new QAction("Quit", this);

    fileMenu->addAction(openAction);
    fileMenu->addSeparator();
    fileMenu->addAction(quitAction);

    connect(openAction, &QAction::triggered,
            this, &MainWindow::openFile);
    connect(quitAction, &QAction::triggered,
            qApp, &QApplication::quit);
}

void MainWindow::createCentralWidget() {
    trackList = new QListWidget(this);

    reverseBtn = new QPushButton("Reverse", this);
    speedBtn   = new QPushButton("Speed…", this);
    pitchBtn   = new QPushButton("Pitch…", this);
    deleteBtn  = new QPushButton("Delete", this);
    mergeBtn   = new QPushButton("Merge 2", this);

    auto *buttonLayout = new QVBoxLayout;
    buttonLayout->addWidget(reverseBtn);
    buttonLayout->addWidget(speedBtn);
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

    // Connect buttons
    connect(reverseBtn, &QPushButton::clicked, this, &MainWindow::reverseTrack);
    connect(speedBtn,   &QPushButton::clicked, this, &MainWindow::speedTrack);
    connect(pitchBtn,   &QPushButton::clicked, this, &MainWindow::pitchTrack);
    connect(deleteBtn,  &QPushButton::clicked, this, &MainWindow::deleteTrack);
    connect(mergeBtn,   &QPushButton::clicked, this, &MainWindow::mergeTracks);
}

void MainWindow::refreshTrackList() {
    trackList->clear();
    for (size_t i = 0; i < manager.size(); ++i) {
        // For now, just display “Track N”
        // (You can store filenames in TrackManager later.)
        trackList->addItem(QString("Track %1").arg(i));
    }
}

void MainWindow::openFile() {
    QString path = QFileDialog::getOpenFileName(
        this,
        "Open audio file",
        QString(),
        "Audio Files (*.wav *.aiff *.mp3);;All Files (*)"
    );
    if (path.isEmpty())
        return;

    try {
        manager.addTrack(path.toStdString());
        refreshTrackList();
    } catch (const std::exception &e) {
        // You can show a QMessageBox here
        qWarning("Failed to open file: %s", e.what());
    }
}

static bool getSingleSelection(QListWidget* list, int& index) {
    index = list->currentRow();
    return index >= 0;
}

void MainWindow::reverseTrack() {
    int idx;
    if (!getSingleSelection(trackList, idx)) return;
    if (idx >= static_cast<int>(manager.size())) return;

    manager.track(idx).reverse();
}

void MainWindow::speedTrack() {
    int idx;
    if (!getSingleSelection(trackList, idx)) return;
    if (idx >= static_cast<int>(manager.size())) return;

    bool ok = false;
    double ratio = QInputDialog::getDouble(
        this,
        "Change speed",
        "Speed ratio (e.g. 0.5 = half, 2.0 = double):",
        1.0, 0.1, 8.0, 2, &ok
    );
    if (!ok) return;

    manager.track(idx).adjustSpeed(ratio);
}

void MainWindow::pitchTrack() {
    int idx;
    if (!getSingleSelection(trackList, idx)) return;
    if (idx >= static_cast<int>(manager.size())) return;

    bool ok = false;
    double semitones = QInputDialog::getDouble(
        this,
        "Change pitch",
        "Semitones (+12 = one octave up, -12 = down):",
        0.0, -24.0, 24.0, 1, &ok
    );
    if (!ok) return;

    manager.track(idx).repitch(semitones);
}

void MainWindow::deleteTrack() {
    int idx;
    if (!getSingleSelection(trackList, idx)) return;
    if (idx >= static_cast<int>(manager.size())) return;

    manager.deleteTrack(static_cast<size_t>(idx));
    refreshTrackList();
}

void MainWindow::mergeTracks() {
    auto selected = trackList->selectedItems();
    if (selected.size() != 2) return;

    int idx1 = trackList->row(selected[0]);
    int idx2 = trackList->row(selected[1]);
    if (idx1 == idx2) return;

    if (idx1 > idx2) std::swap(idx1, idx2);
    manager.mergeTrack(static_cast<size_t>(idx1),
                       static_cast<size_t>(idx2));
    refreshTrackList();
}
