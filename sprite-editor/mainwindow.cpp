/**
 * Dana Ahn, Alberto del Rossario, Ethan Kai Falconer, Kevin Matsui, Aidan Naito
 * Assignment 6/7 : QT Sprite Editor
 * CS 3505
 * April 4, 2022
 */
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "toolbar.h"
#include "drawcommand.h"
#include <QPainter>
#include <QMouseEvent>
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    connect(ui->frameScrollBar,
            &QScrollBar::valueChanged,
            this,
            &MainWindow::changeCurrentFrame);

    connect(ui->newProjectButton,
            &QPushButton::clicked,
            this,
            &MainWindow::createNewProject);

    connect(&this->colorPicker,
            &QColorDialog::currentColorChanged,
            this,
            &MainWindow::colorChange);

    connect(ui->rotateButton, &QPushButton::clicked,
            this, &MainWindow::rotateGrid);

    connect(ui->flipButton, &QPushButton::clicked,
            this, &MainWindow::flipGrid);

    connect(ui->saveButton, &QPushButton::clicked,
            this, &MainWindow::saveProject);

    //frame label geometry
    model.init(16, 600, 270, 10, canvas);
    current = QColor(0, 0, 0);

    ui -> undoButton -> setIcon(QIcon(":/icons/undo.png"));
    ui -> redoButton -> setIcon(QIcon(":/icons/redo.png"));
    ui -> colorDialogButton -> setIcon(QIcon(":/icons/color.png"));
    ui -> Eraser -> setIcon(QIcon(":/icons/eraser.png"));
    ui -> rotateButton -> setIcon(QIcon(":/icons/rotation.png"));
    ui -> flipButton -> setIcon(QIcon(":/icons/flip.png"));
    ui -> shapeButton -> setIcon(QIcon(":/icons/geometric-shape.png"));
    ui -> addFrameBut -> setIcon(QIcon(":/icons/frame.png"));
    ui -> previewButton -> setIcon(QIcon(":/icons/preview.png"));
    ui -> paintBucketButton -> setIcon(QIcon(":/icons/paint-bucket.png"));
    ui -> gridCheckBox -> setIcon(QIcon(":/icons/grid.png"));
    ui -> newProjectButton -> setIcon(QIcon(":/icons/new-project.png"));

}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event) {
    if (canvas.canvasChanged()) {
        QMessageBox::StandardButton result;
        result = QMessageBox::question(this, "Warning", "You have unsaved changes. Would you like to save?",
                                       QMessageBox::Yes|QMessageBox::No);

        if (result == QMessageBox::Yes) {
            saveProject();
        }
    }

    event->accept();
}

void MainWindow::rotateGrid() {
    canvas.rotateGrid();

    model.addToUndo();
    model.bypassRedo();

    update();
}

void MainWindow::flipGrid() {
    canvas.flipHorizontally();

    model.addToUndo();
    model.bypassRedo();

    update();
}

void MainWindow::saveProject() {
    QString fileName = QFileDialog::getSaveFileName(this,
            tr("Save Project"), "",
            tr("Project Type (*.ssp);;All Files (*)"));

    model.saveFile(fileName);
}

void MainWindow::on_gridCheckBox_stateChanged(int arg1) {
    if(arg1 != 0) {
        gridChecked = true;
    }
    else {
        gridChecked = false;
    }
    update();
}

void MainWindow::createNewProject() {
    if (canvas.canvasChanged()) {
        saveClauseCatch();
    }
    // if paintBucket was previously clicked, cancel it
    paintBucket = false;
    ui -> paintBucketButton -> setEnabled(true);

    reset();
    int options = ui ->sizeOptions->currentIndex();
    size = model.gridSize(options);

    model.init(size, 600, 270, 10, canvas);
}

void MainWindow::reset() {
    changeCurrentFrame(0);
    ui->frameScrollBar->setMaximum(0);
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    if (event->type() == QEvent::MouseMove && pressed && !shapeChecked) {
        QPoint pixelPos = event -> pos();
        //^^^^^^need offset edit
        // if inside canvas
        if(canvas.boundsDetection(pixelPos.x()-270, pixelPos.y()-10) && !shapeChecked && !paintBucket){
            canvas.updateGrid(pixelPos, current);
            update();
        }
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event) {
    if(pressed){
        if(shapeChecked){
            bool isTriangle = ui -> shapeComboBox -> currentIndex() == 0;
            if(isTriangle) {
                shapeCmd -> drawShape(event -> pos(), "triangle", current);
            }
            else if(!isTriangle) {
                shapeCmd -> drawShape(event -> pos(), "rectangle", current);
            }
            update();
            shapeChecked = false;
        }

        model.addToUndo();
        model.bypassRedo();
    }

    pressed = false;
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    QPoint pixelPos = event -> pos();

    // if inside canvas // offset^^^^^^
    if(canvas.boundsDetection(pixelPos.x() - 270, pixelPos.y() - 10)){
        pressed = true;
        if(shapeChecked){
            shapeCmd = new DrawCommand(&canvas, pixelPos);
        }else if(paintBucket) {
            canvas.fillArea(pixelPos.x(), pixelPos.y(), current);
            paintBucket = false;
            ui -> paintBucketButton -> setEnabled(true);
        }else {
            canvas.updateGrid(pixelPos, current);
        }


        update();
    }
}

void MainWindow::paintEvent(QPaintEvent*) {
    QPainter painter(this);

    int xLoc = ui -> frameLabel -> x();
    int yLoc = ui -> frameLabel -> y();

    drawSprite(&painter, canvas.getGrid());
    //Put this in model/ or new private method
    if(gridChecked){
        int gridSize = (ui -> frameLabel ->width()) / 7;
        // for vertical lines
        for(int i =0; i < 7; i++) {
            xLoc += gridSize;
            painter.drawLine(xLoc, yLoc, xLoc, yLoc + (ui -> frameLabel ->height()));
        }

        // for horizontal lines
        xLoc = ui -> frameLabel -> x();
        for(int i = 0; i < 6; i++) {
            yLoc += gridSize;
            painter.drawLine(xLoc, yLoc, xLoc + (ui -> frameLabel -> width()), yLoc);
        }
    }
}

void MainWindow::drawSprite(QPainter *painter, QVector<QVector<QColor>>& grid) {
    for (int x = 0; x < grid.count(); x++) {
        for (int y = 0; y < grid[x].count(); y++) {
            QColor color = grid[x][y];
            int xCord = x, yCord = y;
            canvas.colRowsToAbsolutePos(xCord, yCord);
            int pixelSize = canvas.getPixelSize();
            QRect rectangle(xCord, yCord , pixelSize, pixelSize);
            painter->fillRect(rectangle, color);
        }
    }
}

void MainWindow::changeCurrentFrame(int frame) {
    canvas.changeFrame(frame);
    model.resetCurrent();

    QString text = QString("Current Frame: %1").arg(canvas.getCurrentFrame());
    ui->frameCounter->setText(text);

    update();
}

// opening the preview window on button click
void MainWindow::on_previewButton_clicked() {
    previewWindow.show();
    previewWindow.previewSetup(canvas.getFrames(), canvas, size);
    previewWindow.startDrawing();
}


void MainWindow::on_undoButton_clicked() {
    model.undoDraw();
    update();
}


void MainWindow::on_redoButton_clicked() {
    model.redoDraw();
    update();
}


void MainWindow::on_addFrameBut_clicked() {
    canvas.addFrame();
    model.addUndoStack();
    model.addRedoStack();
    // if paintBucket was previously clicked, cancel it
    paintBucket = false;
    ui -> paintBucketButton -> setEnabled(true);

    QScrollBar* bar = ui->frameScrollBar;
    int frame = canvas.getCurrentFrame();
    bar->setMaximum(frame);
    bar->setValue(frame);

    update();
}

void MainWindow::on_shapeButton_clicked() {
    shapeChecked = true;
    // if paintBucket was previously clicked, cancel it
    paintBucket = false;
    ui -> paintBucketButton -> setEnabled(true);

}


void MainWindow::on_paintBucketButton_clicked() {
    paintBucket = true;
    ui -> paintBucketButton->setEnabled(false);
}


void MainWindow::on_colorDialogButton_clicked() {
   colorPicker.setVisible(true);
}

void MainWindow::colorChange(QColor color) {
   int red = current.red();
   int green = current.green();
   int blue = current.blue();
   ui->previousColorButton->setStyleSheet( QString("QPushButton {background-color: rgb("+QString::number(red)+","+QString::number(green)+","+QString::number(blue)+");} QPushButton:pressed {background-color: rgb(255,150,150);}"));
   model.selectColor(color);
   current = color;
}



void MainWindow::on_previousColorButton_clicked() {
    current = model.previousColor;
    model.selectColor(model.previousColor);
    int red = model.previousColor.red();
    int green = model.previousColor.green();
    int blue = model.previousColor.blue();
    ui->previousColorButton->setStyleSheet( QString("QPushButton {background-color: rgb("+QString::number(red)+","+QString::number(green)+","+QString::number(blue)+");} QPushButton:pressed {background-color: rgb(255,150,150);}"));
}

void MainWindow::on_loadButton_clicked() {
    if (canvas.canvasChanged()) {
        saveClauseCatch();
    }
    // if paintBucket was previously clicked, cancel it
    paintBucket = false;
    ui -> paintBucketButton -> setEnabled(true);
    QString fileName = QFileDialog::getOpenFileName(this,
            tr("Open Sprite Project"), "",
            tr("Project Type (*.ssp);;All Files (*)"));

    if (!model.loadFile(fileName)) {
        QMessageBox::warning(this, tr("Load Failed"),
                             tr("Check if file is valid or file structure is correct"));
    }

    ui->frameScrollBar->setMaximum(canvas.getTotalFrame());
    ui->frameScrollBar->setValue(canvas.getCurrentFrame());

    update();
}

void MainWindow::saveClauseCatch() {
    QMessageBox::StandardButton result;
    result = QMessageBox::question(this, "Unsaved Changes", "Save Changes?",
                                   QMessageBox::Yes|QMessageBox::No);

    if (result == QMessageBox::Yes) {
        saveProject();
    }
}

void MainWindow::on_Eraser_clicked(bool checked) {
    if (checked) {
        eraserHold = current;
        current = QColor("white");
        // if paintBucket was previously clicked, cancel it
        paintBucket = false;
        ui -> paintBucketButton -> setEnabled(true);
    } else {
        current = eraserHold;
    }
}

