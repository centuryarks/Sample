/*
Copyright (c) 2017, CenturyArks
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met: 

1. Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer. 
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution. 

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are those
of the authors and should not be interpreted as representing official policies, 
either expressed or implied, of the FreeBSD Project.
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

/***************************************************************
 *  Defines for Main window
 **************************************************************/
#define VERSION     "CA378-AOIS  Demo  ver 1.0.0"
#define STR_MAX     256

enum E_FOCUS_MODE
{
    FOCUS_MODE_DIRECT = 0,
    FOCUS_MODE_INFINITY,
    FOCUS_MODE_MACRO
};

/***************************************************************
 *  Extern function for Main window
 **************************************************************/
extern "C"
{
int GetSensID();
int DemoInit();
int DemoExit();
int DemoControl(char *commandLine);
int StillCapture(int mode);
int Highspeed();
}

/***************************************************************
 *  Declare class for Main window
 **************************************************************/
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_AF_On_clicked();
    void on_pushButton_AF_Off_clicked();
    void on_lineEdit_FocusPosition_editingFinished();
    void on_radioButton_Direct_clicked();
    void on_radioButton_Infinity_clicked();
    void on_radioButton_Macro_clicked();
    void on_pushButton_FocusApply_clicked();
    void on_radioButton_OIS_Mode0_clicked();
    void on_radioButton_OIS_Mode1_clicked();
    void on_radioButton_OIS_Mode2_clicked();
    void on_radioButton_OIS_Mode3_clicked();
    void on_radioButton_OIS_Mode4_clicked();
    void on_radioButton_OIS_Mode5_clicked();
    void on_pushButton_OIS_Apply_clicked();
    void on_checkBox_LSC_clicked(bool checked);
    void on_pushButton_Still12M_NML_clicked();
    void on_pushButton_Still12M_HDR_clicked();
    void on_pushButton_Highspeed_clicked();

private:
    Ui::MainWindow *ui;
    int focus_position;
    int focus_mode;
    int ois_mode;
    int af_on;
};

#endif // MAINWINDOW_H
