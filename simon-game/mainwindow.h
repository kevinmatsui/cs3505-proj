/**
 * @file mainwindow.cpp
 * @author Ee Jae Ahn, Kevin Matsui
 * @brief This is the header file for the mainwindow of the simon game which is the view, and displays the user interface for the game
 * @version 0.1
 * @date 2022-03-18
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Model.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(Model& model, QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void yesClicked();
    void noClicked();

private slots:
    void lightOnButton(QString color);
    void lightOffButton(QString color);
    void buttonsEnabled();
    void startButtonEnabled();
    void buttonsDisabled();
    void warningPopUp();
    void startButtonDisabled();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
