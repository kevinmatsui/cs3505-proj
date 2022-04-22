/**
 * @file mainwindow.cpp
 * @author Ee Jae Ahn, Kevin Matsui
 * @brief This is the source file for the mainwindow of the simon game which is the view, and displays the user interface for the game
 * @version 0.1
 * @date 2022-03-18
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Model.h"
#include <QMessageBox>
#include <QtGui>

/**
 * @brief MainWindow::MainWindow Constructor that sets up the connections for the signals and slots for the
 * model to communicate with the view
 * @param model
 * @param parent
 */
MainWindow::MainWindow(Model& model, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui -> setupUi(this);
    // connects startButton clicked signal to startGame slot
    connect(ui -> startButton,
            &QPushButton::clicked,
            &model,
            &Model::startGame);
    // connects Model's lightOn signal to lightOnButton slot
    connect(&model,
            &Model::lightOn,
            this,
            &MainWindow::lightOnButton);
    // connects Model's lightOff signal to lightOffButton slot
    connect(&model,
            &Model::lightOff,
            this,
            &MainWindow::lightOffButton);
    // connects Model's enableButtons signal to buttonsEnabled slot
    connect(&model,
            &Model::enableButtons,
            this,
            &MainWindow::buttonsEnabled);
    // connects Model's disableButtons signal to buttonsDisabled slot
    connect(&model,
            &Model::disableButtons,
            this,
            &MainWindow::buttonsDisabled);
    // connects Model's enableStartButton signal to startButtonEnabled slot
    connect(&model,
            &Model::enableStartButton,
            this,
            &MainWindow::startButtonEnabled);
    // connects Model's disableStartButton signal to startButtonDisabled slot
    connect(&model,
            &Model::disableStartButton,
            this,
            &MainWindow::startButtonDisabled);
    // connects redButton clicked signal to redClicked slot
    connect(ui -> redButton,
            &QPushButton::clicked,
            &model,
            &Model::redClicked);
    // connects blueButton clicked signal to blueClicked slot
    connect(ui -> blueButton,
            &QPushButton::clicked,
            &model,
            &Model::blueClicked);
    // connects Model's lightOn signal to lightOnButton slot
    connect(&model,
            &Model::updateProgressBar,
            ui->progressBar,
            &QProgressBar::setValue);
    // connects Model's lightOn signal to lightOnButton slot
    connect(&model,
            &Model::updateTextBox,
            ui->speedTextBox,
            &QTextEdit::setText);
    // connects Model's lightOn signal to lightOnButton slot
    connect(&model,
            &Model::warningMessage,
            this,
            &MainWindow::warningPopUp);
    // connects yesClicked clicked signal to continueGame slot
    connect(this,
            &MainWindow::yesClicked,
            &model,
            &Model::continueGame);
    // connects noCLicked clicked signal to endGame slot
    connect(this,
            &MainWindow::noClicked,
            &model,
            &Model::endGame);
    // connects endButton clicked signal to endGame slot
    connect(ui -> endButton,
            &QPushButton::clicked,
            &model,
            &Model::endGame);
    // connects speedUpButton clicked signal to speedUpGame slot
    connect(ui -> speedUpButton,
            &QPushButton::clicked,
            &model,
            &Model::speedUpGame);

    // set default speed text box text to 2 seconds
    ui -> speedTextBox -> setText("2.0 seconds");
    ui -> redButton -> setEnabled(false);
    ui -> blueButton -> setEnabled(false);

    // set the colors of the red and blue button
    ui->redButton->setStyleSheet( QString("QPushButton {background-color: rgb(200,50,50);} QPushButton:pressed {background-color: rgb(255,150,150);}"));
    ui->blueButton->setStyleSheet( QString("QPushButton {background-color: rgb(50,50,200);} QPushButton:pressed {background-color: rgb(150,150,255);}"));
}

/**
 * @brief MainWindow::~MainWindow
 */
MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * @brief MainWindow::lightOnButton Lights up the corresponding button
 * @param color
 */
void MainWindow::lightOnButton(QString color) {
    // change the color of the corresponding button to light it up
    if(color == "R") {
        ui -> redButton -> setStyleSheet(QString("background-color: rgb(255,150,150);"));
    } else if (color == "B") {
        ui -> blueButton -> setStyleSheet(QString("background-color: rgb(150,150,255);"));
    }
}

/**
 * @brief MainWindow::lightOffButton Turns off the corresponding buttons light up color
 * @param color
 */
void MainWindow::lightOffButton(QString color) {
    // change the color of the corresponding button to turn the light off
    if(color == "R") {
        ui -> redButton -> setStyleSheet(QString("background-color: rgb(200,50,50);"));
    } else if (color == "B") {
         ui -> blueButton -> setStyleSheet(QString("background-color: rgb(50,50,200);"));
    }

}

/**
 * @brief MainWindow::buttonsEnabled
 * Enables the red and blue buttons
 */
void MainWindow::buttonsEnabled() {
    ui -> redButton -> setEnabled(true);
    ui -> blueButton -> setEnabled(true);
}

/**
 * @brief MainWindow::buttonsDisabled
 * Disables the red and blue buttons
 */
void MainWindow::buttonsDisabled() {
    ui -> redButton -> setEnabled(false);
    ui -> blueButton -> setEnabled(false);
    ui -> speedUpButton -> setEnabled(false);
}

/**
 * @brief MainWindow::startButtonDisabled
 * Disables the start button
 */
void MainWindow::startButtonDisabled() {
    ui -> startButton -> setEnabled(false);
}

/**
 * @brief MainWindow::startButtonEnabled
 * Enables the start button and the speed up button
 */
void MainWindow::startButtonEnabled() {
     ui -> startButton -> setEnabled(true);
     ui -> speedUpButton -> setEnabled(true);
}

/**
 * @brief MainWindow::warningPopUp
 * Displays a warning message that says you lost the game, gives yes no option to continue the game
 */
void MainWindow::warningPopUp() {
    // display a warning message that says you lost the game
    QMessageBox::StandardButton reply;
    reply = QMessageBox::warning(this, "Simon Game", "You Lost the Game. Do you want to retry the current game stage?",
                                    QMessageBox::Yes|QMessageBox::No);
      // if the user wants to continue the game
      if (reply == QMessageBox::Yes) {
          emit yesClicked();
      }
      // if the user wants to restart the game
      else if(reply == QMessageBox::No) {
          emit noClicked();
          // enable the game speed button
          ui -> speedUpButton -> setEnabled(true);
      }
}



