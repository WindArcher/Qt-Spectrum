#pragma once

#include <QObject>
#include <QFont>
#include <QVector>
#include <QString>
#include <fftw/fftw3.h>
#include "qcustomplot/qcustomplot.h"
#include "transformations/transformations.h"
#include "AudioFile/AudioFile.h"

class FileInputHandler : public QObject
{
    Q_OBJECT
public:
    FileInputHandler();
    ~FileInputHandler();
    void setAmpPlot( QCustomPlot* ampPlot, QFont legendFont );
    void setFreqPlot( QCustomPlot* freqPlot );
    void setTransformationType( DFT::Type type );
public slots:
    void readFileData( const QString& fileName );
    void compute();
    void play();
    void openTestWindow();
private:
     bool m_fileLoaded = false;
     QString m_fileName;
     QVector<double> m_samples;
     QVector<double> m_indices;
     QVector<double> m_fftIndices;
     float m_freq;
     int m_sampleNum;
     double m_duration;
     DFT::Type m_type = DFT::Type::FFT;
     QCustomPlot* m_ampPlot = nullptr;
     QCustomPlot* m_freqPlot = nullptr;

     void updateAmpPlot();
     void updateFreqPlot();
     void executeTransformation( QVector<double>& outVec );
     void countFreqIndex( double freqStep );
};
