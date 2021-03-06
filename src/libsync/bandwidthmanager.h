/*
 * Copyright (C) by Markus Goetz <markus@woboq.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
 * for more details.
 */

#ifndef BANDWIDTHMANAGER_H
#define BANDWIDTHMANAGER_H

#include <QObject>
#include <QLinkedList>
#include <QTimer>
#include <QIODevice>

namespace OCC {

class UploadDevice;
class GETFileJob;
class OwncloudPropagator;

/**
 * @brief The BandwidthManager class
 * @ingroup libsync
 */
class BandwidthManager : public QObject {
    Q_OBJECT
public:
    BandwidthManager(OwncloudPropagator *p);
    ~BandwidthManager();

    bool usingAbsoluteUploadLimit() { return _currentUploadLimit > 0; }
    bool usingRelativeUploadLimit() { return _currentUploadLimit < 0; }
    bool usingAbsoluteDownloadLimit() { return _currentDownloadLimit > 0; }
    bool usingRelativeDownloadLimit() { return _currentDownloadLimit < 0; }


public slots:
    void registerUploadDevice(UploadDevice*);
    void unregisterUploadDevice(UploadDevice*);
    void unregisterUploadDevice(QObject*);

    void registerDownloadJob(GETFileJob*);
    void unregisterDownloadJob(GETFileJob*);
    void unregisterDownloadJob(QObject*);

    void absoluteLimitTimerExpired();
    void switchingTimerExpired();

    void relativeUploadMeasuringTimerExpired();
    void relativeUploadDelayTimerExpired();

    void relativeDownloadMeasuringTimerExpired();
    void relativeDownloadDelayTimerExpired();

private:
    QTimer _switchingTimer; // for switching between absolute and relative bw limiting
    OwncloudPropagator *_propagator; // FIXME this timer and this variable should be replaced
    // by the propagator emitting the changed limit values to us as signal

    QTimer _absoluteLimitTimer; // for absolute up/down bw limiting

    QLinkedList<UploadDevice*> _absoluteUploadDeviceList;
    QLinkedList<UploadDevice*> _relativeUploadDeviceList; // FIXME merge with list above ^^
    QTimer _relativeUploadMeasuringTimer;
    QTimer _relativeUploadDelayTimer; // for relative bw limiting, we need to wait this amount before measuring again
    UploadDevice *_relativeLimitCurrentMeasuredDevice; // the device measured
    qint64 _relativeUploadLimitProgressAtMeasuringRestart; // for measuring how much progress we made at start
    qint64 _currentUploadLimit;

    QLinkedList<GETFileJob*> _downloadJobList;
    QTimer _relativeDownloadMeasuringTimer;
    QTimer _relativeDownloadDelayTimer; // for relative bw limiting, we need to wait this amount before measuring again
    GETFileJob *_relativeLimitCurrentMeasuredJob; // the device measured
    qint64 _relativeDownloadLimitProgressAtMeasuringRestart; // for measuring how much progress we made at start
    qint64 _currentDownloadLimit;
};

}

#endif
