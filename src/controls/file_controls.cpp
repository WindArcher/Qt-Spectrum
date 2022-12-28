#include "file_controls.h"
#include <QPushButton>
#include <QFileDialog>
#include <QStandardPaths>

FileControls::FileControls()
{
    QPushButton* loadFile = new QPushButton( "Load File" );
    QPushButton* createSpectrogram = new QPushButton( "Create Spectrogram" );
    QPushButton* compute = new QPushButton( "Compute" );
    QPushButton* play = new QPushButton( "Play" );
    QPushButton* runTests = new QPushButton( "Run tests" );

    connect( loadFile, &QPushButton::pressed, this, &FileControls::findPathToFile );
    connect( createSpectrogram, &QPushButton::pressed, this, &FileControls::createSpectrogeramSlot );
    connect( compute, &QPushButton::pressed, this, &FileControls::computeFile );
    connect( play, &QPushButton::pressed, this, &FileControls::play );
    connect( runTests, &QPushButton::pressed, this, &FileControls::runTests );
    m_layout = new QVBoxLayout( this );
    m_layout->addWidget( loadFile );
    m_layout->addWidget( createSpectrogram );
    m_layout->addWidget( compute );
    m_layout->addWidget( play );
    m_layout->addWidget( runTests );
    this->setLayout( m_layout );
}

void FileControls::findPathToFile()
{
    m_fileName = QFileDialog::getOpenFileName( this,
                                                     tr("Open Audio"),
                                                     QStandardPaths::writableLocation( QStandardPaths::MusicLocation ),
                                                     tr("Audio Files (*.wav)"));
    emit foundPathToFile( m_fileName );
}

void FileControls::createSpectrogeramSlot()
{
    if( m_fileName != "" )
        emit createSpectrogram( m_fileName );
}
