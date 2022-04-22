/**
 * @file mainwindow.cpp
 * @author Ee Jae Ahn, Kevin Matsui
 * @brief This is the header file for the model of the simon game which has all the detailed controls for the game
 * @version 0.1
 * @date 2022-03-18
 *
 * @copyright Copyright (c) 2022
 */

#ifndef MODEL_H
#define MODEL_H
#include <QObject>
#include <QList>
#include <QTimer>

class Model: public QObject
{
    Q_OBJECT

public:
    explicit Model(QObject *parent = nullptr);


signals:
    void lightOn(QString color);
    void lightOff(QString color);
    void enableButtons();
    void enableStartButton();
    void disableStartButton();
    void disableButtons();
    void updateProgressBar(int percentage);
    void warningMessage();
    void updateTextBox(QString speed);

public slots:
    void startGame();
    void displayColor();
    void removeColor();
    void redClicked();
    void blueClicked();
    void continueGame();
    void endGame();
    void speedUpGame();

private:
    QList<QString> colorList;
    QString color;
    void addColor();
    void gameEnded();
    int index;
    int userIndex;
    int lightUpSpeed;

};
#endif // MODEL_H
