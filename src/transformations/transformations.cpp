#include "transformations.h"
#include "fftw/fftw3.h"
#include <QDebug>
#include <cmath>
#include <chrono>

namespace DFT
{
    #define PI 3.14159

    void hamming( QVector<double> &signal )
    {
        int N = signal.size();
        for (int i = 0; i < N; i++)
            signal[i] *= ( 0.54 - 0.46 * cos( 2*PI * i/N) );
    }

    void hamming( double* signal, const int N )
    {
        for (int i = 0; i < N; i++)
            signal[i] *= ( 0.54 - 0.46 * cos( 2*PI * i/N) );
    }

    void countSpectrum( double* input, QVector<double>& spec, int size, bool log )
    {
        if( log )
        {
            spec.append( 10 * std::log( input[0]*input[0] ) );
        }
        else
            spec.append( abs( input[0] ) );
        for( int i = 1; i < ( size +1 ) / 2; ++i )
        {
            double engValue = input[i]*input[i] + input[size - i] * input[size - i];
            if( log )
            {
                spec.append( 10 * std::log( input[i] * input[i] ) );
            }
            else
                spec.append( sqrt( engValue ) );
        }
        if( size % 2 == 0 )
        {
            if( log )
            {
               spec.append( 10 * std::log( input[size/2] * input[size/2] ) );
            }
             else
                spec.append( abs( input[size/2] ) );
        }
    }

    void countSpectrum( fftw_complex* input, QVector<double>& spec, int size, bool log )
    {
        for( int i = 1; i < ( size +1 ) / 2; ++i )
        {
            double enrValue = input[i][0]*input[i][0] + input[i][1] * input[i][1];
            if( log )
                spec.append( 10 * std::log( enrValue ) );
            else
                spec.append( sqrt( enrValue ) );
        }
    }

    QPair<double, int> FFTc( QVector<double> inVec, QVector<double>& outVec, bool log = false )
    {
        int nfft = inVec.size();
        QPair<double,int> returnValue;
        returnValue.second = 2*nfft;
        double* fftIn  = fftw_alloc_real( nfft );
        fftw_complex* fftOut = fftw_alloc_complex( nfft );
        fftw_plan fftPlan = fftw_plan_dft_r2c_1d( nfft, fftIn, fftOut, FFTW_ESTIMATE );
        auto start = std::chrono::steady_clock::now();
        memcpy( fftIn, inVec.data(), nfft*sizeof(double) );
        hamming( fftIn, nfft );
        fftw_execute( fftPlan );
        outVec.clear();
        countSpectrum( fftOut, outVec, nfft, log );
        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsed_seconds = end-start;
        returnValue.first = elapsed_seconds.count();
        fftw_free(fftIn);
        fftw_free(fftOut);
        fftw_destroy_plan(fftPlan);
        return returnValue;
    }

    QPair<double,int> FHT_func( QVector<double> inVec, QVector<double>& outVec, bool log = false )
    {
        int nfft = inVec.size();
        QPair<double,int> returnValue;
        returnValue.second = nfft;
        double* fftIn  = fftw_alloc_real( nfft );
        double* fftOut = fftw_alloc_real( nfft );
        fftw_plan fftPlan = fftw_plan_r2r_1d( nfft, fftIn, fftOut, FFTW_DHT, FFTW_ESTIMATE );
        auto start = std::chrono::steady_clock::now();
        memcpy( fftIn, inVec.data(), nfft*sizeof(double) );
        hamming( fftIn, nfft );
        fftw_execute( fftPlan );
        outVec.clear();
        countSpectrum( fftOut, outVec, nfft, log );
        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsed_seconds = end-start;
        returnValue.first = elapsed_seconds.count();
        fftw_free(fftIn);
        fftw_free(fftOut);
        fftw_destroy_plan(fftPlan);
        return returnValue;
    }

    QPair<double,int> FFThc( const QVector<double>& inVec, QVector<double>& outVec, bool log = false )
    {
        int nfft = inVec.size();
        QPair<double,int> returnValue;
        returnValue.second = nfft;
        double* fftIn  = fftw_alloc_real( nfft );
        double* fftOut = fftw_alloc_real( nfft );
        fftw_plan fftPlan = fftw_plan_r2r_1d( nfft, fftIn, fftOut, FFTW_R2HC, FFTW_ESTIMATE );
        auto start = std::chrono::steady_clock::now();
        memcpy( fftIn, inVec.data(), nfft*sizeof(double) );
        hamming( fftIn, nfft );
        fftw_execute( fftPlan );
        outVec.clear();
        countSpectrum( fftOut, outVec, nfft, log );
        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsed_seconds = end-start;
        returnValue.first = elapsed_seconds.count();
        qDebug() << "Duration" << elapsed_seconds.count();
        fftw_free( fftIn );
        fftw_free( fftOut );
        fftw_destroy_plan(fftPlan);
        return returnValue;
    }

    QPair<double,int> executeDFT( Type type, const QVector<double>& inVec, QVector<double>& outVec, bool log )
    {
        switch( type )
        {
            case Type::FFThc:
                return FFThc( inVec, outVec, log );
                break;
            case Type::FFT:
                return FFTc( inVec, outVec, log );
                break;
            case Type::FHT:
                return FHT_func( inVec, outVec, log );
                break;
        }
    }
};


