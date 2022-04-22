/**
 * Dana Ahn, Alberto del Rossario, Ethan Kai Falconer, Kevin Matsui, Aidan Naito
 * Assignment 6/7 : QT Sprite Editor
 *
 * Handles special draw logic for making rectangles and triangles for the canvas
 *
 * CS 3505
 * April 4, 2022
 */
#include "drawcommand.h"
#include "canvas.h"
#include <QPainter>
#include <QRect>
#include <cmath>

DrawCommand::DrawCommand(Canvas *_canvas)
    : canvas(_canvas) {

}

DrawCommand::DrawCommand(Canvas *_canvas, QPoint _initShapePos) : canvas(_canvas), initShapePos(_initShapePos){
}

void DrawCommand::drawShape(QPoint shapePos, QString shapeStr, QColor color) {
    double smallX, bigX;
    int smallY, bigY;

    if(shapePos.x() < initShapePos.x()) {
        smallX = shapePos.x();
        bigX = initShapePos.x();
    }else {
        smallX = initShapePos.x();
        bigX = shapePos.x();
    }

    if(shapePos.y() < initShapePos.y()){
        smallY = shapePos.y();
        bigY = initShapePos.y();
    }else {
        smallY = initShapePos.y();
        bigY = shapePos.y();
    }
    if(shapeStr == "triangle") {
        // for vertical line
        for(int y = smallY; y < bigY; y++) {
            paintPixel(initShapePos.x(), y, color);
        }

        // for horizontal line
        for(int x = smallX; x < bigX; x++) {
            paintPixel(x, initShapePos.y(), color);
        }

        double xDiff = bigX - smallX;
        double yDiff = bigY - smallY;

        // for diagonal line
        if((shapePos.x() == bigX && shapePos.y() == smallY) || (shapePos.x() == smallX && shapePos.y() == bigY)) {
            for(int x =0; x < xDiff; x++){
                double y = getYCoord(yDiff/xDiff, x, 0);
                paintPixel(x + smallX, y + smallY, color);
            }
        } else {
            for(int x =0; x < xDiff; x++){
                double y = getYCoord(yDiff/xDiff, x, 0);
                paintPixel(bigX - x, y + smallY, color);
            }
        }
    }
    else if(shapeStr == "rectangle"){

        // horizontal lines
        for(int x = smallX; x < bigX; x++) {
            paintPixel(x, initShapePos.y(), color);
            paintPixel(x, shapePos.y(), color);
        }

        // vertical lines
        for(int y = smallY; y < bigY; y++) {
            paintPixel(initShapePos.x(), y, color);
            paintPixel(shapePos.x(), y, color);
        }
    }
}

double DrawCommand::getYCoord(double slope, int x, int b) {
    return slope*x + b;
}

void DrawCommand::paintPixel(int x, int y, QColor color) {
        QPoint pixel(x, y);

        canvas->updateGrid(pixel, color);
}
