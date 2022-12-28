#pragma once

#include <QMainWindow>
#include <QAction>
#include "qcustomplot/qcustomplot.h"
#include "controls/audio_input_controls.h"
#include "controls/file_controls.h"
#include "audio_input_handler.h"
#include "file_input_handler.h"
#include "SpecrtogramWidget/spek-spectrogram.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void onUseButtonPressed( const QAudioDeviceInfo& dev );
    void initControls( bool checked );
    void createSpecrogram( const QString& filename );
    void setFTT();
    void setFFThc();
    void setFHT();
    void openHelpWindow();
private:
    Ui::MainWindow *ui;

    Spectrogram::SpekSpectrogram* m_specWidget = nullptr;
    AudioInputHandler m_audioInput;
    AudioInputControls m_audioInputContr;
    FileControls m_fileControls;
    FileInputHandler m_fileHandler;
    QAction* m_naiveDft;
    QAction* m_FFT;
    QAction* m_FFThc;
    QAction* m_FHT;
    QAction* m_openEfficiencyWindow;
    QAction* m_openHelpWindow;
};
