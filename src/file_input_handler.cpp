#include "file_input_handler.h"
#include "transformations/transformations.h"
#include <QDebug>
#include <QSound>
#include "test_dialog.h"
#include <math.h>
#define AUDIBLE_RANGE_START 0
#define AUDIBLE_RANGE_END   20000 /* very optimistic */

FileInputHandler::FileInputHandler()
{

}

FileInputHandler::~FileInputHandler()
{

}

void FileInputHandler::setAmpPlot(QCustomPlot* ampPlot, QFont legendFont )
{
    m_ampPlot = ampPlot;
    m_ampPlot->setInteractions( QCP::Interaction::iRangeDrag | QCP::Interaction::iRangeZoom );
    m_ampPlot->legend->setVisible( true );
    legendFont.setPointSize( 10 );
    m_ampPlot->legend->setFont( legendFont );
    m_ampPlot->legend->setSelectedFont( legendFont );
    m_ampPlot->yAxis->setLabel( "Amplitude" );
    m_ampPlot->xAxis->setLabel( "Time" );
    m_ampPlot->yAxis->setRange( -1.0, 1.0 );
    m_ampPlot->clearGraphs();
    m_ampPlot->addGraph();
    m_ampPlot->graph()->setPen( QPen( Qt::black ) );
    m_ampPlot->graph()->setName( "File Input" );
}

void FileInputHandler::setFreqPlot( QCustomPlot* freqPlot )
{
    m_freqPlot = freqPlot;
    m_freqPlot->setInteractions( QCP::iRangeDrag | QCP::iRangeZoom );
    m_freqPlot->legend->setVisible( false );
    m_freqPlot->yAxis->setLabel( "" );
    m_freqPlot->xAxis->setLabel( "Frequency" );
    m_freqPlot->xAxis->setRange( AUDIBLE_RANGE_START, AUDIBLE_RANGE_END );
    m_freqPlot->yAxis->setRange( 0.0, 500.0 );
    m_freqPlot->clearGraphs();
    m_freqPlot->addGraph();
    m_freqPlot->graph()->setPen( QPen( Qt::black ));
    m_freqPlot->graph()->setName( "fft" );
}

void FileInputHandler::setTransformationType(DFT::Type type)
{
    m_type = type;
}

void FileInputHandler::readFileData( const QString& fileName )
{
    m_fileName = fileName;
    AudioFile<double> a;
    a.load( fileName.toStdString() );
    m_freq = a.getSampleRate();
    m_duration = a.getLengthInSeconds();
    qDebug() << "Duration " << m_duration;
    qDebug() << "Comp duration " << m_sampleNum / m_freq;
    m_sampleNum = a.getNumSamplesPerChannel();
    qDebug() << "Size " << a.samples[0].size();
    qDebug() << "Size num" << m_sampleNum;
    qDebug() << "Comp size " << m_duration* m_freq;
    m_samples.clear();
    m_samples = QVector<double>( a.samples[0].begin(), a.samples[0].end() );
    double freqStep = static_cast<double>( m_freq ) / static_cast<double>( m_sampleNum );
    countFreqIndex( freqStep );
    m_indices.clear();
    double i = 0.0;
    while( i < m_duration )
    {
        m_indices.append( i );
        i += 1.0 / m_freq;
    }
    m_fileLoaded = true;
}

void FileInputHandler::compute()
{
    if( m_fileLoaded )
    {
        updateAmpPlot();
        updateFreqPlot();
    }
}

void FileInputHandler::play()
{
    if( m_fileLoaded )
        QSound::play( m_fileName );
}

void FileInputHandler::openTestWindow()
{
    if( m_fileLoaded )
    {
        TestDialog diag( m_fileName );
        diag.exec();
    }
}

void FileInputHandler::updateAmpPlot()
{
    if( m_ampPlot )
    {
        qDebug() << "Ind " << m_indices.size();
        qDebug() << "Sampl " << m_samples.size();
        m_ampPlot->graph(0)->setData( m_indices, m_samples );
        m_ampPlot->xAxis->rescale();
        m_ampPlot->replot();
    }
}

void FileInputHandler::updateFreqPlot()
{
    if( m_freqPlot )
    {
        QVector<double> out;
        executeTransformation( out );
        m_freqPlot->yAxis->setRange( 0.0, 500.0 );
        m_freqPlot->graph( 0 )->setData( m_fftIndices.mid( 0, out.length() ), out );
        m_freqPlot->replot();
    }
}

void FileInputHandler::countFreqIndex( double freqStep )
{
    m_fftIndices.clear();
    double f = AUDIBLE_RANGE_START;
    while( f < AUDIBLE_RANGE_END )
    {
        m_fftIndices.append( f );
        f += freqStep;
    }
}

void FileInputHandler::executeTransformation( QVector<double>& outVec )
{
    QVector<double> temp;
    DFT::executeDFT( m_type, m_samples, temp );
    double index = m_duration;
    int from = AUDIBLE_RANGE_START*index, count = AUDIBLE_RANGE_END*index - AUDIBLE_RANGE_START* index;
    outVec = temp.mid( from, count );
}
