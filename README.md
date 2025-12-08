The following dependencies are required:

    Qt6
    FFTW
    libsndfile

For Windows:

    Qt6: Follow instructions here: https://doc.qt.io/qt-6/get-and-install-qt.html 
    FFTW: https://www.fftw.org/install/windows.html
    libsndfile: https://github.com/libsndfile/libsndfile/releases/ 
    
For macOS:

    Qt6: Follow instructions for Qt6 - https://doc.qt.io/qt-6/get-and-install-qt.html 
    FFTW:  brew install fftw
    libsndfile: brew install libsndfile
    
For Linux (Debian/Ubuntu)

    Qt6: sudo apt install qt6-base-dev build-essential
    FFTW:  sudo apt-get install libfftw3-dev
    libsndfile: sudo apt install libsndfile1-dev


To build from source:

    git clone https://github.com/basicallyAlexOh/CppFinalProject
    cd CppFinalProject
    mkdir build
    cd build
    cmake ..
    cmake --build .
    ./src/MyQtApp
