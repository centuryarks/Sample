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
    QString qstring;

    DemoInit();

    focus_mode  = FOCUS_MODE_DIRECT;
    ois_mode = 0;
    focus_position = 512;
    af_on = 0;
    shutter = GetShutter();
    gain = GetGain();

    metering_mode = 0;
    denoise_mode = 0;
    awb_mode = 0;
    sharpness = GetSharpness();
    contrast = GetContrast();
    brightness = GetBrightness();
    saturation = GetSaturation();
    ev_compensation = GetEvCompensation();
    red_gain = GetRedGain();
    blue_gain = GetBlueGain();

    ui->setupUi(this);
    ui->radioButton_MeteringMode_Centre->setChecked(true);
    ui->radioButton_Direct->setChecked(true);
    ui->radioButton_OIS_Mode0->setChecked(true);
    ui->radioButton_DenoiseMode_Auto->setChecked(true);
    ui->radioButton_AwbMode_Auto->setChecked(true);

	connect(ui->lineEdit_FocusPosition, SIGNAL(returnPressed()), this, SLOT(on_pushButton_FocusApply_clicked()));

    qstring.setNum(focus_position);
    ui->lineEdit_FocusPosition->setText(qstring);
    ui->horizontalSlider_FocusPosition->setValue(focus_position);

    ui->spinBox_Shutter->setValue(shutter);
    ui->doubleSpinBox_Gain->setValue(gain);

    ui->pushButton_FocusApply->hide();
    ui->pushButton_OIS_Apply->hide();

    ui->doubleSpinBox_Sharpness->setValue(sharpness);
    ui->label_Sharpness->setToolTip("The value zero means that no sharpening is applied, the value 1.0 uses the default amount of sharpening, and values greater than 1.0 use extra sharpening.");
    ui->doubleSpinBox_Sharpness->setToolTip("The value zero means that no sharpening is applied, the value 1.0 uses the default amount of sharpening, and values greater than 1.0 use extra sharpening.");

    ui->doubleSpinBox_Contrast->setValue(contrast);
    ui->label_Contrast->setToolTip("The value zero produces minimum contrast, the value 1.0 uses the default amount of contrast, and values greater than 1.0 apply extra contrast.");
    ui->doubleSpinBox_Contrast->setToolTip("The value zero produces minimum contrast, the value 1.0 uses the default amount of contrast, and values greater than 1.0 apply extra contrast.");

    ui->doubleSpinBox_Brightness->setValue(brightness);
    ui->label_Brightness->setToolTip("The value -1.0 produces an (almost) black image, the value 1.0 produces an almost entirely white image and the value 0.0 produces standard image brightness.");
    ui->doubleSpinBox_Brightness->setToolTip("The value -1.0 produces an (almost) black image, the value 1.0 produces an almost entirely white image and the value 0.0 produces standard image brightness.");

    ui->doubleSpinBox_Saturation->setValue(saturation);
    ui->label_Saturation->setToolTip("The value zero produces a greyscale image, the value 1.0 uses the default amount of sautration, and values greater than 1.0 apply extra colour saturation.");
    ui->doubleSpinBox_Saturation->setToolTip("The value zero produces a greyscale image, the value 1.0 uses the default amount of sautration, and values greater than 1.0 apply extra colour saturation.");

    ui->doubleSpinBox_EvCompensation->setValue(ev_compensation);
    ui->label_EvCompensation->setToolTip("Sets the EV compensation of the image in units of stops, in the range -10 to 10. Default is 0.");
    ui->doubleSpinBox_EvCompensation->setToolTip("Sets the EV compensation of the image in units of stops, in the range -10 to 10. Default is 0.");
    ui->label_EvCompensation->setEnabled(false);
    ui->doubleSpinBox_EvCompensation->setEnabled(false);

    ui->doubleSpinBox_ColourGain_Red->setValue(red_gain);
    ui->label_ColourGain_Red->setEnabled(false);
    ui->doubleSpinBox_ColourGain_Red->setEnabled(false);

    ui->doubleSpinBox_ColourGain_Blue->setValue(blue_gain);
    ui->label_ColourGain_Blue->setEnabled(false);
    ui->doubleSpinBox_ColourGain_Blue->setEnabled(false);

    this->setWindowTitle(VERSION);

    on_pushButton_FocusApply_clicked();

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
}

/*******************************************************************************
 * @brief   Update (Timer Event) For Focus Position
 *
 * @param   void
 *
 * @return  void
 ******************************************************************************/
void MainWindow::update()
{
    int value = GetFocusPosition();
    if (focus_position != value)
    {
        focus_position = value;
        QString qstring;
        qstring.setNum(focus_position);
        ui->lineEdit_FocusPosition->setText(qstring);
        ui->horizontalSlider_FocusPosition->setValue(focus_position);
    }
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
    ui->horizontalSlider_FocusPosition->setEnabled(false);

    DemoControl((char*)"af 1");
    af_on = 1;

    if (focus_mode != FOCUS_MODE_DIRECT)
    {
        focus_mode  = FOCUS_MODE_DIRECT;
        ui->radioButton_Direct->setChecked(true);
    }

    timer->start(10);
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
    ui->horizontalSlider_FocusPosition->setEnabled(true);

    timer->stop();

    int value = GetFocusPosition();
    if (focus_position != value)
    {
        focus_position = value;
        QString qstring;
        qstring.setNum(focus_position);
        ui->lineEdit_FocusPosition->setText(qstring);
        ui->horizontalSlider_FocusPosition->setValue(focus_position);
    }
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
    int focus_position = ui->lineEdit_FocusPosition->text().toInt();

    if (focus_position < ui->horizontalSlider_FocusPosition->minimum()) focus_position = ui->horizontalSlider_FocusPosition->minimum();
    if (focus_position > ui->horizontalSlider_FocusPosition->maximum()) focus_position = ui->horizontalSlider_FocusPosition->maximum();

    if (focus_position != ui->lineEdit_FocusPosition->text().toInt())
    {
        QString qstring;
        qstring.setNum(focus_position);
        ui->lineEdit_FocusPosition->setText(qstring);
    }
    ui->horizontalSlider_FocusPosition->setValue(focus_position);
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
    str.setNum(focus_position);
    ui->lineEdit_FocusPosition->setText(str);
    ui->horizontalSlider_FocusPosition->setValue(focus_position);
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

    int focus_pos = 0;
    QString str;
    str.setNum(focus_pos);
    ui->lineEdit_FocusPosition->setText(str);
    ui->horizontalSlider_FocusPosition->setValue(focus_pos);
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

    int focus_pos = 1023;
    QString str;
    str.setNum(focus_pos);
    ui->lineEdit_FocusPosition->setText(str);
    ui->horizontalSlider_FocusPosition->setValue(focus_pos);
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
    on_lineEdit_FocusPosition_editingFinished();
    
    char command[256];
    snprintf(command, 256, "focus %d",
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
    on_pushButton_OIS_Apply_clicked();
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
    on_pushButton_OIS_Apply_clicked();
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
    on_pushButton_OIS_Apply_clicked();
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
    on_pushButton_OIS_Apply_clicked();
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
    on_pushButton_OIS_Apply_clicked();
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
    on_pushButton_OIS_Apply_clicked();
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
    snprintf(command, 256, "ois %d", ois_mode);
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
    snprintf(command, 256, "lsc %d", checked);
    DemoControl(command);
}

/*******************************************************************************
 * @brief   Still clicked on push button
 *
 * @param   void
 *
 * @return  void
 ******************************************************************************/
void MainWindow::on_pushButton_Still_clicked()
{
    setWindowState(windowState() ^ Qt::WindowMinimized);
    show();
    QApplication::processEvents();

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

    Capture(0);

    setWindowState(windowState() ^ Qt::WindowMinimized);
    show();
}

/*******************************************************************************
 * @brief   Shutter/Gain apply clicked on push button
 *
 * @param   void
 *
 * @return  void
 ******************************************************************************/
void MainWindow::on_pushButton_ShutterGainApply_clicked()
{
    char command[256];
    snprintf(command, 256, "metering %d", metering_mode);
    DemoControl(command);
    snprintf(command, 256, "shutter/gain %d %2.1f",
        ui->spinBox_Shutter->value(),
        ui->doubleSpinBox_Gain->value());
    DemoControl(command);
}

/*******************************************************************************
 * @brief   Focus position slider value changed
 *
 * @param   value   New focus position value
 *
 * @return  void
 ******************************************************************************/
void MainWindow::on_horizontalSlider_FocusPosition_valueChanged(int value)
{
    if (focus_position != value)
    {
        if ((value != 0) && (value != 1023))
        {
            focus_mode  = FOCUS_MODE_DIRECT;
            ui->radioButton_Direct->setChecked(true);
        }
        QString qstring;
        qstring.setNum(value);
        ui->lineEdit_FocusPosition->setText(qstring);
        focus_position = value;
        on_pushButton_FocusApply_clicked();
    }
}

/*******************************************************************************
 * @brief   Shutter/Gain group checkbox clicked
 *
 * @param   checked   1: Manual / 0: Auto control
 *
 * @return  void
 ******************************************************************************/
void MainWindow::on_groupBox_ShutterGain_clicked(bool checked)
{
    char command[256];
    snprintf(command, 256, "aec/agc %d", !checked);
    DemoControl(command);

    ui->label_EvCompensation->setEnabled(!checked);
    ui->doubleSpinBox_EvCompensation->setEnabled(!checked);

    ui->groupBox_MeteringMode->setEnabled(!checked);
}

/*******************************************************************************
 * @brief   Raw image checkbox clicked
 *
 * @param   checked   1: Save raw file(DNG format)
 *
 * @return  void
 ******************************************************************************/
void MainWindow::on_checkBox_RawImage_clicked(bool checked)
{
    char command[256];
    snprintf(command, 256, "raw %d", checked);
    DemoControl(command);
}

/*******************************************************************************
 * @brief   Denoise mode clicked on radio button
 *
 * @param   void
 *
 * @return  void
 ******************************************************************************/
void MainWindow::on_radioButton_DenoiseMode_Auto_clicked()
{
    denoise_mode = 0;
}

void MainWindow::on_radioButton_DenoiseMode_Off_clicked()
{
    denoise_mode = 1;
}

void MainWindow::on_radioButton_DenoiseMode_CdnOff_clicked()
{
    denoise_mode = 2;
}

void MainWindow::on_radioButton_DenoiseMode_CdnFast_clicked()
{
    denoise_mode = 3;
}

void MainWindow::on_radioButton_DenoiseMode_CdnHq_clicked()
{
    denoise_mode = 4;
}

/*******************************************************************************
 * @brief   AWB mode clicked on radio button
 *
 * @param   void
 *
 * @return  void
 ******************************************************************************/
void MainWindow::on_radioButton_AwbMode_Auto_clicked()
{
    awb_mode = 0;
    awbModeFixed(false);
}

void MainWindow::on_radioButton_AwbMode_Incandescent_clicked()
{
    awb_mode = 1;
    awbModeFixed(false);
}

void MainWindow::on_radioButton_AwbMode_Tungsten_clicked()
{
    awb_mode = 2;
    awbModeFixed(false);
}

void MainWindow::on_radioButton_AwbMode_Fluorescent_clicked()
{
    awb_mode = 3;
    awbModeFixed(false);
}

void MainWindow::on_radioButton_AwbMode_Indoor_clicked()
{
    awb_mode = 4;
    awbModeFixed(false);
}

void MainWindow::on_radioButton_AwbMode_Daylight_clicked()
{
    awb_mode = 5;
    awbModeFixed(false);
}

void MainWindow::on_radioButton_AwbMode_Cloudy_clicked()
{
    awb_mode = 6;
    awbModeFixed(false);
}

void MainWindow::on_radioButton_AwbMode_Fixed_clicked()
{
    awb_mode = 7;
    awbModeFixed(true);
}

/*******************************************************************************
 * @brief   Camera Control apply clicked on push button
 *
 * @param   void
 *
 * @return  void
 ******************************************************************************/
void MainWindow::on_pushButton_CameraControlApply_clicked()
{
    char command[256];
    snprintf(command, 256, "cameracontrol %2.1f %2.1f %2.1f %2.1f %2.1f %d %d %2.1f %2.1f",
        ui->doubleSpinBox_Sharpness->value(),
        ui->doubleSpinBox_Contrast->value(),
        ui->doubleSpinBox_Brightness->value(),
        ui->doubleSpinBox_Saturation->value(),
        ui->doubleSpinBox_EvCompensation->value(),
        denoise_mode,
        awb_mode, ui->doubleSpinBox_ColourGain_Red->value(), ui->doubleSpinBox_ColourGain_Blue->value());
    DemoControl(command);
}

/*******************************************************************************
 * @brief   GUI control when AWB mode is fixed
 *
 * @param   fixed       true: fixed / false: auto mode
 *
 * @return  void
 ******************************************************************************/
void MainWindow::awbModeFixed(bool fixed)
{
    ui->label_ColourGain_Red->setEnabled(fixed);
    ui->doubleSpinBox_ColourGain_Red->setEnabled(fixed);

    ui->label_ColourGain_Blue->setEnabled(fixed);
    ui->doubleSpinBox_ColourGain_Blue->setEnabled(fixed);
}

/*******************************************************************************
 * @brief   Metering mode clicked on radio button
 *
 * @param   void
 *
 * @return  void
 ******************************************************************************/
void MainWindow::on_radioButton_MeteringMode_Centre_clicked()
{
    metering_mode = 0;
}

void MainWindow::on_radioButton_MeteringMode_Spot_clicked()
{
    metering_mode = 1;
}

void MainWindow::on_radioButton_MeteringMode_Average_clicked()
{
    metering_mode = 2;
}
