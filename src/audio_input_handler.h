#pragma once

#include <QObject>
#include <QAudioInput>
#include <QAudioDeviceInfo>
#include <QBuffer>
#include "fftw/fftw3.h"
#include "qcustomplot/qcustomplot.h"
#include "transformations/transformations.h"

class AudioInputHandler : public QObject
{
    Q_OBJECT
public:
    explicit AudioInputHandler( QObject *parent = nullptr );
    explicit AudioInputHandler( const QAudioDeviceInfo& dev, QObject *parent = nullptr );
    ~AudioInputHandler();
    void setNewDevice( const QAudioDeviceInfo& dev );
    void setAmpPlot( QCustomPlot* ampPlot, QFont font );
    void setFreqPlot( QCustomPlot* freqPlot );
    void start();
    void stop();
    void setTransformationType( DFT::Type type );
signals:
    void dataReady();
public slots:
    void volumeChanged( int pos );
private slots:
    void emitDataReady();
    void stateChangeAudioIn( QAudio::State s );
private:
    QAudioInput* m_audioInput = nullptr;
    QBuffer m_buffer;
    QCustomPlot* m_ampPlot = nullptr;
    QCustomPlot* m_freqPlot = nullptr;
    QVector<double> m_samples;
    QVector<double> m_indices;
    QVector<double> m_fftIndices;
    DFT::Type m_type = DFT::Type::FFT;

    fftw_plan mFftPlan;
    double *mFftIn;
    double *mFftOut;

    void updateAmpPlot();
    void updateFreqPlot();
    QVector<double> executeTransformation();
};
