#pragma once
#include <QVBoxLayout>
#include <QComboBox>
#include <QAudioDeviceInfo>

class AudioInputControls : public QWidget
{
    Q_OBJECT
public:
    AudioInputControls();
signals:
    void useButtonPressed( const QAudioDeviceInfo& device );
    void refreshButtonPressed();
    void volumeChanged( int position );
private slots:
    void useButtonPressedSlot();
    void refreshButtonPressedSlot();
    void volumeChangedSlot( int position );
private:
    QVBoxLayout* m_layout;
    QComboBox* m_audioInputBox;
};

