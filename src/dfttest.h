#pragma once

#include <QString>
#include "AudioFile/AudioFile.h"

struct TestResult
{
    int sampleSize;
    int memoryUsage;
    double timeOfTransform;
    double timeOfReverseTransfrom;
    double fullTime;
};

class DFTTest
{
public:
    DFTTest();
    void loadFile( const QString& fileName );
    TestResult runFTTtest();
    TestResult runFHTtest();
    TestResult runFFThctest();
private:
    QString m_fileName;
    AudioFile<double> m_file;
};
