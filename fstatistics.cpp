#include "fstatistics.h"

#include <QDirIterator>
#include <QBarSet>
#include <QtAlgorithms>

using statistics_map = FStatistics::statistics_map;

FStatistics::FStatistics()
{
    _directoriesCounter = 0;
}

void FStatistics::refresh(const QString& directory)
{
    _all_stat.clear();
    _directoriesCounter = 0;

    QDirIterator itDir(directory, QDirIterator::Subdirectories);
    while ( itDir.hasNext() )
    {
        QFileInfo fileInfo = itDir.nextFileInfo();
        if ( ! fileInfo.isDir() )
        {
            QString key = fileInfo.suffix();
            _all_stat[key].size += fileInfo.size();
            ( _all_stat[key].count++ );
        }
        else
            (_directoriesCounter++);
    }

    qDebug() << "Size: " << _all_stat.size();
    qDebug() << "Directories: " << _directoriesCounter;

    for (auto& it : _all_stat)
        qDebug() << it.first << " -> " << it.second.count;
}

statistics_map& FStatistics::getStatistics()
{
    return (_all_stat);
}

QPieSeries* FStatistics::getPieSeries() const
{
    const quint32 importantSize = std::min(MAX_IMPORTANT_SIZE, _all_stat.size());
    important_statistics_vector top(importantSize);

    std::partial_sort_copy(_all_stat.begin(), _all_stat.end(),
                           top.begin(), top.end(),
                           [](const Pair& lhs, const Pair& rhs)
                           { return lhs.second.count > rhs.second.count; });

    QPieSeries* series = new QPieSeries();
    for (auto& set : top)
    {
        QPieSlice *pieSlice = new QPieSlice(set.first, set.second.count);
        series->append(pieSlice);
    }
    return (series);
}

QBarSeries* FStatistics::getBarSeries() const
{
    const quint32 importantSize = std::min(MAX_IMPORTANT_SIZE, _all_stat.size());
    important_statistics_vector top(importantSize);

    std::partial_sort_copy(_all_stat.begin(), _all_stat.end(),
                           top.begin(), top.end(),
                           [](const Pair& lhs, const Pair& rhs)
                           { return lhs.second.count > rhs.second.count; });

    QBarSeries* series = new QBarSeries();
    for (auto& set : top)
    {
        QBarSet *barSet = new QBarSet(set.first);
        *barSet << set.second.count;
        series->append(barSet);
    }
    return (series);
}

// *********************** PRIVATE MEMBERS **************************
