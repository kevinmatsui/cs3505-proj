/**
 * Dana Ahn, Alberto del Rossario, Ethan Kai Falconer, Kevin Matsui, Aidan Naito
 * Assignment 6/7 : QT Sprite Editor
 * CS 3505
 * April 4, 2022
 */
#ifndef DRAWCOMMAND_H
#define DRAWCOMMAND_H

#include <QUndoCommand>
#include <QPainter>
#include "canvas.h"


class DrawCommand : public QUndoCommand
{
public:
    DrawCommand(Canvas * _canvas);
    DrawCommand(Canvas * _canvas, QPoint shapeInitPos);

    void drawShape(QPoint shapePos, QString shapeStr, QColor color);

private:
    Canvas *canvas;
    QColor prevColor;
    QColor curColor;
    QPoint initShapePos;

    void paintPixel(int x, int y, QColor color);
    double getYCoord(double slope, int x, int b);

};

#endif // DRAWCOMMAND_H
