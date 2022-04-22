/**
 * Dana Ahn, Alberto del Rossario, Ethan Kai Falconer, Kevin Matsui, Aidan Naito
 * Assignment 6/7 : QT Sprite Editor
 * CS 3505
 * April 4, 2022
 */
#include "model.h"
#include <QMouseEvent>
#include <QStack>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QTextStream>

/**
 * @brief Model::Model constructor for the Model class
 */
Model::Model()
{
    currentColor = QColor(0, 0, 0);
    previousColor = QColor(0, 0, 0);
}

/**
 * @brief Model::init setting up initial variables for the Model class
 * @param gridSize - amount of columns for the canvas object
 * @param canvasSize - the area in which a user can actively draw
 * @param offsetX - sets the correct position of the canvas onto the whole window
 * @param offsetY - sets the correct position of the canvas onto the whole window
 * @param _canvas - reference of Canvas object
 */
void Model::init(int gridSize, int canvasSize, int offsetX, int offsetY, Canvas& _canvas) {
    PIXELSIZE = canvasSize / gridSize;
    actualSize = gridSize;
    canvas = &_canvas;
    canvas->initialize(canvasSize, canvasSize, offsetX, offsetY, PIXELSIZE);
    currentGrid = canvas->getGrid();  

    addUndoStack();
    addRedoStack();
}

void Model::addUndoStack() {
    QStack<QVector<QVector<QColor>>> undoStack;
    undoStacks.push_back(undoStack);
}

void Model::addRedoStack() {
    QStack<QVector<QVector<QColor>>> undoStack;
    redoStacks.push_back(undoStack);
}

/**
 * @brief Model::addToUndo pushes the previous grid to the stack and updates to the current state
 */
void Model::addToUndo() {
    int frameNum = canvas -> getCurrentFrame();
    QStack<QVector<QVector<QColor>>>& undoStack = undoStacks[frameNum];

    if (undoStack.count() > 20) {
        undoStack.pop_back();
    }
    undoStack.push(currentGrid);
    currentGrid = canvas->getGrid();
}

/**
 * @brief Model::bypassRedo clear the redo stack if a change is added after undoing
 */
void Model::bypassRedo() {
    int frameNum = canvas -> getCurrentFrame();
    QStack<QVector<QVector<QColor>>>& redoStack = redoStacks[frameNum];
    redoStack.clear();
}

/**
 * @brief Model::redoDraw retrieves the previous redo grid and updates it to the current grid
 */
void Model::redoDraw() {
    int frameNum = canvas -> getCurrentFrame();
     QStack<QVector<QVector<QColor>>>& undoStack = undoStacks[frameNum];
     QStack<QVector<QVector<QColor>>>& redoStack = redoStacks[frameNum];
    if (!redoStack.isEmpty()) {
        if (undoStack.count() > 20) {
            undoStack.pop_back();
        }
        undoStack.push(currentGrid);
        currentGrid = redoStack.pop();
        canvas->setGrid(currentGrid);
    }
}

/**
 * @brief Model::undoDraw retrieves the previous undo grid and updates it to the current grid
 */
void Model::undoDraw() {
    int frameNum = canvas -> getCurrentFrame();
    QStack<QVector<QVector<QColor>>>& undoStack = undoStacks[frameNum];
    QStack<QVector<QVector<QColor>>>& redoStack = redoStacks[frameNum];


    if (!undoStack.isEmpty()) {
        if (redoStack.count() > 20) {
            redoStack.pop_back();
        }
        redoStack.push(currentGrid);
        currentGrid = undoStack.pop();
        canvas->setGrid(currentGrid);
    }
}

void Model::resetCurrent() {
    currentGrid = canvas -> getGrid();
}

void Model::resetUndoRedoStacks() {
    undoStacks.clear();
    redoStacks.clear();
}


/**
 * @brief Model::gridSize changes the size of the grid and pixels based on the users selection
 * @param options - options from the spin box of the project size
 * @return - int to be sent to the view to update the canvas
 */
int Model::gridSize(int options) {
    int size = 0;
    switch (options) {
    case 0:
        size = 8;
        break;
    case 1:
        size = 16;
        break;
    case 2:
        size = 32;
        break;
    case 3:
        size = 64;
        break;
    }
    return size;
}

/**
 * @brief Model::selectColor updates and tracks the current and previous QColor used
 * @param newColor - the current color the user has selected
 */
void Model::selectColor(QColor newColor) {
    previousColor = currentColor;
    currentColor = newColor;
}

/**
 * @brief Model::saveFile Saving sprites using json and .ssp
 * @param fileName - the name of the file to be saved under
 */
void Model::saveFile(QString fileName) {
    if (fileName.isEmpty())
        return;

    int totalFrame = canvas->getTotalFrame();
    QVector<QVector<QVector<QColor>>> frames = canvas->getFrames();
    QJsonObject project;

    project.insert("height", actualSize);
    project.insert("width", actualSize);
    project.insert("numberOfFrames", totalFrame);

    QJsonObject frame;
    for (int n = 0; n < totalFrame; n ++) {
        QJsonObject rows;
        for (int y = 0 ; y < actualSize; y++) {
            QJsonArray cols;
            for (int x = 0; x < actualSize; x++) {
                QColor color = frames[n][x][y];
                QJsonArray colVals{color.red(), color.green(), color.blue(), color.alpha()};
                cols.push_back(colVals);
            }
            rows.insert(QString("row%1").arg(y), cols);
        }
        frame.insert(QString("frame%1").arg(n), rows);
    }

    project.insert("frames", frame);
    QJsonDocument document;
    document.setObject(project);
    QFile file(fileName);
    if (file.open(QFile::WriteOnly)) {
        file.write(document.toJson());
        file.close();
    }
    canvas->resetCanvasChange();
}

/**
 * @brief Model::loadFile method to load .ssp files into our sprite editor
 * @param file_path name of the file to be loaded
 * @return true if loaded correctly, false otherwise
 */
bool Model::loadFile(QString file_path){
   QFile file_obj(file_path);

   if (file_obj.open(QIODevice::ReadOnly)) {
       QByteArray bytes = file_obj.readAll();
       file_obj.close();

       QJsonParseError jsonError;
       QJsonDocument document = QJsonDocument::fromJson(bytes, &jsonError);
       if (jsonError.error != QJsonParseError::NoError) {
           return false;
       }

       if (document.isObject()) {
           try{
               QJsonObject docObj = document.object();

               int gridX, gridY, numberOfFrames;
               gridX = docObj.value("width").toInt();
               gridY = docObj.value("height").toInt();
               numberOfFrames = docObj.value("numberOfFrames").toInt();

               actualSize = gridX;
               resetUndoRedoStacks();
               canvas->initializeFromJson(gridX, gridY, numberOfFrames);

               QJsonObject frameObj = docObj.value("frames").toObject();
               for (int n = 0; n < numberOfFrames; n++) {
                   QVector<QVector<QColor>> grid;
                   grid.resize(gridX, QVector<QColor>(gridY));

                   QString frameNum = QString("frame%1").arg(n);
                   QJsonObject curFrame = frameObj[frameNum].toObject();
                   for (int y = 0; y < gridY; y++) {
                       QString rowNum = QString("row%1").arg(y);
                       QJsonArray row = curFrame.take(rowNum).toArray();
                       for (int x = 0; x < gridX; x++) {
                           QJsonArray rgba = row[x].toArray();

                           QColor pix(rgba[0].toInt(), rgba[1].toInt(), rgba[2].toInt(), rgba[3].toInt());
                           grid[x][y] = pix;
                       }
                   }
                   canvas->setGrid(grid, n);
                   addUndoStack();
                   addRedoStack();
               }
               currentGrid = canvas->getGrid();
               return true;
           } catch (...) {
               return false;
           }
       }
   }
   return false;
}

