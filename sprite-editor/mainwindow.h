/**
 * Dana Ahn, Alberto del Rossario, Ethan Kai Falconer, Kevin Matsui, Aidan Naito
 * Assignment 6/7 : QT Sprite Editor
 * CS 3505
 * April 4, 2022
 */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "toolbar.h"
#include "canvas.h"
#include "model.h"
#include <QColorDialog>
#include <previewform.h>
#include <QPoint>
#include "drawcommand.h"
//#include <QUndoCommand>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    int size;

signals:
    void undoDraw();

private slots:
    void on_gridCheckBox_stateChanged(int arg1);
    void colorChange(QColor color);
    void on_previewButton_clicked();
    void on_undoButton_clicked();
    void on_redoButton_clicked();
    void on_addFrameBut_clicked();
    void on_shapeButton_clicked();
    void on_paintBucketButton_clicked();
    void on_colorDialogButton_clicked();
    void on_previousColorButton_clicked();
    void on_loadButton_clicked();
    void on_Eraser_clicked(bool checked);

private:
    Ui::MainWindow *ui;
    Canvas canvas;
    Model model;
    QColorDialog colorPicker;
    QColor current, eraserHold;
    bool gridChecked = false;
    bool pressed = false;
    bool paintBucket = false;
    bool shapeChecked = false;
    DrawCommand *shapeCmd;

    void changeCurrentFrame(int frame);
    void reset();
    void createNewProject();
    void saveProject();
    void rotateGrid();
    void flipGrid();
    void saveClauseCatch();

    void closeEvent(QCloseEvent* event);
    void drawSprite(QPainter * painter, QVector<QVector<QColor>>& grid);

    PreviewForm previewWindow;

protected:
    void paintEvent(QPaintEvent *event);
};
#endif // MAINWINDOW_H
