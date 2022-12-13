#include "fstatistics.h"

#include <QDirIterator>
#include <QBarSet>
#include <QtAlgorithms>

using statistics_map = std::map<QString, quint32>;
using important_statistics_vector = std::vector<QString, quint32>;

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
            if ( !_all_stat.count(key) )
                _all_stat[key] = 1;
            else
                ( _all_stat[key]++ );
        }
        else
            (_directoriesCounter++);
    }

    qDebug() << "Size: " << _all_stat.size();
    qDebug() << "Directories: " << _directoriesCounter;

    for (auto& it : _all_stat)
        qDebug() << it.first << " -> " << it.second;
}

statistics_map FStatistics::getStatistics() const
{
    return (_all_stat);
}

QPieSeries* FStatistics::getPieSeries() const
{
    const quint32 importantSize = std::min(MAX_IMPORTANT_SIZE, _all_stat.size());
    important_statistics_vector top(importantSize);

    std::partial_sort_copy(_all_stat.begin(), _all_stat.end(),
                           top.begin(), top.end(),
                           [](const Pair& lhs, const Pair& rhs){ return lhs.second > rhs.second; });

    QPieSeries* series = new QPieSeries();
    for (auto& set : top)
    {
        QPieSlice *pieSlice = new QPieSlice(set.first, set.second);
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
                           [](const Pair& lhs, const Pair& rhs){ return lhs.second > rhs.second; });

    QBarSeries* series = new QBarSeries();
    for (auto& set : top)
    {
        QBarSet *barSet = new QBarSet(set.first);
        *barSet << set.second;
        series->append(barSet);
    }
    return (series);
}

// *********************** PRIVATE MEMBERS **************************
