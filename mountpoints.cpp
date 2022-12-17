#include "mountpoints.h"

bool MountPoints::isSystemMount(const QStorageInfo& storage) const
{
    const QString device = storage.device();
    if ( ! device.contains( "/" ) ) return (true);

    const QString path = storage.rootPath();
    if ( path.startsWith("/dev")  ) return (true);
    if ( path.startsWith("/proc") ) return (true);
    if ( path.startsWith("/sys")  ) return (true);

    return (false);
}

bool MountPoints::isSnapPackage(const QStorageInfo& storage) const
{
    QString path = storage.rootPath();
    QString fsType = storage.fileSystemType();

    return ( path.startsWith("/snap") && (fsType.toLower() == "squashfs") );
}

bool MountPoints::isValidVolume(const QStorageInfo& storage) const
{
    return ((storage.blockSize() > 0) && !(storage.fileSystemType().toLower() == "autofs") );
}

// ****** PUBLIC *******

MountPoints& MountPoints::self()
{
    static MountPoints instance;
    return (instance);
}

volumes_list_t MountPoints::getNormalVolumes() const
{
#ifdef Q_OS_WIN
     return (_mountedVolumes);
#endif
#ifdef Q_OS_UNIX
    volumes_list_t normalVolumes;
    foreach (QStorageInfo volume, _mountedVolumes)
    {
        if ( !isSystemMount(volume) && !isSnapPackage(volume) && isValidVolume(volume))
            normalVolumes.append(volume);
    }
    return (normalVolumes);
#endif
}

void MountPoints::refreshMountedVolumes()
{
    _mountedVolumes.clear();

    foreach (QStorageInfo volume, QStorageInfo::mountedVolumes()) {
        if ( volume.isValid() && volume.isReady() )
            _mountedVolumes.append(volume);
    }
}
