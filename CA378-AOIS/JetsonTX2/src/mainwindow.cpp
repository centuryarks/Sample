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

#include <stdio.h>
#include "mainwindow.h"
#include "ui_mainwindow.h"


/*******************************************************************************
 * @brief   Main window constructor
 *
 * @param   parent  Widget parent
 *
 * @return  void
 ******************************************************************************/
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    char string[STR_MAX];

    DemoInit();

    focus_mode  = FOCUS_MODE_DIRECT;
    ois_mode = 0;
    focus_position = 512;
    af_on = 0;

    ui->setupUi(this);
    ui->radioButton_Direct->setChecked(true);
    ui->radioButton_OIS_Mode0->setChecked(true);
    ui->lineEdit_FocusPosition->setText("512");

    ui->pushButton_Still12M_HDR->setEnabled(false);
    ui->pushButton_Highspeed->setEnabled(false);

    sprintf(string, "%s  [sensor-id=%d]", VERSION, GetSensID());
    this->setWindowTitle(string);
}

/*******************************************************************************
 * @brief   Main window destructor
 *
 * @param   void
 *
 * @return  void
 ******************************************************************************/
MainWindow::~MainWindow()
{
    DemoExit();

    delete ui;
}

/*******************************************************************************
 * @brief   AF ON clicked on push button
 *
 * @param   void
 *
 * @return  void
 ******************************************************************************/
void MainWindow::on_pushButton_AF_On_clicked()
{
    ui->pushButton_FocusApply->setEnabled(false);
    ui->pushButton_AF_On->setEnabled(false);
    ui->radioButton_Direct->setEnabled(false);
    ui->radioButton_Infinity->setEnabled(false);
    ui->radioButton_Macro->setEnabled(false);

    DemoControl((char*)"af 1");
    af_on = 1;
}

/*******************************************************************************
 * @brief   AF OFF clicked on push button
 *
 * @param   void
 *
 * @return  void
 ******************************************************************************/
void MainWindow::on_pushButton_AF_Off_clicked()
{
    af_on = 0;
    DemoControl((char*)"af 0");

    ui->pushButton_FocusApply->setEnabled(true);
    ui->pushButton_AF_On->setEnabled(true);
    ui->radioButton_Direct->setEnabled(true);
    ui->radioButton_Infinity->setEnabled(true);
    ui->radioButton_Macro->setEnabled(true);
}

/*******************************************************************************
 * @brief   Focus position editing finished on edit line
 *
 * @param   void
 *
 * @return  void
 ******************************************************************************/
void MainWindow::on_lineEdit_FocusPosition_editingFinished()
{
    focus_position = ui->lineEdit_FocusPosition->text().toInt();
}

/*******************************************************************************
 * @brief   Direct clicked on radio button
 *
 * @param   void
 *
 * @return  void
 ******************************************************************************/
void MainWindow::on_radioButton_Direct_clicked()
{
    focus_mode  = FOCUS_MODE_DIRECT;
    QString str;
    str.sprintf("%d", focus_position);
    ui->lineEdit_FocusPosition->setText(str);
}

/*******************************************************************************
 * @brief   Infinity clicked on radio button
 *
 * @param   void
 *
 * @return  void
 ******************************************************************************/
void MainWindow::on_radioButton_Infinity_clicked()
{
    focus_mode  = FOCUS_MODE_INFINITY;
    ui->lineEdit_FocusPosition->setText("0");
}

/*******************************************************************************
 * @brief   Macro clicked on radio button
 *
 * @param   void
 *
 * @return  void
 ******************************************************************************/
void MainWindow::on_radioButton_Macro_clicked()
{
    focus_mode  = FOCUS_MODE_MACRO;
    ui->lineEdit_FocusPosition->setText("1023");
}

/*******************************************************************************
 * @brief   Focus apply clicked on radio button
 *
 * @param   void
 *
 * @return  void
 ******************************************************************************/
void MainWindow::on_pushButton_FocusApply_clicked()
{
    char command[256];

    sprintf(command, "focus %d",
            ui->lineEdit_FocusPosition->text().toInt());

    DemoControl(command);
}

/*******************************************************************************
 * @brief   OIS mode 0 clicked on radio button
 *
 * @param   void
 *
 * @return  void
 ******************************************************************************/
void MainWindow::on_radioButton_OIS_Mode0_clicked()
{
    ois_mode = 0;
}

/*******************************************************************************
 * @brief   OIS mode 1 clicked on radio button
 *
 * @param   void
 *
 * @return  void
 ******************************************************************************/
void MainWindow::on_radioButton_OIS_Mode1_clicked()
{
    ois_mode = 1;
}

/*******************************************************************************
 * @brief   OIS mode 2 clicked on radio button
 *
 * @param   void
 *
 * @return  void
 ******************************************************************************/
void MainWindow::on_radioButton_OIS_Mode2_clicked()
{
    ois_mode = 2;
}

/*******************************************************************************
 * @brief   OIS mode 3 clicked on radio button
 *
 * @param   void
 *
 * @return  void
 ******************************************************************************/
void MainWindow::on_radioButton_OIS_Mode3_clicked()
{
    ois_mode = 3;
}

/*******************************************************************************
 * @brief   OIS mode 4 clicked on radio button
 *
 * @param   void
 *
 * @return  void
 ******************************************************************************/
void MainWindow::on_radioButton_OIS_Mode4_clicked()
{
    ois_mode = 4;
}

/*******************************************************************************
 * @brief   OIS mode 5 clicked on radio button
 *
 * @param   void
 *
 * @return  void
 ******************************************************************************/
void MainWindow::on_radioButton_OIS_Mode5_clicked()
{
    ois_mode = 5;
}

/*******************************************************************************
 * @brief   OIS apply clicked on push button
 *
 * @param   void
 *
 * @return  void
 ******************************************************************************/
void MainWindow::on_pushButton_OIS_Apply_clicked()
{
    char command[256];
    sprintf(command, "ois %d", ois_mode);
    DemoControl(command);
}

/*******************************************************************************
 * @brief   LSC clicked on push button
 *
 * @param   void
 *
 * @return  void
 ******************************************************************************/
void MainWindow::on_checkBox_LSC_clicked(bool checked)
{
    char command[256];
    sprintf(command, "lsc %d", checked);
    DemoControl(command);
}

/*******************************************************************************
 * @brief   Still 12M Normal clicked on push button
 *
 * @param   void
 *
 * @return  void
 ******************************************************************************/
void MainWindow::on_pushButton_Still12M_NML_clicked()
{
    if (af_on)
    {
        af_on = 0;
        DemoControl((char*)"af 0");
        ui->pushButton_FocusApply->setEnabled(true);
        ui->pushButton_AF_On->setEnabled(true);
        ui->radioButton_Direct->setEnabled(true);
        ui->radioButton_Infinity->setEnabled(true);
        ui->radioButton_Macro->setEnabled(true);
    }

    StillCapture(0);
}

/*******************************************************************************
 * @brief   Still 12M HDR clicked on push button
 *
 * @param   void
 *
 * @return  void
 ******************************************************************************/
void MainWindow::on_pushButton_Still12M_HDR_clicked()
{
    if (af_on)
    {
        af_on = 0;
        DemoControl((char*)"af 0");
        ui->pushButton_FocusApply->setEnabled(true);
        ui->pushButton_AF_On->setEnabled(true);
        ui->radioButton_Direct->setEnabled(true);
        ui->radioButton_Infinity->setEnabled(true);
        ui->radioButton_Macro->setEnabled(true);
    }

    StillCapture(1);
}

/*******************************************************************************
 * @brief   Highspeed clicked on push button
 *
 * @param   void
 *
 * @return  void
 ******************************************************************************/
void MainWindow::on_pushButton_Highspeed_clicked()
{
    Highspeed();
}
