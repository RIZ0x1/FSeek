#include "fstatistics.h"

#include <QDirIterator>
#include <QBarSet>

using extention_statistics_t = QMap<QString, quint32>;

void FStatistics::refresh(const QString& directory)
{
    _stat.clear();
    directoriesCounter = 0;

    QDirIterator itDir(directory, QDirIterator::Subdirectories);

    while ( itDir.hasNext() )
    {
        QFileInfo fileInfo = itDir.nextFileInfo();
        if ( ! fileInfo.isDir() )
        {
            QString key = fileInfo.suffix();
            if ( ! _stat.contains(key) )
                _stat[key] = 1;
            else
                ( _stat[key]++ );
        }
        else
            (directoriesCounter++);
    }

    qDebug() << "Size: " << _stat.size();
    qDebug() << "Directories: " << directoriesCounter;

    QMapIterator<QString,quint32> it(_stat);
    while ( it.hasNext() )
        qDebug() << it.next().key() << " -> " << it.value();
}

extention_statistics_t FStatistics::getStatistics() const
{
    return (_stat);
}

QPieSeries* FStatistics::getPieSeries() const
{
    QPieSeries* series = new QPieSeries();
    QMapIterator<QString, quint32> it(_stat);

    while ( it.hasNext() )
    {
        QPieSlice *pieSlice = new QPieSlice(it.next().key(), it.value());
        series->append(pieSlice);
    }

    return (series);
}

QBarSeries* FStatistics::getBarSeries() const
{
    QBarSeries* series = new QBarSeries();
    QMapIterator<QString, quint32> it(_stat);

    while ( it.hasNext() )
    {
        QBarSet *barSet = new QBarSet(it.next().key());
        *barSet << it.value();
        series->append(barSet);
    }
    return (series);
}
