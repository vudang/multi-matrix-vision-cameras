/*
* Copyright (c) 2014-2015 Vu T. Dang <dangtranvu@{gmail.com, ieee.org}>
*
* Permission to use, copy, modify, and distribute this software for any
* purpose with or without fee is hereby granted, provided that the above
* copyright notice and this permission notice appear in all copies.
*
* THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
* WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
* ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
* WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
* ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
* OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#ifndef WORKERTHREAD_H
#define WORKERTHREAD_H

#include <QtCore>
#include <gleswidget.h>

#include <mvIMPACT_CPP/mvIMPACT_acquire_GenICam.h>
#include <mvIMPACT_CPP/mvIMPACT_acquire.h>

using namespace std;
using namespace mvIMPACT::acquire;

//-----------------------------------------------------------------------------
class WorkerThread : public QThread
//-----------------------------------------------------------------------------
{
    Q_OBJECT

    void run(void);

	mvIMPACT::acquire::DeviceManager*         pDevMgr;
    mvIMPACT::acquire::Device*                pDev;
    unsigned int                              iWidth;
    unsigned int                              iHeight;
    bool                                      DoRunThread;
    GLESWidget*                               pGL;
    FunctionInterface*                        pFI;
    bool                                      IsColorSensor;
    int                                       frameNr;
    
    void OpenBlueLYNXDevice(void);
    void OpenBlueFOXDevice(void);
    void OpenVirtualDevice(void);
    void StartAcquisition( void );
    void StopAcquisition( void );
    int DequeueVideo( void );
    void ReleaseRequest(int requestNr);

signals:
    void DoLoadTexture( unsigned char* inbuffer );
    void DoUpdateGL( int reqNr );
    void DoUpdateStatusBar( QString message );
    void EnableMenuActions( bool arg );

public:
    WorkerThread( DeviceManager *pDevMgr_, QObject* parent = 0 );
    void AttachGLWidget( GLESWidget* pGL_ ) { pGL = pGL_; }
    void OpenDevice(unsigned int index, unsigned int w, unsigned int h);
    void StartThread( void );
    void StopThread( void );
    
public slots:
    void DoWhiteBalanceIA(bool arg);
    void DoAutoExposureIA(bool arg);
};

#endif // WORKERTHREAD_H
