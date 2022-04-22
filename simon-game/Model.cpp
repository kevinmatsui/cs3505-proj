/**
 * @file Model.cpp
 * @author Ee Jae Ahn, Kevin Matsui
 * @brief This is the source file for the model of the simon game which has all the detailed controls for the game
 * @version 0.1
 * @date 2022-03-18
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "Model.h"
#include <QRandomGenerator>
#include <QString>


/**
 * @brief Model::Model Constructor for the simon game model that initializes the speed of the game
 * @param parent
 */
Model::Model(QObject *parent) : QObject(parent) {
    lightUpSpeed = 2000;
}

/**
 * @brief Model::startGame Slot that starts game by initializing the list and calling the helper continueGame slot
 */
void Model::startGame(){
    // clear the list whenever game started
    colorList.clear();
    // add three random colors
    addColor();
    addColor();
    addColor();
    // add back the speed that was subtracted from addColor method
    lightUpSpeed += 300;

    // continue and start the game
    continueGame();
}

/**
 * @brief Model::continueGame Slot that continues the game by initializing the beginning set up for the game and starting the light up for the game
 */
void Model::continueGame() {
    // display the current speed
    QString speedString = QString::number(lightUpSpeed/1000.0) + " seconds";
    emit updateTextBox(speedString); //update text for the speed

    // fire disabling buttons event
    emit disableStartButton();
    emit disableButtons();

    // initialize index counter for the colorList
    index=0;
    userIndex = 0;

    // initialize the progress bar to 0
    emit updateProgressBar(0);

    // start lighting up after the seconds from lightUpSpeed ms
    QTimer::singleShot(lightUpSpeed, this, &Model::displayColor);
    // start lighting off after the seconds from lightUpSpeed * 0.5
    QTimer::singleShot(lightUpSpeed*1.5, this, &Model::removeColor);
}

/**
 * @brief Model::displayColor Slot that display the light up color for the game by following the colorList
 */
void Model::displayColor() {
    // before all the elements from colorList checked
    if(index<colorList.size()){
        // change the color to light up buttons by the list index
        color = colorList[index];
        // fire light on event
        emit lightOn(color);
        index++;
        // call the current slot again after lightUpSpeed ms
        QTimer::singleShot(lightUpSpeed, this, &Model::displayColor);
    }
}

/**
 * @brief Model::removeColor Slot that removes the color light up for the game by following the colorList
 */
void Model::removeColor() {
    if(index < colorList.size()){
        // fire light off event
        emit lightOff(color);
        // call the current slot again after lightUpSpeed ms
        QTimer::singleShot(lightUpSpeed, this, &Model::removeColor);
    }
    // if checking the last element
    else if(index == colorList.size()){
        // light off the last color and enable the buttons
        emit lightOff(color);
        emit enableButtons();
    }

}
/**
 * @brief Model::addColor Private method that adds the color for the next game when the user won the current game
 */
void Model::addColor(){
    speedUpGame();
    // variable for random double number between 0 to 2
    double random = QRandomGenerator::global()->bounded(2);
    // append the color string for the list by random numbers
    if(random < 0.95) {
        colorList.append("R");
    }
    else {
        colorList.append("B");
    }
}

/**
 * @brief Model::redClicked Slot that responds when the user have pressed the red button
 */
void Model::redClicked() {
    // if user clicked the right button (Red)
    if(colorList[userIndex] == "R") {
        userIndex++;
        // update progress bar by corresponding progress percentage
        int percentage = userIndex/(double)colorList.size() * 100;
        emit updateProgressBar(percentage);
        // if the last element(Red) is clicked by the user
        if(userIndex == colorList.size()) {
            addColor(); // add color to the list
            // continue to the next game after the delay
            QTimer::singleShot(700, this, &Model::continueGame);
        }
    }
    // if clicked wrong color
    else {
        emit enableStartButton();
        emit warningMessage(); // warning message pop up
    }
}

/**
 * @brief Model::blueClicked Slot that responds when the user have pressed the blue button
 */
void Model::blueClicked() {
     // if user clicked the right button (Blue)
    if(colorList[userIndex] == "B") {
        userIndex++;
        int percentage = userIndex/(double)colorList.size() * 100;
        emit updateProgressBar(percentage);
        // if last element(Blue) is clicked by the user
        if(userIndex == colorList.size()) {
            addColor();
            QTimer::singleShot(700, this, &Model::continueGame);
        }
    }
    // if clicked wrong color
    else {
        emit enableStartButton();
        emit warningMessage();
    }
}

/**
 * @brief Model::endGame Slot that ends the game and reset the game
 */
void Model::endGame(){
    colorList.clear();
    // turn light off the red and blue buttons
    emit lightOff("R");
    emit lightOff("B");
    // disable red, blue, and speed up buttons
    emit disableButtons();
    emit enableStartButton();
    // initialize progress bar and the text
    emit updateProgressBar(0);
    emit updateTextBox("2 seconds");
    // update index counter to large number for game to end
    index = 100;
    // initialize the game speed
    lightUpSpeed = 2000;
}

/**
 * @brief Model::speedUpGame Slot that speeds up the game by user clicking the speed up button
 */
void Model::speedUpGame() {
    // before minium speed have reached
    if(lightUpSpeed > 100) {
        lightUpSpeed -= 100; // speed up the game
        // display the current speed in seconds
        QString speedString = QString::number(lightUpSpeed/1000.0) + " seconds";
        emit updateTextBox(speedString); //update text for the speed
    }
}


