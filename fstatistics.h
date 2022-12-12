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

    using extention_statistics_t = QMap<QString, quint32>;
    extention_statistics_t _stat;
    quint32 directoriesCounter;

public:
    FStatistics() : directoriesCounter(0) {}
    ~FStatistics() = default;

    void                   refresh(const QString& directory);
    extention_statistics_t getStatistics() const;
    QPieSeries*            getPieSeries() const;
    QBarSeries*            getBarSeries() const;

};

#endif // FSTATISTICS_H
