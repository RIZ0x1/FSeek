#ifndef FSTATISTICS_H
#define FSTATISTICS_H

#include <QObject>
#include <QThread>
#include <QString>
#include <QPieSeries>
#include <QBarSeries>
#include <QMap>

class WorkerThread;

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

    void             refresh(const QString& directory);
    QPieSeries*      getPieSeries() const;
    QBarSeries*      getBarSeries() const;
    statistics_map&  getStatistics();

private:
    const statistics_map::size_type MAX_IMPORTANT_SIZE = 8;
    WorkerThread                    *_workerThread;
    statistics_map                  *_statistics;
    quint32                         _directoriesCounter;

    important_statistics_vector     getImportant() const;

private slots:
    void getResult(FStatistics::statistics_map* statistics, quint32 directoriesCounter);

signals:
    void resultReady();
};

class WorkerThread : public QThread
{
    Q_OBJECT
    QString directory;

    void run() override;
public:
    WorkerThread(const QString& directory) : directory(directory) {}

signals:
    void resultReady(FStatistics::statistics_map* statistics, quint32 directoriesCounter);
};

#endif // FSTATISTICS_H
