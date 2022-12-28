QT       += core gui printsupport multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
CONFIG(release, debug|release) {
  DESTDIR = $$PWD/bin/release
}
CONFIG(debug, debug|release) {
  DESTDIR = $$PWD/bin/debug
}


INCLUDEPATH += $$PWD/include

DEPENDPATH += $$PWD/include

LIBS +=  $$PWD/lib/qcustomplot/libqcustomplot2.a \
         $$PWD/lib/fftw/libfftw3-3.lib \
         $$PWD/lib/ffmpeg/avcodec.lib \
         $$PWD/lib/ffmpeg/avdevice.lib \
         $$PWD/lib/ffmpeg/avfilter.lib \
         $$PWD/lib/ffmpeg/avformat.lib \
         $$PWD/lib/ffmpeg/avutil.lib \
         $$PWD/lib/ffmpeg/postproc.lib \
         $$PWD/lib/ffmpeg/swresample.lib \
         $$PWD/lib/ffmpeg/swscale.lib

SOURCES += \
    src/SpecrtogramWidget/spek-audio.cpp \
    src/SpecrtogramWidget/spek-fft.cpp \
    src/SpecrtogramWidget/spek-palette.cpp \
    src/SpecrtogramWidget/spek-pipeline.cpp \
    src/SpecrtogramWidget/spek-ruler.cpp \
    src/SpecrtogramWidget/spek-spectrogram.cpp \
    src/SpecrtogramWidget/spek-utils.cpp \
    src/dfttest.cpp \
    src/file_input_handler.cpp \
    src/controls/audio_input_controls.cpp \
    src/audio_input_handler.cpp \
    src/controls/file_controls.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/test_dialog.cpp \
    src/transformations/transformations.cpp

HEADERS += \
    src/SpecrtogramWidget/spek-audio.h \
    src/SpecrtogramWidget/spek-fft.h \
    src/SpecrtogramWidget/spek-palette.h \
    src/SpecrtogramWidget/spek-pipeline.h \
    src/SpecrtogramWidget/spek-ruler.h \
    src/SpecrtogramWidget/spek-spectrogram.h \
    src/SpecrtogramWidget/spek-utils.h \
    src/dfttest.h \
    src/file_input_handler.h \
    src/controls/audio_input_controls.h \
    src/audio_input_handler.h \
    src/controls/file_controls.h \
    src/mainwindow.h \
    src/test_dialog.h \
    src/transformations/transformations.h \

FORMS += \
    src/mainwindow.ui \
    src/test_dialog.ui
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
