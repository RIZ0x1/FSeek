#ifndef FSTATISTICS_H
#define FSTATISTICS_H

#include <QObject>
#include <QString>
#include <QPieSeries>
#include <QBarSeries>
#include <QMap>

struct FileFormatInfo
{
    unsigned int count = 0;
    unsigned int size = 0;
};

class FStatistics : public QObject
{
    Q_OBJECT

public:
    using Pair = std::pair<QString, FileFormatInfo>;
    using statistics_map = std::map<QString, FileFormatInfo>;
    using statistics_map_iterator = std::map<QString, FileFormatInfo>::iterator;
    using important_statistics_vector = std::vector<Pair>;

    FStatistics();
    ~FStatistics() = default;

    void                   refresh(const QString& directory);
    QPieSeries*            getPieSeries() const;
    QBarSeries*            getBarSeries() const;
    statistics_map&        getStatistics();

private:
    const statistics_map::size_type MAX_IMPORTANT_SIZE = 8;
    statistics_map                  _all_stat;
    quint32                         _directoriesCounter;

    //    important_statistics_vector getImportant();

};

#endif // FSTATISTICS_H
