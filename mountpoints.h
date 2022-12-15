#ifndef MOUNTPOINTS_H
#define MOUNTPOINTS_H

#include <QObject>
#include <QStringList>
#include <QStorageInfo>

using volumes_list_t = QList<QStorageInfo>;

class MountPoints : public QObject
{
    Q_OBJECT

    volumes_list_t _mountedVolumes;

    MountPoints() = default;
    virtual ~MountPoints() = default;

    bool isSystemMount(const QStorageInfo& storage) const; // *
    bool isDuplicate(const QStorageInfo& storage) const;   // * Most of these methods
    bool isSnapPackage(const QStorageInfo& storage) const; // * are for linux
    bool isValidVolume(const QStorageInfo& storage) const; // *

public:
    static MountPoints&  self();
    volumes_list_t       getNormalVolumes() const;

public slots:
    void refreshMountedVolumes();

};

#endif // MOUNTPOINTS_H
