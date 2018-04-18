/*
# Copyright (c) 2018, Ole-André Rodlie <ole.andre.rodlie@gmail.com> All rights reserved.
#
# Available under the 3-clause BSD license
# See the LICENSE file for full details
*/

#include "systray.h"
#include <QIcon>
#include <QProcess>
#include <QTimer>
#include <QMenu>
#include <QAction>
#include <QDebug>

SysTray::SysTray(QObject *parent)
    : QObject(parent)
    , tray(0)
    , menu(0)
    , man(0)
{
    menu = new QMenu();

    tray = new QSystemTrayIcon(QIcon::fromTheme("drive-removable-media"), this);
    connect(tray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(trayActivated(QSystemTrayIcon::ActivationReason)));
    connect(tray, SIGNAL(messageClicked()), this, SLOT(handleMessageClicked()));

    man  = new Manager(this);
    connect(man , SIGNAL(updatedDevices()), this, SLOT(generateContextMenu()));
    connect(man, SIGNAL(deviceErrorMessage(QString,QString)), this, SLOT(handleDeviceError(QString,QString)));
    connect(man, SIGNAL(mediaChanged(QString,bool)), this, SLOT(handleDeviceMediaChanged(QString,bool)));
    connect(man, SIGNAL(mountpointChanged(QString,QString)), this, SLOT(handleDeviceMountpointChanged(QString,QString)));
}

void SysTray::generateContextMenu()
{
    /*for(int i=0;i<contextMenu->actions().size();i++) {
        contextMenu->actions().at(i)->disconnect();
        delete contextMenu->actions().at(i);
    }
    contextMenu->clear();
    QVector<QStringList> devices = uDisks2::getRemovableDevices();
    //qDebug() << "devices" << devices;
    if (devices.size()==0) { return; }
    for (int i=0;i<devices.size();i++) {
        if (devices.at(i).size()<2) { continue; }
        QString path = devices.at(i).at(0);
        QString drive = uDisks2::getDrivePath(path);

        bool isOptical = false;
        if (devices.at(i).size()>=4) {
            if (devices.at(i).at(3) == "optical") { isOptical = true; }
        }
        if (isOptical) {
            if (!monitoredDevices.contains(drive)) {
                monitoredDevices << drive;
                QDBusConnection system = QDBusConnection::systemBus();
                system.connect(DBUS_SERVICE, drive, DBUS_PROPERTIES, "PropertiesChanged", this, SLOT(handlePropertiesChanged(const QString&,const QMap<QString, QVariant>&)));
            }
            if (!uDisks2::hasMedia(drive)) { continue; } // optical don't have media, don't add to menu
        }

        QAction *devAction = new QAction(this);
        QString title;
        QString mountpoint;
        if (devices.at(i).size()>=3) { mountpoint = devices.at(i).at(2); }
        if (mountpoint.isEmpty()) {
            devAction->setIcon(QIcon::fromTheme(isOptical?"drive-optical":"drive-removable-media"));
            if (isOptical) {
                bool hasAudio = uDisks2::opticalAudioTracks(drive)>0?true:false;
                bool hasData = uDisks2::opticalDataTracks(drive)>0?true:false;
                bool hasBlank = uDisks2::opticalMediaIsBlank(drive);
                if (hasBlank||(hasAudio&&!hasData)) { devAction->setIcon(QIcon::fromTheme("media-eject")); }
            }
        }
        else { devAction->setIcon(QIcon::fromTheme("media-eject")); }
        devAction->setText(devices.at(i).at(1));
        devAction->setData(path);
        connect(devAction, SIGNAL(triggered(bool)), this, SLOT(handleContextMenuAction()));
        contextMenu->addAction(devAction);
    }

    if (contextMenu->actions().size()==0) {
        if (tray->isVisible()) { tray->hide(); }
    } else {
        if (!tray->isVisible() && tray->isSystemTrayAvailable()) { tray->show(); }
    }*/
}

void SysTray::trayActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason) {
    case QSystemTrayIcon::Context:
    case QSystemTrayIcon::Trigger:
        if (menu->actions().size()>0) { menu->popup(QCursor::pos()); }
    default:;
    }
}

void SysTray::handleMessageClicked()
{
    // Do something?
}

void SysTray::handleContextMenuAction()
{
    QAction *action = qobject_cast<QAction*>(sender());
    if (action==NULL) { return; }

    /*QString actionText = action->text();
    QString path = action->data().toString();
    QString drive = uDisks2::getDrivePath(path);
    bool isOptical = uDisks2::isOptical(drive);
    bool hasMedia = uDisks2::hasOpticalMedia(drive);
    QString mountpoint = uDisks2::getMountPoint(path);
    bool hasAudio = uDisks2::opticalAudioTracks(drive)>0?true:false;
    bool hasData = uDisks2::opticalDataTracks(drive)>0?true:false;
    bool hasBlank = uDisks2::opticalMediaIsBlank(drive);

    if (isOptical && hasMedia) {
        bool wasUnmounted = false;
        if (!mountpoint.isEmpty()) { // unmount device
            QString unmounted = uDisks2::unmountDevice(path);
            qDebug() << "unmounted?" << unmounted;
            if (!unmounted.isEmpty()) {
                if (tray->isVisible()) { tray->showMessage(tr("Failed to unmount %1").arg(actionText), tr("Failed to unmount the device, please check that none application are using any files on this device.")); }
                return;
            }
            wasUnmounted = true;
        }
        if (hasBlank || wasUnmounted || (hasAudio && !hasData)) { // eject device
            QString ejected = uDisks2::ejectDevice(drive);
            qDebug() << "ejected?" << ejected;
            if (!ejected.isEmpty()) {
                if (tray->isVisible()) { tray->showMessage(tr("Failed to eject %1").arg(actionText), tr("Failed to eject the device, this should not happen, is it still mounted?")); }
            } else { generateContextMenu(); }
            return;
        }
    }

    if (!mountpoint.isEmpty()) { // unmount device
        uDisks2::unmountDevice(path);
        mountpoint = uDisks2::getMountPoint(path);
        if (!mountpoint.isEmpty()) {
            if (tray->isVisible()) { tray->showMessage(tr("Failed to unmount %1").arg(actionText), tr("Failed to unmount the device, please check that none application are using any files on this device.")); }
        } else {
            if (tray->isVisible()) { tray->showMessage(tr("%1 removed").arg(actionText), tr("It's now safe to remove %1 from your computer.").arg(actionText)); }
            generateContextMenu();
        }
    } else { // mount device
        QStringList mounted = uDisks2::mountDevice(path);
        generateContextMenu();
        if (!mounted.at(0).isEmpty()) {
            // TODO: check for DVD
            QProcess::startDetached(QString("lumina-open %1").arg(mounted.at(0)));
        } else {
            QString msg = tr("Failed to mount volume %1, this is probably due to an unsupported file system (%2) or permissions.").arg(actionText).arg(uDisks2::getFileSystem(path));
            if (mounted.size()>1) { msg = mounted.at(1); }
            if (tray->isVisible()) { tray->showMessage(tr("Failed to mount %1").arg(actionText), msg); }
        }
    }*/
}

void SysTray::handleDeviceError(QString path, QString error)
{
    qDebug() << "handle device error" << path << error;
    if (!tray->isSystemTrayAvailable()||!man->devices.contains(path)) { return; }
    tray->showMessage(QString("Error for device %1").arg(man->devices[path]->name), error);
}

void SysTray::handleDeviceMediaChanged(QString path, bool media)
{
    qDebug() << "handle device media changed" << path << media;
    if (!tray->isSystemTrayAvailable()||!man->devices.contains(path)) { return; }
    generateContextMenu();
}

void SysTray::handleDeviceMountpointChanged(QString path, QString mountpoint)
{
    qDebug() << "handle device mountpoint changed" << path << mountpoint;
    if (!tray->isSystemTrayAvailable()||!man->devices.contains(path)) { return; }
    generateContextMenu();
}
