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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <mvIMPACT_CPP/mvIMPACT_acquire.h>
#include <gleswidget.h>
#include <workerthread.h>
#include <stdlib.h>
#include <string>
#include <map>

#include <mvIMPACT_CPP/mvIMPACT_acquire.h>

using namespace std;

namespace Ui {
    class MainWindow;
}

//-----------------------------------------------------------------------------
class MainWindow : public QMainWindow
//-----------------------------------------------------------------------------
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
	unsigned int iWidth;
	unsigned int iHeight;
	QComboBox *ComboBoxDevices;
	mvIMPACT::acquire::DeviceManager* pDevMgr;
    WorkerThread* pWorker_;
    GLESWidget* pGLESWidget_;
	QSet<unsigned int> DeviceSet;
	
	void DetectDevices(void);

public:
    QWidget* GLWidget(void);
    
public slots:
    void DoUpdateGL( int reqNr );
    void UpdateStatusBar( QString message );
    void EnableMenuActions( bool arg );

private slots:
    void on_actionLive_toggled(bool arg1);
    void on_actionWhitBalance_triggered(bool checked);
    void on_actionAutoExposure_triggered(bool checked);

signals:
    void DoWhiteBalance(void);
    void DoWhiteBalanceIA(bool arg);
    void DoAutoExposureIA(bool arg);
    void UpdateGL( int reqNr ); //unused
};

#endif // MAINWINDOW_H
