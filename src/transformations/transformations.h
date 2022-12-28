#pragma once
#include <QPair>
#include <QVector>

namespace DFT
{
    enum class Type
    {
        FHT,
        FFT,
        FFThc
    };
    QPair<double,int> executeDFT( Type type, const QVector<double>& inVec, QVector<double>& outVec, bool log = false );
};
