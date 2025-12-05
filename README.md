The following dependencies are required:
    FFTW
    libsndfile

For Windows:
    FFTW: https://www.fftw.org/install/windows.html
    Libsndfile: https://github.com/libsndfile/libsndfile/releases/ 
For macOS:
    FFTW:  brew install fftw
    libsndfile: brew install libsndfile
For Linux (Debian/Ubuntu)
    FFTW:  sudo apt-get install libfftw3-dev
    libsndfile: sudo apt install libsndfile1-dev


To build from source:
    git clone https://github.com/basicallyAlexOh/CppFinalProject
    cd CppFinalProject
    mkdir build
    cd build
    cmake .
    cmake --build .
    ./src/MyQtApp
