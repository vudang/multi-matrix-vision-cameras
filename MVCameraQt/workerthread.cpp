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

#include "workerthread.h"

//-----------------------------------------------------------------------------
WorkerThread::WorkerThread( DeviceManager *pDevMgr_, QObject* parent ) : QThread(parent),
pDevMgr(pDevMgr_), pDev(0), DoRunThread(false), frameNr(0)
//-----------------------------------------------------------------------------
{
	iWidth = 640;
	iHeight = 480;
}

//-----------------------------------------------------------------------------
void WorkerThread::OpenDevice( unsigned int index, unsigned int w, unsigned int h )
//-----------------------------------------------------------------------------
{
	iWidth = w;
	iHeight = h;
	
	if( pDev && pDevMgr->getDevice(index)->serial.readS().compare(pDev->serial.readS()) )
	{
		printf("Closing device %s.\n", pDev->serial.readS().c_str() );
		pDev->close();
	}
	
	pDev = pDevMgr->getDevice(index);
	
	if( !pDev->family.readS().compare("mvBlueFOX") )
		OpenBlueFOXDevice();
	else if( !pDev->family.readS().compare("mvVirtualDevice") )
		OpenVirtualDevice();
	else if( !pDev->family.readS().compare("mvBlueLYNX") )
		OpenBlueLYNXDevice();
}

//-----------------------------------------------------------------------------
void WorkerThread::OpenBlueLYNXDevice(void)
//-----------------------------------------------------------------------------
{
    if(pDev)
    {
		if( !pDev->isOpen() )
		{
			pDev->interfaceLayout.write( dilGenICam );
			pDev->open();
		}

        SystemSettings sys(pDev);
        sys.requestCount.write(4);

        pFI = new FunctionInterface(pDev);

        mvIMPACT::acquire::GenICam::AcquisitionControl ac(pDev);
        mvIMPACT::acquire::GenICam::ImageFormatControl ifc(pDev);
        mvIMPACT::acquire::GenICam::CounterAndTimerControl ctc(pDev);

        int maxw = ifc.widthMax.read();
        int maxh = ifc.heightMax.read();

        int xoff = (maxw - iWidth) / 2;
        int yoff = (maxh - iHeight) / 2;

        if( xoff < 0 )
            xoff = 0;
        if( yoff < 0 )
            yoff = 0;

        ifc.width.write( iWidth );
        ifc.height.write( iHeight );
        ifc.offsetX.write( xoff );
        ifc.offsetY.write( yoff );
		
        try
        {
        	ifc.pixelFormat.writeS( "YUV422Packed" );
        	IsColorSensor = true; // if it's not, an exception is thrown when writing pixelFormat=YUV422Packed
        }
        catch( ... )
        {
        	ImageDestination id(pDev);
        	id.pixelFormat.writeS("YUV422_UYVYPacked");
        	IsColorSensor = false;
        }

        ac.acquisitionMode.writeS( "Continuous" );
        ac.exposureTime.write(20000.0);

        ctc.timerSelector.writeS( "Timer1" );
        ctc.timerTriggerSource.writeS( "Timer1End" );
        ctc.timerDuration.write( 83333.0 ); // 12Hz = 83333.0

        ac.triggerSource.writeS( "Timer1End" );
        ac.triggerMode.writeS( "On" );
        
        pGL->SetVideoSrcIsYUV(true);
        emit EnableMenuActions(true); // AWB & AE
    }
}

//-----------------------------------------------------------------------------
void WorkerThread::OpenBlueFOXDevice(void)
//-----------------------------------------------------------------------------
{
    if(pDev)
    {
		if( !pDev->isOpen() )
			pDev->open();

        pFI = new FunctionInterface(pDev);

        mvIMPACT::acquire::CameraSettingsBase cs(pDev);
        mvIMPACT::acquire::CameraSettingsBlueFOX csbf(pDev);
        mvIMPACT::acquire::ImageDestination id(pDev);
		
        int maxw = cs.aoiWidth.getMaxValue();
        int maxh = cs.aoiHeight.getMaxValue();

        int xoff = (maxw - iWidth) / 2;
        int yoff = (maxh - iHeight) / 2;

        if( xoff < 0 )
            xoff = 0;
        if( yoff < 0 )
            yoff = 0;

        cs.aoiWidth.write(iWidth);
        cs.aoiHeight.write(iHeight);
        cs.aoiStartX.write(xoff);
        cs.aoiStartY.write(yoff);
        csbf.pixelClock_KHz.write(cpc40000KHz);
        csbf.expose_us.write(20000);
        id.pixelFormat.write(idpfRGBx888Packed);
        
        pGL->SetVideoSrcIsYUV(false);
		emit EnableMenuActions(false); // AWB & AE
    }
}

//-----------------------------------------------------------------------------
void WorkerThread::OpenVirtualDevice(void)
//-----------------------------------------------------------------------------
{
    if(pDev)
    {
		if( !pDev->isOpen() )
			pDev->open();

        pFI = new FunctionInterface(pDev);

        mvIMPACT::acquire::CameraSettingsVirtualDevice csvd(pDev);
        mvIMPACT::acquire::CameraSettingsBase cs(pDev);
        mvIMPACT::acquire::ImageDestination id(pDev);

        cs.aoiWidth.write(iWidth);
        cs.aoiHeight.write(iHeight);
        cs.aoiStartX.write(0);
        cs.aoiStartY.write(0);

        #ifdef ARM
        csvd.frameDelay_us.write(40000);
        #else
        csvd.frameDelay_us.write(10000);
        #endif

        csvd.testMode.write(vdtmMovingBayerDataRamp);
        id.pixelFormat.write(idpfRGBx888Packed);
        
        pGL->SetVideoSrcIsYUV(false);
		emit EnableMenuActions(false); // AWB & AE
    }
}

//-----------------------------------------------------------------------------
void WorkerThread::StartThread()
//-----------------------------------------------------------------------------
{
    if( pDev && !isRunning() && !DoRunThread )
    {
        DoRunThread = true;
        pGL->VideoEnable(true);
        start();
    }
}

//-----------------------------------------------------------------------------
void WorkerThread::StopThread()
//-----------------------------------------------------------------------------
{
    DoRunThread = false;
    pGL->VideoEnable(false);
    msleep(100);// give thread a chance to exit ordinary
    if(isRunning())
    {
        pGL->ForceUnlockVideoSyncMutex();
    }
}

//-----------------------------------------------------------------------------
void WorkerThread::run(void)
//-----------------------------------------------------------------------------
{
	unsigned char* buf = NULL;
    int requestNr;
    const Request* pRequest = 0;
    unsigned char cnt = 0;
    StartAcquisition();

    while( DoRunThread )
    {
	    cnt++;
	    pGL->VideoSyncMutexLock();
        requestNr = DequeueVideo();
        if( pFI->isRequestNrValid( requestNr ) )
        {
            pRequest = pFI->getRequest( requestNr );
            if( pRequest->isOK() )
            {
	            buf = (unsigned char*)pRequest->imageData.read();
	            
	            emit DoLoadTexture( buf );
	            
	            frameNr = pRequest->infoFrameNr.read();
	            
	            if( frameNr % 10 == 0)
                {
                    Statistics ss(pDev);
                    char buf[16];
                    sprintf(buf, "%3.1f", ss.framesPerSecond.read() );

                    QString message;
                    message += QString(buf);
                    message += " fps - ";
                    message += pRequest->imageWidth.readS().c_str();
                    message += "x";
                    message += pRequest->imageHeight.readS().c_str();
                    message += " - frame#: ";
                    message += pRequest->infoFrameNr.readS().c_str();
                    emit DoUpdateStatusBar(message);
                }
            }
        	emit DoUpdateGL( requestNr );            
        }
        ReleaseRequest(requestNr);
    }
    
    StopAcquisition();
    DoRunThread = false;
    exit(0);
}

//-----------------------------------------------------------------------------
void WorkerThread::StartAcquisition( void )
//-----------------------------------------------------------------------------
{
    int numRequests = 0;
    int requestResult = DMR_NO_ERROR;

    while( ( requestResult = pFI->imageRequestSingle() ) == DMR_NO_ERROR )
    {
        numRequests++;
    }
    printf("Starting videostream and using %d buffers\n", numRequests );
    pFI->acquisitionStart();
}

//-----------------------------------------------------------------------------
void WorkerThread::StopAcquisition( void )
//-----------------------------------------------------------------------------
{
    pFI->acquisitionStop();
    pFI->imageRequestReset(0,0);
    pGL->update();
}

//-----------------------------------------------------------------------------
int WorkerThread::DequeueVideo( void )
//-----------------------------------------------------------------------------
{
    if(!pDev || !pFI )
        return -1;

    return pFI->imageRequestWaitFor(2000);
}

//-----------------------------------------------------------------------------
void WorkerThread::ReleaseRequest(int requestNr)
//-----------------------------------------------------------------------------
{
    pGL->VideoSyncMutexLock();
    pFI->imageRequestUnlock(requestNr);
    pFI->imageRequestSingle();
    pGL->VideoSyncMutexUnlock();
}

//-----------------------------------------------------------------------------
void WorkerThread::DoWhiteBalanceIA(bool arg)
//-----------------------------------------------------------------------------
{
    if( !std::string("mvBlueLYNX").compare( pDev->family.readS() ) && IsColorSensor )
    {
		mvIMPACT::acquire::GenICam::mvOMAPPreviewConfig pc(pDev);

        if( arg )
            pc.mvBalanceWhiteAuto.writeS("Continuous");
        else
            pc.mvBalanceWhiteAuto.writeS("Off"); // 2 || Off
    }
}

//-----------------------------------------------------------------------------
void WorkerThread::DoAutoExposureIA(bool arg)
//-----------------------------------------------------------------------------
{
    if( !std::string("mvBlueLYNX").compare( pDev->family.readS() ) && IsColorSensor )
    {
        mvIMPACT::acquire::GenICam::mvOMAPPreviewConfig pc(pDev);

        if( arg )
            pc.mvAutoExposure.writeS("Continuous");
        else
            pc.mvAutoExposure.writeS("Off");
    }
}

