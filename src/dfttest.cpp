#include "dfttest.h"
#include <QVector>
#include <chrono>
#include "fftw/fftw3.h"
DFTTest::DFTTest()
{

}

void DFTTest::loadFile(const QString&fileName)
{
    m_fileName = fileName;
    m_file.load( fileName.toStdString() );
}

TestResult DFTTest::runFTTtest()
{
    TestResult r;
    QVector<double> samples( m_file.samples[0].begin(), m_file.samples[0].end() );
    r.sampleSize = samples.size();
    r.memoryUsage = samples.size() * 2;
    int nfft = samples.size();

    auto start = std::chrono::steady_clock::now();
    fftw_complex* complexV = fftw_alloc_complex( nfft );
    fftw_plan fftPlan = fftw_plan_dft_r2c_1d( nfft, samples.data(), complexV, FFTW_ESTIMATE );
    fftw_execute( fftPlan );
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;
    r.timeOfTransform = elapsed_seconds.count();


    start = std::chrono::steady_clock::now();
    double* realV = fftw_alloc_real( nfft );
    fftw_plan rFFtPlan = fftw_plan_dft_c2r_1d( nfft, complexV, realV, FFTW_ESTIMATE );
    fftw_execute( fftPlan );
    end = std::chrono::steady_clock::now();
    elapsed_seconds = end-start;
    r.timeOfReverseTransfrom = elapsed_seconds.count();

    r.fullTime = r.timeOfReverseTransfrom + r.timeOfTransform;

    fftw_free(complexV);
    fftw_free(realV);
    fftw_destroy_plan(fftPlan);
    fftw_destroy_plan(rFFtPlan);
    return r;
}

TestResult DFTTest::runFHTtest()
{
    TestResult r;
    QVector<double> samples( m_file.samples[0].begin(), m_file.samples[0].end() );
    r.sampleSize = samples.size();
    r.memoryUsage = samples.size();
    int nfft = samples.size();

    auto start = std::chrono::steady_clock::now();
    double* outHC = fftw_alloc_real( nfft );
    fftw_plan fftPlan = fftw_plan_r2r_1d( nfft, samples.data(), outHC, FFTW_R2HC, FFTW_ESTIMATE );
    fftw_execute( fftPlan );
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;
    r.timeOfTransform = elapsed_seconds.count();


    start = std::chrono::steady_clock::now();
    double* realV = fftw_alloc_real( nfft );
    fftw_plan rfftPlan = fftw_plan_r2r_1d( nfft, outHC, realV, FFTW_HC2R, FFTW_ESTIMATE );
    fftw_execute( fftPlan );
    end = std::chrono::steady_clock::now();
    elapsed_seconds = end-start;
    r.timeOfReverseTransfrom = elapsed_seconds.count();

    r.fullTime = r.timeOfReverseTransfrom + r.timeOfTransform;

    fftw_free(outHC);
    fftw_free(realV);
    fftw_destroy_plan(fftPlan);
    fftw_destroy_plan(rfftPlan);
    return r;
}

TestResult DFTTest::runFFThctest()
{
    TestResult r;
    QVector<double> samples( m_file.samples[0].begin(), m_file.samples[0].end() );

    r.sampleSize = samples.size();
    r.memoryUsage = samples.size();
    int nfft = samples.size();

    auto start = std::chrono::steady_clock::now();
    double* outR = fftw_alloc_real( nfft );
    fftw_plan fftPlan = fftw_plan_r2r_1d( nfft, samples.data(), outR, FFTW_DHT, FFTW_ESTIMATE );
    fftw_execute( fftPlan );
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;
    r.timeOfTransform = elapsed_seconds.count();


    start = std::chrono::steady_clock::now();
    double* realV = fftw_alloc_real( nfft );
    fftw_plan rfftPlan = fftw_plan_r2r_1d( nfft, outR, realV, FFTW_DHT, FFTW_ESTIMATE );
    fftw_execute( fftPlan );
    end = std::chrono::steady_clock::now();
    elapsed_seconds = end-start;
    r.timeOfReverseTransfrom = elapsed_seconds.count();

    r.fullTime = r.timeOfReverseTransfrom + r.timeOfTransform;

    fftw_free( outR );
    fftw_free( realV );
    fftw_destroy_plan( fftPlan );
    fftw_destroy_plan( rfftPlan );
    return r;
}
