#include "fstatistics.h"

#include <QDirIterator>
#include <QBarSet>
#include <QtAlgorithms>

using statistics_map = FStatistics::statistics_map;
using important_statistics_vector = FStatistics::important_statistics_vector;

FStatistics::FStatistics()
{
    _directoriesCounter = 0;
    _statistics = nullptr;
    _workerThread = nullptr;
}

void FStatistics::refresh(const QString& directory)
{
    if (_workerThread && _workerThread->isRunning() )
        _workerThread->requestInterruption();

    _workerThread = new WorkerThread(directory);
    connect(_workerThread, &WorkerThread::resultReady, this, &FStatistics::getResult);
    connect(_workerThread, &WorkerThread::finished, _workerThread, &QObject::deleteLater);
    _workerThread->start();
}

statistics_map& FStatistics::getStatistics()
{
    return ( *_statistics );
}

important_statistics_vector FStatistics::getImportant() const
{
    if (_statistics == nullptr)
        return important_statistics_vector();

    const quint32 importantSize = std::min(MAX_IMPORTANT_SIZE, _statistics->size());
    important_statistics_vector important(importantSize);

    std::partial_sort_copy(_statistics->begin(), _statistics->end(),
                           important.begin(), important.end(),
                           [](const Pair& lhs, const Pair& rhs)
                           { return lhs.second.count > rhs.second.count; });
    return (important);
}

QPieSeries* FStatistics::getPieSeries() const
{
    QPieSeries* series = new QPieSeries();
    for (auto& set : getImportant())
    {
        QPieSlice *pieSlice = new QPieSlice(set.first, set.second.count);
        series->append(pieSlice);
    }
    return (series);
}

QBarSeries* FStatistics::getBarSeries() const
{
    QBarSeries* series = new QBarSeries();
    for (auto& set : getImportant())
    {
        QBarSet *barSet = new QBarSet(set.first);
        *barSet << set.second.count;
        series->append(barSet);
    }
    return (series);
}

void FStatistics::getResult(FStatistics::statistics_map* statistics, quint32 directoriesCounter)
{
    _statistics = statistics;
    _directoriesCounter = directoriesCounter;
    _workerThread = nullptr;
    emit resultReady();
}

// *****************************************************
// ****************** WorkerThread *********************
// *****************************************************

void WorkerThread::run()
{
    statistics_map *statistics = new statistics_map;
    statistics_map &stat = *statistics;
    quint32        directoriesCounter = 0;

    QDirIterator itDir(directory, QDirIterator::Subdirectories);
    while ( itDir.hasNext() )
    {
        if ( QThread::isInterruptionRequested() )
        {
            delete statistics;
            return ;
        }

        QFileInfo fileInfo = itDir.nextFileInfo();
        if ( ! fileInfo.isDir() )
        {
            QString key = fileInfo.suffix();
            stat[key].size += fileInfo.size();
            ( stat[key].count++ );
        }
        else
            (directoriesCounter++);
    }
    emit resultReady(statistics, directoriesCounter);
}
