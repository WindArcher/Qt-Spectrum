#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Sound");
    ui->gridLayout->addWidget( &m_fileControls );
    ui->gridLayout->addWidget( &m_audioInputContr );
    initControls( true );
    connect( ui->fileModeButton, &QRadioButton::toggled, this, &MainWindow::initControls );
    connect( &m_fileControls, &FileControls::foundPathToFile, &m_fileHandler, &FileInputHandler::readFileData );
    connect( &m_fileControls, &FileControls::computeFile, &m_fileHandler,&FileInputHandler::compute );
    connect( &m_fileControls, &FileControls::play, &m_fileHandler, &FileInputHandler::play );
    connect( &m_fileControls, &FileControls::runTests, &m_fileHandler, &FileInputHandler::openTestWindow );
    connect( &m_audioInputContr, &AudioInputControls::useButtonPressed, this, &MainWindow::onUseButtonPressed );
    connect( &m_audioInputContr, &AudioInputControls::volumeChanged, &m_audioInput, &AudioInputHandler::volumeChanged );
    connect( &m_fileControls, &FileControls::createSpectrogram, this, &MainWindow::createSpecrogram );
    connect( ui->actionFFT, &QAction::triggered, this, &MainWindow::setFTT );
    connect( ui->actionFFThc, &QAction::triggered, this, &MainWindow::setFFThc );
    connect( ui->actionFHT, &QAction::triggered, this, &MainWindow::setFHT );
    connect( ui->actionHelp, &QAction::triggered, this, &MainWindow::openHelpWindow );
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_specWidget;
}

void MainWindow::onUseButtonPressed( const QAudioDeviceInfo& dev )
{
    m_audioInput.setNewDevice( dev );
    m_audioInput.start();
}

void MainWindow::initControls( bool checked )
{
    if( ui->fileModeButton->isChecked() )
    {
        m_audioInputContr.hide();
        ui->actionnaiveDFT->setEnabled( true );
        m_audioInput.stop();
        m_fileHandler.setAmpPlot( ui->plot, font() );
        m_fileHandler.setFreqPlot( ui->plot2 );
        m_fileControls.show();
    }
    else
    {
        ui->actionnaiveDFT->setEnabled( false );
        m_fileControls.hide();
        m_audioInputContr.show();
        m_audioInput.setAmpPlot( ui->plot, font() );
        m_audioInput.setFreqPlot( ui->plot2 );
    }
}

void MainWindow::createSpecrogram( const QString& filename )
{
    using namespace Spectrogram;
    if( m_specWidget )
        delete m_specWidget;
    m_specWidget = new SpekSpectrogram();
    m_specWidget->show();
    m_specWidget->open( filename );
}

void MainWindow::setFTT()
{
    m_fileHandler.setTransformationType( DFT::Type::FFT );
    m_audioInput.setTransformationType( DFT::Type::FFT );
}

void MainWindow::setFFThc()
{
    m_fileHandler.setTransformationType( DFT::Type::FFThc );
    m_audioInput.setTransformationType( DFT::Type::FFThc );
}

void MainWindow::setFHT()
{
    m_fileHandler.setTransformationType( DFT::Type::FHT );
    m_audioInput.setTransformationType( DFT::Type::FHT );
}

void MainWindow::openHelpWindow()
{

}
