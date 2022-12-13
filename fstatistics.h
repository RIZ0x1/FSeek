#ifndef FSTATISTICS_H
#define FSTATISTICS_H

#include <QObject>
#include <QString>
#include <QPieSeries>
#include <QBarSeries>
#include <QMap>

class FStatistics : public QObject
{
    Q_OBJECT

    using Pair = std::pair<QString, quint32>;
    using statistics_map = std::map<QString, quint32>;
    using statistics_map_iterator = std::map<QString, quint32>::iterator;
    using important_statistics_vector = std::vector<Pair>;

    const statistics_map::size_type MAX_IMPORTANT_SIZE = 8;
    statistics_map                  _all_stat;
    quint32                         _directoriesCounter;

//    important_statistics_vector getImportant();

public:
    FStatistics() : _directoriesCounter(0) {}
    ~FStatistics() = default;

    void                   refresh(const QString& directory);
    statistics_map         getStatistics() const;
    QPieSeries*            getPieSeries() const;
    QBarSeries*            getBarSeries() const;

};

#endif // FSTATISTICS_H
