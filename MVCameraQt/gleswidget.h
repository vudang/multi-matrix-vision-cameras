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

#ifndef GLESWIDGET_H
#define GLESWIDGET_H

#include <QtCore>
#include <QtOpenGL>
#include <QtOpenGL/qglshaderprogram.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <QMutex>

using namespace std;

//-----------------------------------------------------------------------------
class GLESWidget : public QGLWidget
//-----------------------------------------------------------------------------
{
    Q_OBJECT

    unsigned int           iWidth;
    unsigned int           iHeight;
    float                  fWidth;
    float                  fHeight;
    QGLShaderProgram       ShaderProgram_;
    GLuint                 texId;
    int                    frameNr;
    GLuint                 baseMapTexId;
    bool                   AcquisitionStarted;
    QMutex                 VideoSyncMutex;
    bool                   IsInitialised;
    bool                   VideoSrcIsYUV;
    bool                   TextureLoaded;
    
    char* LoadShader( std::string filename );
    
public:
    GLESWidget(unsigned int w, unsigned int h, QWidget *parent = 0);
    ~GLESWidget();

    void VideoEnable( bool enable );
    void VideoSyncMutexLock( void )	{ VideoSyncMutex.lock(); }
    void VideoSyncMutexUnlock( void ) { VideoSyncMutex.unlock(); }
    void ForceUnlockVideoSyncMutex(void) { VideoSyncMutex.unlock(); }
    void SetVideoSrcIsYUV( bool mode ) { VideoSrcIsYUV = mode; }

protected:
    void initializeGL();
    void paintGL();
    void DoPaint(void);

public slots:
    void DoLoadTexture( unsigned char* inbuffer );
    void DoUpdateGL( int reqNr );
};

#endif // GLESWIDGET_H
