#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QString>
#include "qcustomplot/qcustomplot.h"
class FileControls : public QWidget
{
    Q_OBJECT
public:
    FileControls();
signals:
    void foundPathToFile( const QString& path );
    void computeFile();
    void createSpectrogram( const QString& path );
    void play();
    void runTests();
private slots:
    void findPathToFile();
    void createSpectrogeramSlot();
private:
    QVBoxLayout* m_layout;
    QString m_fileName = "";
};

