/**
* A mainwindow class. Retrieves table data and dynamically updates the bar graph according to
* user selections.
* @author Jennifer and Alex
*/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qcustomplot.h"
#include <QVector>
#include <iostream>
#include <fstream>
#include <QDebug>
#include <QDir>
#include <QMessageBox>
#include <sstream>
#include <cmath>
#include <QFileDialog>

using std::min_element;
using std::max_element;

/**
 * @brief A constructor: Instantiates a ui and displays all default graph data.
 * @param parent
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {

    ui->setupUi(this);
    this->setWindowTitle("OMBI");

    // Create WelcomeScreen and connect to MainWindow
    _welcomeScreen = new WelcomeScreen();
    connect(_welcomeScreen->getStartButton(), SIGNAL(clicked()), this, SLOT(showMainScreen()));

    // Create MainScreen and connect to MainWindow
    _mainScreen = new MainScreen();
    _mainScreen->getServiceSelector()->addItems(_data.getServiceAreaNames().toList());

    // Connect Service Icon Buttons.
    connect(_mainScreen->getFireButton(), SIGNAL(clicked()), this, SLOT(userSelectFire()));
    connect(_mainScreen->getLibraryButton(), SIGNAL(clicked()), this, SLOT(userSelectLibrary()));
    connect(_mainScreen->getParksButton(), SIGNAL(clicked()), this, SLOT(userSelectParks()));
    connect(_mainScreen->getPoliceButton(), SIGNAL(clicked()), this, SLOT(userSelectPolice()));
    connect(_mainScreen->getWasteButton(), SIGNAL(clicked()), this, SLOT(userSelectWaste()));
    connect(_mainScreen->getWaterButton(), SIGNAL(clicked()), this, SLOT(userSelectWater()));
    connect(ui->mainMenu, SIGNAL(clicked()), this, SLOT(backToMainMenu()));
    // Connect Radio Buttons.
    connect (_mainScreen->getTableRadio(), SIGNAL(toggled(bool)), this, SLOT(changeChartNum()));
    connect (_mainScreen->getBarRadio(), SIGNAL(toggled(bool)), this, SLOT(changeChartNum()));
    connect (_mainScreen->getLineRadio(), SIGNAL(toggled(bool)), this, SLOT(changeChartNum()));
    // Connect Display Button.
    connect(_mainScreen->getDisplayButton(), SIGNAL(clicked()), this, SLOT(userSelectArea()));

    //Allows multiple municipalities to be selected at once
    ui->muniList->setSelectionMode(QAbstractItemView::MultiSelection);

    _serviceMeasures = ui->serviceMeasures;
    connect(ui->serviceMeasures, SIGNAL(activated(int)), this, SLOT(measureChanged()));

    // retrieve all municipalities and years from the data class
    QDir tempDir;
    std::string tempStr = tempDir.absolutePath().toUtf8().constData();
    tempStr = tempStr.substr(0, tempStr.length() - 24) + "src/userData.txt";
    std::ifstream userFile;                                     //Creates input file stream to read in user data
    QVector<QString> tempMunis;
    QVector<int> tempYears;
    userFile.open(tempStr.c_str());                                   //Reads in user data
    if (userFile.fail())                                              //Reads in default values if file cannot be opened
    {
        _welcomeScreen->show();
        _mainScreen->getTableRadio()->setChecked(true);
    }
    else {
        this->show();
        getline(userFile, tempStr, '\n');
        std::stringstream serviceLine(tempStr);
        std::getline(serviceLine, tempStr, ',');
        _selectedServiceArea = atoi(tempStr.c_str());
        std::getline(serviceLine, tempStr, ',');
        _selectedServiceMeasure = QString::fromStdString(tempStr);
        std::getline(serviceLine, tempStr, ',');
        _chartNum = atoi(tempStr.c_str());
        
        if (_chartNum == 1)
            _mainScreen->getBarRadio()->setChecked(true);
        else if (_chartNum == 2)
            _mainScreen->getLineRadio()->setChecked(true);
        else
            _mainScreen->getTableRadio()->setChecked(true);

        QVector<QString> measures = ServiceArea(_mainScreen->getServiceSelector()->itemText(_selectedServiceArea)).getServiceMeasuresIDs();
        for(int i = 0; i < measures.size(); ++i){
            _serviceMeasures->addItem(measures.at(i), QVariant(i));
            tempStr = _selectedServiceMeasure.toUtf8().constData();
            if(tempStr.compare(measures.at(i).toUtf8().constData()) == 0)
                _serviceMeasures->setCurrentIndex(i);
        }

        _selectedMunis = _data.getServiceAreas().at(_selectedServiceArea).getServiceMeasure(_selectedServiceMeasure).getMunicipalities();
        _selectedYears = _data.getServiceAreas().at(_selectedServiceArea).getServiceMeasure(_selectedServiceMeasure).getYears();
        getline(userFile, tempStr, '\n');
        std::stringstream muniLine(tempStr);
        while (muniLine.good()){
            std::getline(muniLine, tempStr, ',');
            if (tempStr.compare("") != 0)
            tempMunis.push_back(QString::fromStdString(tempStr));
        }
        getline(userFile, tempStr, '\n');
        std::stringstream yearLine(tempStr);
        while (yearLine.good()){
            std::getline(yearLine, tempStr, ',');
            if (tempStr.compare("") != 0)
            tempYears.push_back(atoi(tempStr.c_str()));
        }
        getline(userFile, tempStr, '\n');
        if (tempStr.compare("1") == 0) ui->meanLineButton->setChecked(true);
        else ui->meanLineButton->setChecked(false);
        userFile.close();

        // sorts years to chronological format
        std::sort(_selectedYears.begin(), _selectedYears.end());
        std::sort(tempYears.begin(), tempYears.end());

        for (int i = 0; i < _selectedMunis.size(); ++i){
            ui->muniList->addItem(_selectedMunis.at(i));
            if (tempMunis.contains(_selectedMunis.at(i))) ui->muniList->item(i)->setSelected(true);
        }

        if (tempYears.contains(_selectedYears.at(0))) ui->y2008Button->setChecked(true);
        else ui->y2008Button->setChecked(false);
        if (tempYears.contains(_selectedYears.at(1))) ui->y2009Button->setChecked(true);
        else ui->y2009Button->setChecked(false);
        if (tempYears.contains(_selectedYears.at(2))) ui->y2010Button->setChecked(true);
        else ui->y2010Button->setChecked(false);
        if (tempYears.contains(_selectedYears.at(3))) ui->y2011Button->setChecked(true);
        else ui->y2011Button->setChecked(false);
        if (tempYears.contains(_selectedYears.at(4))) ui->y2012Button->setChecked(true);
        else ui->y2012Button->setChecked(false);

        _selectedMunis = tempMunis;
        _selectedYears = tempYears;

        if (_chartNum == 1) startBarChart();
        else if (_chartNum == 2) startLineGraph();
        else startTable();
    }
    // bold specified labels
    ui->muniLabel->setText("<b>" + ui->muniLabel->text() + "</b>");
    ui->yearLabel->setText("<b>" + ui->yearLabel->text() + "</b>");
    ui->statsLabel->setText("<b>" + ui->statsLabel->text() + "</b>");

    // when the current filter changes, go to the appropriate handling method to update the visualization
    connect(ui->muniList, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(muniButtonPressed()));
    connect(ui->y2008Button,SIGNAL(stateChanged(int)),this,SLOT(yearButtonPressed()));
    connect(ui->y2009Button,SIGNAL(stateChanged(int)),this,SLOT(yearButtonPressed()));
    connect(ui->y2010Button,SIGNAL(stateChanged(int)),this,SLOT(yearButtonPressed()));
    connect(ui->y2011Button,SIGNAL(stateChanged(int)),this,SLOT(yearButtonPressed()));
    connect(ui->y2012Button,SIGNAL(stateChanged(int)),this,SLOT(yearButtonPressed()));
}

/**
* @brief A destructor.
* Deletes ui.
*/
MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * @brief a private method: Initializes all information to create a table
 */
void MainWindow::startTable(){
    // only show ui elements related to the table
    ui->tableWidget->setVisible(true);
    ui->tableLabel->setVisible(true);
    ui->meanButton->setVisible(true);
    ui->tableStats->setVisible(true);
    ui->meanLineButton->setVisible(false);
    ui->meanLineLabel->setVisible(false);
    ui->legendButton->setVisible(false);
    ui->chartWidget->setVisible(false);
    ui->saveButton->setVisible(false);

    ui->y2008Label->setText("<font color='black'>" + ui->y2008Label->text() + "</font>");
    ui->y2009Label->setText("<font color='black'>" + ui->y2009Label->text() + "</font>");
    ui->y2010Label->setText("<font color='black'>" + ui->y2010Label->text() + "</font>");
    ui->y2011Label->setText("<font color='black'>" + ui->y2011Label->text() + "</font>");
    ui->y2012Label->setText("<font color='black'>" + ui->y2012Label->text() + "</font>");

    ui->selectedCellsLabel->setText("<b>" + ui->selectedCellsLabel->text() + "</b>");

    tableReplot();
    tableReplot();

    // when a button's clicked, go to the appropriate handling method to execute the functionality for that button
    connect(ui->saveCSVButton,SIGNAL(clicked()),this,SLOT(toCSVFile()));
    connect(ui->meanButton,SIGNAL(clicked()),this,SLOT(meanButtonPressed()));
    connect(ui->minButton,SIGNAL(clicked()),this,SLOT(minButtonPressed()));
    connect(ui->maxButton,SIGNAL(clicked()),this,SLOT(maxButtonPressed()));
}

/**
 * @brief a private method: Initializes all information to create a bar chart
 */
void MainWindow::startBarChart(){
    ui->saveButton->setVisible(true);
    ui->chartWidget->setVisible(true);
    ui->meanLineButton->setVisible(true);
    ui->meanLineLabel->setVisible(true);
    ui->legendButton->setVisible(false);
    ui->tableStats->setVisible(false);
    ui->meanButton->setVisible(false);
    ui->meanValueLabel->setVisible(false);
    ui->saveCSVButton->setVisible(true);
    ui->saveButton->setText(QString::fromStdString("Save as Image"));
    ui->saveCSVButton->setText(QString::fromStdString("Save as CSV"));

    ui->tableWidget->setVisible(false);
    ui->tableLabel->setVisible(false);
    // set year label colours
    ui->y2008Label->setText("<font color='orange'>" + ui->y2008Label->text() + "</font>");
    ui->y2009Label->setText("<font color='blue'>" + ui->y2009Label->text() + "</font>");
    ui->y2010Label->setText("<font color='darkred'>" + ui->y2010Label->text() + "</font>");
    ui->y2011Label->setText("<font color='green'>" + ui->y2011Label->text() + "</font>");
    ui->y2012Label->setText("<font color='darkmagenta'>" + ui->y2012Label->text() + "</font>");

    ui->meanLineLabel->setText("<b><font size = 4>" + ui->meanLineLabel->text() + "</font></b>");

    for (int i = 0; i < _barChart.size(); ++i){
        if (ui->chartWidget->hasPlottable(_barChart.at(i))) ui->chartWidget->removePlottable(_barChart.at(i));
    }
    _barChart.clear();
    // create QCPBar object for each year
    for (int i = 0; i < _data.getServiceAreas().at(_selectedServiceArea).getServiceMeasure(_selectedServiceMeasure).getYears().size(); ++i){
        _barChart.push_back(new QCPBars(ui->chartWidget->xAxis, ui->chartWidget->yAxis));
        if (!(ui->chartWidget->hasPlottable(_barChart.at(i)))) ui->chartWidget->addPlottable(_barChart.at(i));
    }

    // set the fill colour and outline colour for all the bars in each QCPBar object
    QPen pen;
    pen.setWidthF(1.2);
    pen.setColor(QColor(255, 0, 230));
    _barChart.at(0)->setPen(pen);
    _barChart.at(0)->setBrush(QColor(255, 0, 230, 50));
    pen.setColor(QColor(0, 190, 0));
    _barChart.at(1)->setPen(pen);
    _barChart.at(1)->setBrush(QColor(0, 190, 0, 50));
    pen.setColor(QColor(190, 0, 0));
    _barChart.at(2)->setPen(pen);
    _barChart.at(2)->setBrush(QColor(190, 0, 0, 50));
    pen.setColor(QColor(0, 0, 255));
    _barChart.at(3)->setPen(pen);
    _barChart.at(3)->setBrush(QColor(0, 0, 255, 50));
    pen.setColor(QColor(255, 190, 0));
    _barChart.at(4)->setPen(pen);
    _barChart.at(4)->setBrush(QColor(255, 190, 0, 50));

    // set xAxis label to be bold
    QFont font("Cantarell", 11, QFont::Bold);
    ui->chartWidget->xAxis->setLabel("Municipalities");
    ui->chartWidget->xAxis->setLabelFont(font);

    // modify properties of the bar chart
    barChartReplot();

    connect(ui->saveButton,SIGNAL(clicked()),this,SLOT(imageButtonPressed()));
    connect(ui->saveCSVButton, SIGNAL(clicked()),this,SLOT(toCSVFile()));
    connect(ui->meanLineButton,SIGNAL(clicked()),this,SLOT(toggleMeanLine()));

}

/**
   * @brief a private method: Initializes all information to create a line graph
   */
void MainWindow::startLineGraph(){
    // only show ui elements related to the table
    ui->meanLineButton->setVisible(true);
    ui->meanLineLabel->setVisible(true);
    ui->legendButton->setVisible(true);
    ui->saveButton->setVisible(true);
    ui->chartWidget->setVisible(true);
    ui->saveCSVButton->setVisible(true);
    ui->tableStats->setVisible(false);
    ui->tableWidget->setVisible(false);
    ui->tableLabel->setVisible(false);
    ui->meanButton->setVisible(false);
    ui->meanValueLabel->setVisible(false);
    ui->saveButton->setText(QString::fromStdString("Save as Image"));
    ui->saveCSVButton->setText(QString::fromStdString("Save as CSV"));

    ui->y2008Label->setText("<font color='black'>" + ui->y2008Label->text() + "</font>");
    ui->y2009Label->setText("<font color='black'>" + ui->y2009Label->text() + "</font>");
    ui->y2010Label->setText("<font color='black'>" + ui->y2010Label->text() + "</font>");
    ui->y2011Label->setText("<font color='black'>" + ui->y2011Label->text() + "</font>");
    ui->y2012Label->setText("<font color='black'>" + ui->y2012Label->text() + "</font>");

    // bold label for mean line
    ui->meanLineLabel->setText("<b>" + ui->meanLineLabel->text() + "</b>");

    // set xAxis label to be bold
    QFont font("Cantarell", 11, QFont::Bold);
    ui->chartWidget->xAxis->setLabel("Years");
    ui->chartWidget->xAxis->setLabelFont(font);

    // modify properties of the bar chart
    lineGraphReplot();

    // when a button's clicked, go to the appropriate handling method to execute the functionality for that button
    connect(ui->saveButton,SIGNAL(clicked()),this,SLOT(imageButtonPressed()));
    connect(ui->saveCSVButton, SIGNAL(clicked()),this,SLOT(toCSVFile()));
    connect(ui->meanLineButton,SIGNAL(clicked()),this,SLOT(toggleMeanLine()));
    connect(ui->legendButton,SIGNAL(clicked()),this,SLOT(toggleLegend()));
}

/**
   * @brief: a private member that replots the table
   * Uses all currently selected filters of municipalities and years,
   * replots the table and shows it on the screen.
   */
void MainWindow::tableReplot(){
    // add title layout element
    ui->tableLabel->setText("<b><font size = 4>" + _data.getServiceAreas().at(_selectedServiceArea).getServiceMeasure(_selectedServiceMeasure).getTitle() + "</font></b>");

    // clears current headers so they can be repopulated with current filter
    _tableHHeader.clear();
    _tableVHeader.clear();

    // create a column header for each selected year
    for(QVector<int>::iterator i0 = _selectedYears.begin(); i0 != _selectedYears.end(); ++i0){
        int tempYear = *i0;
        QString s = QString::number(tempYear);
        _tableHHeader << s;
    }

    double currentDouble;
    int row = 0;
    int column = 0;
    double meanValue = 0;
    int size = 0;
    double min = 1000000000;
    double max = -1000000000;
    QString s;
    QTableWidgetItem *item;
    for(QVector<QString>::iterator i1 = _selectedMunis.begin(); i1 != _selectedMunis.end(); ++i1){
        _tableVHeader << *i1;
        for(QVector<int>::iterator i2 = _selectedYears.begin(); i2 != _selectedYears.end(); ++i2){
            // get the data for that municipality at that year
            currentDouble = _data.getServiceAreas().at(_selectedServiceArea).getServiceMeasure(_selectedServiceMeasure).getValue(std::make_pair(*i1, *i2));
            // round the decimal value
            currentDouble = std::floor(currentDouble * 100 + 0.5)/100;
            // if there is no value for that munipality at that year, set the QString to null
            if(currentDouble == 0){
                s = "";
            }
            else{
                // set data value to min if it's less than current min value
                if(currentDouble < min)
                    min = currentDouble;
                // set data value to max if it's more than current max value
                if(currentDouble > max)
                    max = currentDouble;
                // add data value to total
                meanValue += currentDouble;
                // increment number of data values here so mean is only calculated for non-null data values
                ++size;
                s = QString::number(currentDouble);
            }
            // set cell value to the QString value
            item = new QTableWidgetItem(s, 0);
            item->setTextAlignment(Qt::AlignCenter);
            // sets it so that cell values cannot be changed by the user in the ui
            item->setFlags(item->flags() ^ Qt::ItemIsEditable);
            // add item to the table
            ui->tableWidget->setItem(row, column, item);
            ++column;
        }
        ++row;
        column = 0;
    }
    // calculate mean
    meanValue /= size;
    // if there is no data in the table, set mean, min and max to zero by default
    if(_selectedMunis.size() == 0 || _selectedYears.size() == 0){
        ui->totalMeanLabel->setText("Mean = 0");
        ui->totalMaxLabel->setText("Max = 0");
        ui->totalMinLabel->setText("Min = 0");
    }
    // display mean, min and max of the entire table to the user
    else{
        ui->totalMeanLabel->setText("Mean = " + QString::number(meanValue));
        ui->totalMaxLabel->setText("Max = " + QString::number(max));
        ui->totalMinLabel->setText("Min = " + QString::number(min));
    }

    ui->tableWidget->setColumnCount(_tableHHeader.size());
    // set column width so the whole data value is displayed and is not potentially cut off
    for(int i = 0; i < _tableHHeader.size(); ++i){
        ui->tableWidget->setColumnWidth(i, 110);
    }
    ui->tableWidget->setRowCount(_tableVHeader.size());
    // set row and column headers to selected municipalities and years
    ui->tableWidget->setHorizontalHeaderLabels(_tableHHeader);
    ui->tableWidget->setVerticalHeaderLabels(_tableVHeader);

    saveUserData();
}

/**
 *@brief A private member that replots the bar chart
 *Uses all currently selected filters of municipalities and years,
 *replots the chart and shows it on the screen.
 */
void MainWindow::barChartReplot(){
    for (int i = 0; i < _barChart.size(); ++i){
        if (!(ui->chartWidget->hasPlottable(_barChart.at(i)))) ui->chartWidget->addPlottable(_barChart.at(i));
    }
    // add title layout element
    if(ui->chartWidget->plotLayout()->elementCount() == 1){
        ui->chartWidget->plotLayout()->insertRow(0);
        ui->chartWidget->plotLayout()->addElement
                (0, 0, new QCPPlotTitle(ui->chartWidget, _data.getServiceAreas().at(_selectedServiceArea).getServiceMeasure(_selectedServiceMeasure).getTitle().toUtf8().constData()));
    }
    else{
        // overwrite old title with new measure title
        QCPLayoutElement *element = ui->chartWidget->plotLayout()->element(0,0);
        ui->chartWidget->plotLayout()->remove(element);
        ui->chartWidget->plotLayout()->addElement
                (0, 0, new QCPPlotTitle(ui->chartWidget, _data.getServiceAreas().at(_selectedServiceArea).getServiceMeasure(_selectedServiceMeasure).getTitle().toUtf8().constData()));
    }
    ui->chartWidget->clearGraphs();
    ui->chartWidget->legend->setVisible(false);

    double tick = 2;                            //Counter used to place ticks on x-Axis
    QVector<QString> keyData;                   //QVector of QStrings used to store labels of the x-Axis
    QVector<double> allTicks;                   //QVector of doubles used to store every tick placed on the x-Axis
    QMap<int, QVector<double> > tickPlacement;  //QMap of integers signifying the years, which point to a vector of doubles representing the tick placement on the x-Axis for each year
    QMap<int, QVector<double> > valueData;      //QMap of integers signifying the years, which point to a vector of doubles representing the value given to each tick on the x-Axis for each year
    double currentDouble;


    for (int i = 0; i < _data.getServiceAreas().at(_selectedServiceArea).getServiceMeasure(_selectedServiceMeasure).getYears().size(); ++i){  //Loops through all currently selected years and gives a base value to all maps
        tickPlacement[_data.getServiceAreas().at(_selectedServiceArea).getServiceMeasure(_selectedServiceMeasure).getYears().at(i)] << 0;
        valueData[_data.getServiceAreas().at(_selectedServiceArea).getServiceMeasure(_selectedServiceMeasure).getYears().at(i)] << 0;
    }

    for(QVector<QString>::iterator i1 = _selectedMunis.begin(); i1 != _selectedMunis.end(); ++i1){  //Iterates through all selected municipalities
        QVector<int>::iterator i2;
        int i3;
        for(i2 = _selectedYears.begin(), i3 = 0; i2 != _selectedYears.end(); ++i2, ++i3){           //Iterates through all selected years
            if (i3 == _selectedYears.size()/2 && _selectedYears.size()%2 == 1){                     //Checks if there is an odd number of years, and if the current iteration is the middle bar
                keyData << *i1;                                                     //Assigns municipality name to x-Axis label
                currentDouble = _data.getServiceAreas().at(_selectedServiceArea).getServiceMeasure(_selectedServiceMeasure).getValue(std::make_pair(*i1, *i2));
                valueData[*i2] << currentDouble;         //Gets value based on municipality name and year

                tickPlacement[*i2] << tick;
                allTicks << tick;
                tick += 0.75;
            }
            else if (i3 == (_selectedYears.size()/2 - 1) && _selectedYears.size()%2 == 0){          //Checks if there is an even number of years, and if the curent iteraion is about to be the middle bar
                keyData << QString::fromStdString("");                              //Assigns blank name to x-Axis label
                currentDouble = _data.getServiceAreas().at(_selectedServiceArea).getServiceMeasure(_selectedServiceMeasure).getValue(std::make_pair(*i1, *i2));
                valueData[*i2] << currentDouble;         //Gets value based on municipality name and year
                tickPlacement[*i2] << tick;
                allTicks << tick;
                tick += 0.375;
                keyData << *i1;
                allTicks << tick;
                tick += 0.375;

            }
            else{
                keyData << QString::fromStdString("");                              //Assigns blank name to x-Axis label
                currentDouble = _data.getServiceAreas().at(_selectedServiceArea).getServiceMeasure(_selectedServiceMeasure).getValue(std::make_pair(*i1, *i2));
                valueData[*i2] << currentDouble;         //Gets value based on municipality name and year
                tickPlacement[*i2] << tick;
                allTicks << tick;
                tick += 0.75;
            }

        }
        tick += 3;
    }
    ui->chartWidget->clearGraphs();
      if (ui->meanLineButton->isChecked() ){
        ui->chartWidget->addGraph();
        QPen pen;
        pen.setColor(QColor(0,0,0));
        pen.setStyle(Qt::DashLine);
        ui->chartWidget->graph()->setPen(pen);
        ui->chartWidget->graph()->setName(QString::fromStdString("Mean"));
        QVector<double> x(2), y(2);
        x[0] = -1;
        y[0] = FindDoubleMean();
        x[1] = tick-1;
        y[1] = FindDoubleMean();
        ui->chartWidget->graph()->setData(x, y);
        ui->chartWidget->graph()->rescaleAxes(true);
    }

    keyData << QString::fromStdString("");
    if (_selectedYears.size() > 0){                            //Adds an empty bar to use as a buffer on right side
        tickPlacement[_selectedYears.at(0)] << tick - 1.75;
        valueData[_selectedYears.at(0)] << 0;
    }
    allTicks << tick - 1.75;

    for (int i = 0; i < _data.getServiceAreas().at(_selectedServiceArea).getServiceMeasure(_selectedServiceMeasure).getYears().size(); ++i){         //Assigns values to each bar
        _barChart.at(i)->setData(tickPlacement[_data.getServiceAreas().at(_selectedServiceArea).getServiceMeasure(_selectedServiceMeasure).getYears().at(i)],
                valueData[_data.getServiceAreas().at(_selectedServiceArea).getServiceMeasure(_selectedServiceMeasure).getYears().at(i)]);
    }

    /*
      *Formatting fixes to bar graph
      */
    ui->chartWidget->xAxis->setAutoTicks(false);
    ui->chartWidget->xAxis->setAutoTickLabels(false);
    ui->chartWidget->xAxis->grid()->setVisible(false);
    ui->chartWidget->xAxis->setTicks(false);
    ui->chartWidget->xAxis->setTickVector(allTicks);
    ui->chartWidget->xAxis->setTickVectorLabels(keyData);
    ui->chartWidget->xAxis->setTickLabelRotation(-45);
    ui->chartWidget->rescaleAxes();
    ui->chartWidget->replot();

    FindMin();
    FindMax();
    FindMean();
    saveUserData();
}

/**
   * @brief a private member that replots the line graph
   * Uses all currently selected filters of municipalities and years,
   * replots the line graph and shows it on the screen.
   */
void MainWindow::lineGraphReplot(){
    for (int i = 0; i < _barChart.size(); ++i){
        if (ui->chartWidget->hasPlottable(_barChart.at(i))) ui->chartWidget->removePlottable(_barChart.at(i));
    }
    // add title layout element
    if(ui->chartWidget->plotLayout()->elementCount() == 1){
        ui->chartWidget->plotLayout()->insertRow(0);
        ui->chartWidget->plotLayout()->addElement
                (0, 0, new QCPPlotTitle(ui->chartWidget, _data.getServiceAreas().at(_selectedServiceArea).getServiceMeasure(_selectedServiceMeasure).getTitle().toUtf8().constData()));
    }
    else{
        // overwrite old title with new measure title
        QCPLayoutElement *element = ui->chartWidget->plotLayout()->element(0,0);
        ui->chartWidget->plotLayout()->remove(element);
        ui->chartWidget->plotLayout()->addElement
                (0, 0, new QCPPlotTitle(ui->chartWidget, _data.getServiceAreas().at(_selectedServiceArea).getServiceMeasure(_selectedServiceMeasure).getTitle().toUtf8().constData()));
    }

    // clear all graphs and the legend for replotting
    ui->chartWidget->clearGraphs();
    ui->chartWidget->legend->clearItems();

    // set up the legend
    if(ui->legendButton->isChecked())
        ui->chartWidget->legend->setVisible(true);
    ui->chartWidget->legend->setFont(QFont("Cantarell", 8));
    ui->chartWidget->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignRight);

    // QVector of QStrings used to store labels (as years) of the x-Axis
    QVector<QString> keyData;
    // QVector of doubles used to store every tick placed on the x-Axis
    QVector<double> allTicks;

    QPen pen;
    QCPRange range = ui->chartWidget->xAxis->range();
    // set the interval between each data point along the x-Axis
    double interval = range.upper/_selectedYears.size() - 0.125;
    if (interval <= 0) interval = -interval;
    QVector<double> totals;
    QVector<int> counts;
    for (int q = 0; q < _selectedYears.size(); ++q){
        totals.push_back(0);
        counts.push_back(0);
    }

    // add graphs for each municipality
    for (int i = 0; i <= _selectedMunis.size() - 1; ++i){
        ui->chartWidget->addGraph();
        pen.setColor(QColor(sin((i*2)*1+1.2)*80+80, sin((i*2)*0.3+0)*80+80, sin((i*2)*0.3+1.5)*80+80));
        ui->chartWidget->graph()->setPen(pen);
        ui->chartWidget->graph()->setName(_selectedMunis.at(i));
        ui->chartWidget->graph()->setLineStyle(QCPGraph::lsLine);
        ui->chartWidget->graph()->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 5));
        // generate data:
        QVector<double> x(_selectedYears.size()), y(_selectedYears.size());
        for (int j = 0; j < _selectedYears.size(); ++j)
        {
            // set the interval between each data point
            x[j] = j * interval;
            // get the data value at that year for that municipality
            y[j] = _data.getServiceAreas().at(_selectedServiceArea).getServiceMeasure(_selectedServiceMeasure).getValue(std::make_pair(_selectedMunis.at(i), _selectedYears.at(j)));
            if (y[j] != 0){
                ++counts[j];
                totals[j] = totals[j] + y[j];
            }
        }
        ui->chartWidget->graph()->setData(x, y);
        ui->chartWidget->graph()->rescaleAxes(true);
    }
    if (ui->meanLineButton->isChecked()){
        ui->chartWidget->addGraph();
        pen.setColor(QColor(0,0,0));
        pen.setStyle(Qt::DashLine);
        ui->chartWidget->graph()->setPen(pen);
        ui->chartWidget->graph()->setName(QString::fromStdString("Mean"));
        ui->chartWidget->graph()->setLineStyle(QCPGraph::lsLine);
        ui->chartWidget->graph()->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 5));
        QVector<double> x(_selectedYears.size()), y(_selectedYears.size());
        for (int i = 0; i < _selectedYears.size(); ++i){
            x[i] = i * interval;
            if (counts[i] > 0) y[i] = totals[i]/counts[i];
            else y[i] = 0;
        }
        // set the data for that municipality
        ui->chartWidget->graph()->setData(x, y);
        ui->chartWidget->graph()->rescaleAxes(true);
        // remove the empty line graph from the legend
        ui->chartWidget->legend->removeAt(_selectedMunis.size());
    }
    for (int i = 0; i < _selectedYears.size(); ++i){
        keyData << QString::number(_selectedYears.at(i));
        allTicks << i * interval;
    }

    // if there are no municipalities selected, hide the legend (else it would show an empty box)
    if(ui->chartWidget->legend->itemCount() == 0){
        ui->chartWidget->legend->setVisible(false);
    }

    ui->chartWidget->rescaleAxes();
    // zoom out a bit to add padding on each side
    ui->chartWidget->xAxis->scaleRange(1.1, ui->chartWidget->xAxis->range().center());
    ui->chartWidget->yAxis->scaleRange(1.1, ui->chartWidget->yAxis->range().center());
    // set up the line graph with labels and label placement
    ui->chartWidget->xAxis->setAutoTicks(false);
    ui->chartWidget->xAxis->setAutoTickLabels(false);
    ui->chartWidget->xAxis->setTicks(false);
    ui->chartWidget->yAxis->setTicks(true);
    ui->chartWidget->xAxis->grid()->setVisible(false);
    ui->chartWidget->xAxis->setTickVector(allTicks);
    ui->chartWidget->xAxis->setTickVectorLabels(keyData);
    ui->chartWidget->yAxis->setTickLabels(true);
    // dynamically replot the line graph
    ui->chartWidget->replot();

    FindMin();
    FindMax();
    FindMean();
    saveUserData();
}

/**
* @brief a private slot to update municipalities.
* Updates the selected municipalities QVector and dynamically replots the bar graph.
*/
void MainWindow::muniButtonPressed(){

    _selectedMunis.clear();

    ui->muniList->currentItem()->setSelected(!ui->muniList->currentItem()->isSelected());
    ui->muniList->setCurrentRow(ui->muniList->currentRow(), QItemSelectionModel::Toggle );

    QModelIndexList indexes = ui->muniList->selectionModel()->selectedIndexes();

    foreach(QModelIndex index, indexes)
    {
        _selectedMunis.push_back(_data.getServiceAreas().at(_selectedServiceArea).getServiceMeasure(_selectedServiceMeasure).getMunicipalities().at(index.row()));
    }

    // alphabetically sort municipalities
    std::sort(_selectedMunis.begin(), _selectedMunis.end());

    saveUserData();

    // update and replot the graph
    if (_chartNum == 1)barChartReplot();
    else if (_chartNum == 2)lineGraphReplot();
    else {
        tableReplot();
        tableReplot();
    }
}

/**
* @brief a private slot to update the years.
* Updates the selected years QVector and dynamically replots the bar graph.
*/
void MainWindow::yearButtonPressed(){
    // clear selected years
    _selectedYears.clear();

    // get all years check boxes
    QList<QAbstractButton *> yearList = ui->yearGroup->buttons();

    // repopulate selected years QVector with currently checked years in the ui
    for(QList<QAbstractButton *>::iterator i1 = yearList.begin(); i1 != yearList.end(); ++i1){
        if ((*(*i1)).isChecked()){
            _selectedYears.push_back(atoi(((*(*i1)).text()).toLocal8Bit().constData()));
        }
    }

    // numerically sort years
    std::sort(_selectedYears.begin(), _selectedYears.end());

    saveUserData();

    // update and replot the graph
    if (_chartNum == 1)barChartReplot();
    else if (_chartNum == 2)lineGraphReplot();
    else {
        tableReplot();
        tableReplot();
    }
}

/**
 * @brief a private slot to save the graphs as an image
 * Saves the bar graph as a png when the user selects the Save Image button, displays
 * a message whether the save was successful or not
 */
void MainWindow::imageButtonPressed(){
    QMessageBox msgBox;
    if( _selectedMunis.size() > 0 && _selectedYears.size() > 0){

    // letting the user choose the path and file name.
    QString FilePath = QFileDialog::getSaveFileName(0, tr("Save File"), QDir::currentPath(),"All files (*.*)");

    // if no path or file name selected
    bool empty = FilePath.compare("");

    // if the user did not specify the extension
    if (!FilePath.endsWith(".png"))
        FilePath+=".png";

    // save the image as a png
    std::string FilePathSTD = FilePath.toUtf8().constData();
    bool savedImage = ui->chartWidget->savePng(QString::fromStdString(FilePathSTD), 0, 0, 1.0, -1);

    // create message box to display wether the save was successful or not
    // savePng was successful, display success message to the user
    if(empty && savedImage){
        msgBox.setText("Image saved:\n"+ FilePath);
        msgBox.setWindowTitle("Image Export");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
    }
    }
    // savePng couldn't save the bar chart, display error message to the user
    else{
        msgBox.setText("Could not save image, no data selected");
        msgBox.setWindowTitle("Error");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
    }
}

/**
 * @brief a private slot to toggle mean line in graphs
 * Toggles the radio button to either show or hide the mean line
 */
void MainWindow::toggleMeanLine(){
    saveUserData();
    if (_chartNum == 1)barChartReplot();
    else if (_chartNum == 2)lineGraphReplot();
}

/**
 * @brief a private slot to toggle legend in line graph
 */
void MainWindow::toggleLegend(){
    // change display of the legend
    if(ui->legendButton->isChecked()){
        ui->chartWidget->legend->setVisible(true);
    }
    else
        ui->chartWidget->legend->setVisible(false);
    lineGraphReplot();
}

/**
 * @brief a private slot: Calculates the mean value of the selected cells and displays it to the user
 * Displays an error message if empty cells are selected
 */
void MainWindow::meanButtonPressed(){
    ui->meanValueLabel->setVisible(true);
    double meanValue = 0;
    int size = 0;
    // put the selected cells in a QList
    QList <QTableWidgetItem *>	selectedCells = ui->tableWidget->selectedItems();
    // if there are no cells selected, calculate the mean of the whole table by default
    if(selectedCells.size() == 0){
        int column;
        int row;
        for(row = 0; row < ui->tableWidget->rowCount(); ++row){
            for(column = 0; column < ui->tableWidget->columnCount(); ++column){
                QTableWidgetItem *item = ui->tableWidget->item(row, column);
                QString value = item->text();
                double data = value.toDouble();
                if(data != 0){
                    // add data value to the mean if the cell is not empty
                    meanValue += data;
                    ++size;
                }
            }
        }
        // calculate the mean
        meanValue /= size;
        // if there is no data in the table, set mean to zero by default
        if(_selectedMunis.size() == 0 || _selectedYears.size() == 0){
            ui->meanValueLabel->setText("Mean = 0");
        }
        // display mean value to the user
        else{
            ui->meanValueLabel->setText("Mean = " + QString::number(meanValue));
        }
    }
    else{
        // for each selected item:
        for(int i = 0; i < selectedCells.size(); ++i){
            // grab cell item from the selected cells QList
            QTableWidgetItem *item = selectedCells.at(i);
            QString value = item->text();
            double data = value.toDouble();
            if(data != 0){
                // add data value to the mean if the cell is not empty
                meanValue += data;
                ++size;
            }
        }
        // calculate the mean if there is at least one cell containing a value and display it to the user
        if(size != 0){
            meanValue /= size;
            ui->meanValueLabel->setText("Mean = " + QString::number(meanValue));
        }
        else{
            // set up and display error message
            QMessageBox msgBox;
            msgBox.setText("Could not compute mean. No data selected.");
            msgBox.setWindowTitle("Error");
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setDefaultButton(QMessageBox::Ok);
            msgBox.exec();
        }

    }
    tableReplot();
}

/**
 * @brief a private slot: Calculates the min value of the selected cells and displays it to the user
 * Displays an error message if empty cells are selected
 */
void MainWindow::minButtonPressed(){
    double min = 1000000000;
    int size = 0;
    // put the selected cells in a QList
    QList <QTableWidgetItem *>	selectedCells = ui->tableWidget->selectedItems();
    // if there are no cells selected, calculate the mean of the whole table by default
    if(selectedCells.size() == 0){
        int column;
        int row;
        for(row = 0; row < ui->tableWidget->rowCount(); ++row){
            for(column = 0; column < ui->tableWidget->columnCount(); ++column){
                QTableWidgetItem *item = ui->tableWidget->item(row, column);
                QString value = item->text();
                double data = value.toDouble();
                // only compare non-null values
                if(data != 0){
                    // set data value to min if it's less than current min value
                    if(data < min)
                        min = data;
                }
            }
        }
        // if there is no data in the table, set min to zero by default
        if(_selectedMunis.size() == 0 || _selectedYears.size() == 0){
            ui->minValueLabel->setText("Min = 0");
        }
        // display min value to the user
        else{
            ui->minValueLabel->setText("Min = " + QString::number(min));
        }
    }
    else{
        // for each selected item:
        for(int i = 0; i < selectedCells.size(); ++i){
            // grab cell item from the selected cells QList
            QTableWidgetItem *item = selectedCells.at(i);
            QString value = item->text();
            double data = value.toDouble();
            // only compare non-null values
            if(data != 0){
                ++size;
                // set data value to min if it's less than current min value
                if(data < min)
                    min = data;
            }
        }
        // display min value to the user if there is at least one cell containing a data value
        if(size != 0){
            ui->minValueLabel->setText("Min = " + QString::number(min));
        }
        else{
            // set up and display error message
            QMessageBox msgBox;
            msgBox.setText("Could not compute min. No data selected.");
            msgBox.setWindowTitle("Error");
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setDefaultButton(QMessageBox::Ok);
            msgBox.exec();
        }

    }
    tableReplot();
}

/**
 * @brief a private slot: Calculates the max value of the selected cells and displays it to the user
 * Displays an error message if empty cells are selected
 */
void MainWindow::maxButtonPressed(){
    double max = -1000000000;
    int size = 0;
    // put the selected cells in a QList
    QList <QTableWidgetItem *>	selectedCells = ui->tableWidget->selectedItems();
    // if there are no cells selected, calculate the mean of the whole table by default
    if(selectedCells.size() == 0){
        int column;
        int row;
        for(row = 0; row < ui->tableWidget->rowCount(); ++row){
            for(column = 0; column < ui->tableWidget->columnCount(); ++column){
                QTableWidgetItem *item = ui->tableWidget->item(row, column);
                QString value = item->text();
                double data = value.toDouble();
                // only compare non-null values
                if(data != 0){
                    // set data value to max if it's more than current max value
                    if(data > max)
                        max = data;
                }
            }
        }
        // if there is no data in the table, set max to zero by default
        if(_selectedMunis.size() == 0 || _selectedYears.size() == 0){
            ui->maxValueLabel->setText("Max = 0");
        }
        // display max value to the user
        else{
            ui->maxValueLabel->setText("Max = " + QString::number(max));
        }
    }
    else{
        // for each selected item:
        for(int i = 0; i < selectedCells.size(); ++i){
            // grab cell item from the selected cells QList
            QTableWidgetItem *item = selectedCells.at(i);
            QString value = item->text();
            double data = value.toDouble();
            // only compare non-null values
            if(data != 0){
                ++size;
                // set data value to max if it's more than current max value
                if(data > max)
                    max = data;
            }
        }
        // display max value to the user if there is at least one cell containing a data value
        if(size != 0){
            ui->maxValueLabel->setText("Max = " + QString::number(max));
        }
        else{
            // set up and display error message
            QMessageBox msgBox;
            msgBox.setText("Could not compute max. No data selected.");
            msgBox.setWindowTitle("Error");
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setDefaultButton(QMessageBox::Ok);
            msgBox.exec();
        }

    }
    tableReplot();
}
/**
 * @brief a private method to save the user data
 */
void MainWindow::saveUserData(){
    QDir tempDir;
    std::string tempStr = tempDir.absolutePath().toUtf8().constData();
    tempStr = tempStr.substr(0, tempStr.length() - 24) + "src/userData.txt";
    std::ofstream userFile;
    userFile.open(tempStr.c_str());  //Opens an output file to user data file

    userFile << _selectedServiceArea << "," << _selectedServiceMeasure.toUtf8().constData() << "," << _chartNum << std::endl;

    for (int i = 0; i < _selectedMunis.size(); ++i){
        if (i != _selectedMunis.size()-1) userFile << _selectedMunis.at(i).toUtf8().constData() << ",";
        else userFile << _selectedMunis.at(i).toUtf8().constData();
    }
    userFile << std::endl;

    std::string output = "";
    QList<QAbstractButton *> tempList = ui->yearGroup->buttons();
    for (QList<QAbstractButton *>::iterator it = tempList.begin(); it != tempList.end(); ++it){
        if ((*it)->isChecked()){
            output.append((*it)->text().toUtf8().constData());
            output.append(",");
        }
    }
    userFile << output.substr(0, output.length() - 1) << std::endl;

    if (ui->meanLineButton->isChecked()) userFile << "1" << std::endl;
    else userFile << "0" << std::endl;

    userFile.close();
}

/**
 * @brief A private method: Saves the selected data into a csv file.
 */
void MainWindow::toCSVFile()
{
    QMessageBox msgBox;
    if( _selectedMunis.size() > 0 && _selectedYears.size() > 0){
    //leting the user choose the path and name of the file
    QString FilePath = QFileDialog::getSaveFileName(0, tr("Save File"),QDir::currentPath(),"All files (*.*)");

    // if no path or file name selected
    bool empty = FilePath.compare("");

    //making sure the file is .csv
    if (!FilePath.endsWith(".csv"))
        FilePath+=".csv";

    std::string FilePathSTD = FilePath.toUtf8().constData();

    std::ofstream outputFile;
    outputFile.open(FilePathSTD.c_str());

    QVector< QVector<QString> > table (_selectedMunis.count() +1) ;

    table [0].push_back("Municipality");

    // adding the years to the 2D QVector
    for(int i=0; i< _selectedYears.count() ; ++i)
    {
        table[0].push_back(QString::number(_selectedYears[i]));
    }
    // adding the municipality names to the 2D QVector
    for ( int i=0 ; i<_selectedMunis.count(); ++i)
    {
        table [i+1].push_back(_selectedMunis[i]);
    }

    //getting the values from the data classes and adding it to the 2D vector.
    std::pair<QString, int> valuePair;
    int value;

    for (int i=0 ;  i< _selectedMunis.count() ; ++i)
    {
        for ( int j=0 ; j< _selectedYears.count() ; ++j)
        {
            valuePair.first= _selectedMunis[i];
            valuePair.second=_selectedYears[j];
            value=_data.getServiceAreas().at(_selectedServiceArea).getServiceMeasure(_selectedServiceMeasure).getValue(valuePair);
            table[i+1].push_back(QString::number(value));
        }
    }

    for( int i=0 ; i< table.count() ; ++i )
    {
        for ( int j=0 ;j< table[i].count() ; ++j )
        {
            QString value = table[i][j];
            outputFile <<value.toStdString();

            if ( j < table[i].count()-1)
            {
                outputFile<< ",";
            }

        }
        outputFile<< "\n";
    }

    outputFile.close();

    // create message box to display wether the save was successful or not
    // saveCSV was successful, display success message to the user
    if(empty){
        msgBox.setText("File saved:\n"+ FilePath);
        msgBox.setWindowTitle("File Export");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
    }
    }
    // saveCSV couldn't save the table, display error message to the user
    else{
        msgBox.setText("Could not save file, no data selected");
        msgBox.setWindowTitle("File Export");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
    }
}

/**
 * @brief A public method that calculates the min value in the chart.
 */
void MainWindow::FindMin()
{
    QVector<double>  allValues ;
    double min =0;
    double value =0;

    //getting the values from the data classes and adding it to the vector.
    std::pair<QString, int> valuePair;

    //looping thorugh all the values of service measure and save them in a vector.
    for (int i=0 ;  i< _selectedMunis.count() ; ++i)
    {
        for ( int j=0 ; j< _selectedYears.count() ; ++j)
        {
            valuePair.first= _selectedMunis[i];
            valuePair.second=_selectedYears[j];
            value=_data.getServiceAreas().at(_selectedServiceArea).getServiceMeasure(_selectedServiceMeasure).getValue(valuePair);
            //skipping empty values.
            if (value!=0)
            {
                allValues.push_back(value);
            }
        }
    }

    // finding the minimum value in the vector of values.
    min= *min_element(allValues.begin(), allValues.end());
    //display the minimum value.
    ui->totalMinLabel->setText("Min = " + QString::number(min));
}

/**
 * @brief A public method that calculates the max value of the chart
 */
void MainWindow::FindMax()
{
    QVector<double>  allValues ;
    double max =0;
    double value =0;

    //getting the values from the data classes and adding it to the vector.
    std::pair<QString, int> valuePair;

    //looping thorugh all the values of service measure and save them in a vector.
    for (int i=0 ;  i< _selectedMunis.count() ; ++i)
    {
        for ( int j=0 ; j< _selectedYears.count() ; ++j)
        {
            valuePair.first= _selectedMunis[i];
            valuePair.second=_selectedYears[j];
            value=_data.getServiceAreas().at(_selectedServiceArea).getServiceMeasure(_selectedServiceMeasure).getValue(valuePair);
            //skipping empty values.
            if (value!=0)
            {
                allValues.push_back(value);
            }
        }
    }

    // finding the Maximum value in the vector of values.
    max= *max_element(allValues.begin(), allValues.end());
    //display the Maximum value.
    ui->totalMaxLabel->setText("Max = " + QString::number(max));
}

/**
 * @brief A public method that calculates the mean value of the data shown in the graph.
 */
void MainWindow::FindMean()
{
    double total = 0;
    double value = 0;
    int size = 0;
    double mean = 0;

    //getting the values from the data classes and adding it to the vector.
    std::pair<QString, int> valuePair;

    //looping thorugh all the values of service measure and save them in a vector.
    for (int i=0 ;  i< _selectedMunis.count() ; ++i)
    {
        for ( int j=0 ; j< _selectedYears.count() ; ++j)
        {
            valuePair.first= _selectedMunis[i];
            valuePair.second=_selectedYears[j];
            value=_data.getServiceAreas().at(_selectedServiceArea).getServiceMeasure(_selectedServiceMeasure).getValue(valuePair);
            //skipping empty values.
            if (value!=0)
            {
                total = total + value;
                ++size;
            }
        }
    }

    //making sure that the size is not zero.
    if ( size != 0)
    {
        mean = total/size;
    }
    //display Mean
    ui->totalMeanLabel->setText("Mean = " + QString::number(mean));
}

double MainWindow::FindDoubleMean()
{
   double total = 0;
   double value = 0;
   int size = 0;
   double mean = 0;

    //getting the values from the data classes and adding it to the vector.
    std::pair<QString, int> valuePair;

    //looping thorugh all the values of service measure and save them in a vector.
    for (int i=0 ;  i< _selectedMunis.count() ; ++i)
    {
        for ( int j=0 ; j< _selectedYears.count() ; ++j)
        {
            valuePair.first= _selectedMunis[i];
            valuePair.second=_selectedYears[j];
            value=_data.getServiceAreas().at(_selectedServiceArea).getServiceMeasure(_selectedServiceMeasure).getValue(valuePair);
            //skipping empty values.
            if (value!=0)
            {
                total = total + value;
                ++size;
            }
        }
    }

    //making sure that the size is not zero.
    if ( size != 0)
    {
        mean = total/size;
    }
    //display Mean
   // ui->totalMeanLabel->setText("Mean = " + QString::number(mean));
    return mean;
}

/**
 * @brief a private slots to show Main Screen
 */
void MainWindow::showMainScreen() {
    _welcomeScreen->hide();
    _mainScreen->show();
}

/**
 * @brief a private slots to go back To Main Menu
 */
void MainWindow::backToMainMenu()
{
    this->hide();
    _mainScreen->show();
}

/**
 * @brief a private slots to select a service area.
 */
void MainWindow::userSelectArea()
{
    changeChartNum();
    _mainScreen->hide();
    _selectedServiceArea = _mainScreen->getServiceSelector()->currentIndex();
    _selectedServiceMeasure = _data.getServiceAreas().at(_selectedServiceArea).getServiceMeasure("0").getMeasureID();
    QVector<QString> measures = ServiceArea(_mainScreen->getServiceSelector()->currentText()).getServiceMeasuresIDs();

    _serviceMeasures->clear();
    for(int i = 0; i < measures.size(); i++) {
        _serviceMeasures->addItem(measures.at(i), QVariant(i));
    }

    _selectedMunis = _data.getServiceAreas().at(_selectedServiceArea).getServiceMeasure(0).getMunicipalities();
    _selectedYears = _data.getServiceAreas().at(_selectedServiceArea).getServiceMeasure(0).getYears();

    std::sort(_selectedYears.begin(), _selectedYears.end());

    ui->muniList->clear();

    for (int i = 0; i < _selectedMunis.size(); ++i){
        ui->muniList->addItem(_selectedMunis.at(i));
        ui->muniList->item(i)->setSelected(true);
    }

    ui->y2008Button->setChecked(true);
    ui->y2009Button->setChecked(true);
    ui->y2010Button->setChecked(true);
    ui->y2011Button->setChecked(true);
    ui->y2012Button->setChecked(true);

    // update and replot the graph
    if (_chartNum == 1)startBarChart();
    else if (_chartNum == 2)startLineGraph();
    else {
        startTable();
    }
    this->show();
}

/**
 * @brief a private slots to change the service measure.
 */
void MainWindow::measureChanged()
{
    _selectedServiceMeasure = ui->serviceMeasures->currentText();
    if (_chartNum == 1)barChartReplot();
    else if (_chartNum == 2)lineGraphReplot();
    else {
        tableReplot();
        ui->maxValueLabel->clear();
        ui->minValueLabel->clear();
        ui->meanValueLabel->clear();
    }
}

/**
 * @brief a private slots to change chart number
 */
void MainWindow::changeChartNum()
{
    _chartNum = _mainScreen->getSelectedRadio();
}

/**
 * @brief a private slots for user to Select Water
 */
void MainWindow::userSelectWater()
{
    _mainScreen->getServiceSelector()->setCurrentIndex(0);
}

/**
 * @brief A private slot for  user to Select Waste
 */
void MainWindow::userSelectWaste()
{
    _mainScreen->getServiceSelector()->setCurrentIndex(1);
}

/**
 * @brief A private slot user to Select Library
 */
void MainWindow::userSelectLibrary()
{
    _mainScreen->getServiceSelector()->setCurrentIndex(2);
}

/**
 * @brief A private slot user to select fire
 */
void MainWindow::userSelectFire()
{
    _mainScreen->getServiceSelector()->setCurrentIndex(3);
}

/**
 * @brief A private slot user to Select Police
 */
void MainWindow::userSelectPolice()
{
    _mainScreen->getServiceSelector()->setCurrentIndex(4);
}

/**
 * @brief A private slot user to Select Parks
 */
void MainWindow::userSelectParks()
{
    _mainScreen->getServiceSelector()->setCurrentIndex(5);
}
