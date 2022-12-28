#include "audio_input_handler.h"
#include <QDebug>
#define AUDIBLE_RANGE_START 20
#define AUDIBLE_RANGE_END   20000 /* very optimistic */
#define NUM_SAMPLES 96000
#define SAMPLE_FREQ 48000

AudioInputHandler::AudioInputHandler( QObject*parent ) : QObject( parent )
{
    m_buffer.open(QBuffer::ReadWrite);
}

AudioInputHandler::AudioInputHandler( const QAudioDeviceInfo& dev, QObject *parent ) : QObject( parent )
{
    QAudioFormat format;
    format.setSampleSize(16);
    format.setSampleRate(48000);
    format.setCodec(QString("audio/pcm"));
    format.setSampleType(QAudioFormat::SignedInt);
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setChannelCount(1);
    m_audioInput = new QAudioInput( dev, format );
    m_audioInput->setVolume(1);
    m_audioInput->setNotifyInterval(100);
    m_buffer.open( QBuffer::ReadWrite );

    mFftIn  = fftw_alloc_real(NUM_SAMPLES);
    mFftOut = fftw_alloc_real(NUM_SAMPLES);
    mFftPlan = fftw_plan_r2r_1d(NUM_SAMPLES, mFftIn, mFftOut, FFTW_R2HC,FFTW_ESTIMATE);
}

AudioInputHandler::~AudioInputHandler()
{
    fftw_free(mFftIn);
    fftw_free(mFftOut);
    fftw_destroy_plan(mFftPlan);
}

void AudioInputHandler::setNewDevice( const QAudioDeviceInfo& newDevice )
{
    QAudioFormat format;
    format.setSampleSize(16);
    format.setSampleRate(48000);
    format.setCodec( QString( "audio/pcm" ) );
    format.setSampleType(QAudioFormat::SignedInt);
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setChannelCount(1);
    if( m_audioInput )
        delete m_audioInput;
    m_audioInput = new QAudioInput( newDevice, format );
    m_audioInput->setVolume( 0.1 );
    m_audioInput->setNotifyInterval(100);
    connect( m_audioInput, &QAudioInput::notify, this, &AudioInputHandler::emitDataReady );
    connect( m_audioInput, &QAudioInput::stateChanged, this, &AudioInputHandler::stateChangeAudioIn );
}

void AudioInputHandler::setAmpPlot( QCustomPlot* ampPlot, QFont font )
{
    m_ampPlot = ampPlot;
    m_ampPlot->setInteractions( QCP::Interaction::iRangeDrag | QCP::Interaction::iRangeZoom );
    m_ampPlot->legend->setVisible( true );
    font.setPointSize( 10 );
    m_ampPlot->legend->setFont( font );
    m_ampPlot->legend->setSelectedFont( font );
    m_ampPlot->yAxis->setLabel( "Amplitude" );
    m_ampPlot->xAxis->setLabel( "Sample" );
    m_ampPlot->yAxis->setRange( -1.0, 1.0 );
    m_ampPlot->clearGraphs();
    m_ampPlot->addGraph();
    m_ampPlot->graph()->setPen( QPen( Qt::black ) );
    m_ampPlot->graph()->setName( "Audio In" );
    for( int i = 0; i < 96000; i++ )
    {
        m_indices.append( static_cast<double>( i ) );
        m_samples.append(0);
    }
}

void AudioInputHandler::setFreqPlot( QCustomPlot* freqPlot )
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
    double freqStep = (double)SAMPLE_FREQ / (double)NUM_SAMPLES;
    double f = AUDIBLE_RANGE_START;
    while ( f < AUDIBLE_RANGE_END )
    {
        m_fftIndices.append( f );
        f += freqStep;
    }
}

void AudioInputHandler::start()
{
    if( m_audioInput )
        m_audioInput->start( &m_buffer );
}

void AudioInputHandler::stop()
{
    if( m_audioInput )
        m_audioInput->stop();
}

void AudioInputHandler::setTransformationType( DFT::Type type )
{
    m_type = type;
}

void AudioInputHandler::volumeChanged( int pos )
{
    if( m_audioInput )
        m_audioInput->setVolume( double(pos) / 1000.0 );
}

void AudioInputHandler::emitDataReady()
{
    m_buffer.seek(0);
    QByteArray ba = m_buffer.readAll();

    int num_samples = ba.length() / 2;
    int b_pos = 0;
    for (int i = 0; i < num_samples; i ++)
    {
        int16_t s = 0;
        s = ( s&0xFF) | (ba.at(b_pos++) << 8);
        if (s != 0)
            m_samples.append( (double)s / 32768.0 );
        else
            m_samples.append(0);
    }
    m_buffer.buffer().clear();
    m_buffer.seek(0);
    updateAmpPlot();
    updateFreqPlot();
}

void AudioInputHandler::stateChangeAudioIn( QAudio::State s )
{
    qDebug() << "State change: " << s;
}

void AudioInputHandler::updateAmpPlot()
{
    if( m_ampPlot )
    {
        int n = m_samples.length();
        if (n > 96000)
            m_samples = m_samples.mid( n - 96000, -1 );
        m_ampPlot->graph(0)->setData( m_indices, m_samples );
        m_ampPlot->xAxis->rescale();
        m_ampPlot->replot();
    }
}

void AudioInputHandler::updateFreqPlot()
{
    if( m_freqPlot )
    {
        QVector<double> out( executeTransformation() );
        m_freqPlot->graph( 0 )->setData( m_fftIndices.mid( 0, out.length() ), out );
        m_ampPlot->xAxis->rescale();
        m_freqPlot->replot();
    }
}


QVector<double> AudioInputHandler::executeTransformation()
{
    QVector<double> temp;
    DFT::executeDFT( m_type, m_samples, temp );
    int index = (NUM_SAMPLES/SAMPLE_FREQ);
    int from = AUDIBLE_RANGE_START*index, count = AUDIBLE_RANGE_END*index - AUDIBLE_RANGE_START* index;
    return temp.mid( from, count );
}
