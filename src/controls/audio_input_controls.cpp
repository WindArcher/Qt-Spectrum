#include "audio_input_controls.h"
#include <QPushButton>
#include <QSlider>
#include <QComboBox>

AudioInputControls::AudioInputControls()
{
    QPushButton* useButton = new QPushButton( "Use" );
    QPushButton* refreshButton = new QPushButton( "Refresh" );
    QSlider* slider = new QSlider( Qt::Orientation::Horizontal );
    slider->setValue( 100 );
    m_audioInputBox = new QComboBox();
    QList<QAudioDeviceInfo> inputDevices = QAudioDeviceInfo::availableDevices(QAudio::AudioInput);
    for( const QAudioDeviceInfo& d : inputDevices )
        m_audioInputBox->addItem( d.deviceName(), QVariant::fromValue(d) );
    connect( useButton, &QPushButton::pressed, this, &AudioInputControls::useButtonPressedSlot );
    connect( refreshButton, &QPushButton::pressed, this, &AudioInputControls::refreshButtonPressedSlot );
    connect( slider, &QSlider::sliderMoved, this, &AudioInputControls::volumeChangedSlot );
    m_layout = new QVBoxLayout( this );
    m_layout->addWidget( m_audioInputBox );
    m_layout->addWidget( slider );
    m_layout->addWidget( useButton );
    m_layout->addWidget( refreshButton );
    this->setLayout( m_layout );
}

void AudioInputControls::useButtonPressedSlot()
{
    QVariant v = m_audioInputBox->currentData();
    QAudioDeviceInfo dev = v.value<QAudioDeviceInfo>();
    emit useButtonPressed( dev );
}

void AudioInputControls::refreshButtonPressedSlot()
{
    QList<QAudioDeviceInfo> inputDevices = QAudioDeviceInfo::availableDevices(QAudio::AudioInput);
    m_audioInputBox->clear();
    for( QAudioDeviceInfo d : inputDevices)
        m_audioInputBox->addItem( d.deviceName(), QVariant::fromValue(d) );
}

void AudioInputControls::volumeChangedSlot(int position)
{
    emit volumeChanged( position );
}
