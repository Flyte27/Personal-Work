/**
* A mainwindow class. Retrieves table data and dynamically updates the bar graph according to
* user selections.
* @author Jennifer and Alex
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QString>
#include <QComboBox>
#include <QStandardItemModel>
#include <fstream>

#include "welcomescreen.h"
#include "mainscreen.h"
#include "ombidata.h"
#include "qcustomplot.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
    * A constructor.
    * Instantiates a ui and displays all default graph data.
    */
    explicit MainWindow(QWidget *parent = 0);
    /**
    * A destructor.
    * Deletes ui.
    */
    ~MainWindow();

private:
    /**
    * a private variable.
    * Contains ui elements and displays bar graph.
    */
    Ui::MainWindow *ui;

    /**
     * @brief A pointer to the WelcomeScreen
     */
    WelcomeScreen *_welcomeScreen;

    /**
     * @brief A pointer to the MainScreen
     */
    MainScreen *_mainScreen;

    /**
     * @brief A pointer to the service measures selector.
     */
    QComboBox *_serviceMeasures;

    /**
    * a private variable.
    * @brief A QVector of currently selected municipalities.
    */
    QVector<QString> _selectedMunis;

    /**
    * a private variable.
    * @brief A QVector of currently selected years.
    */
    QVector<int> _selectedYears;

    /**
    * a private variable.
    * @brief An integer showing currently selected serviceArea.
    */
    int _selectedServiceArea;

    /**
    * a private variable.
    * @brief A string showing ID of currently selected service measure.
    */
    QString _selectedServiceMeasure;

    /**
    * a private variable.
    * @brief Contains all the data for each municipality and corresponding year.
    */
    OMBIData _data;

    /**
    * a private variable.
    * @brief A QVector containing QCPBars for each year.
    */
    QVector<QCPBars *> _barChart;

    QCPItemLine *_meanLine;

    /**
     * a private variable.
     * @brief A QStringList containing column header for
     * each selected year
     */
    QStringList _tableHHeader;

    /**
     * a private variable.
     * @brief A QStringList containing row header for
     * each selected municipality
     */
    QStringList _tableVHeader;

    int _chartNum;

    /**
     * a private member.
     * @brief Uses all currently selected filters of municipalities and years,
     * replots the chart and shows it on the screen.
     */
    void barChartReplot();

    /**
     * a private method.
     * @brief Initializes all information to create a bar chart
     */
    void startBarChart();

    /**
     * a private member.
     * @brief Uses all currently selected filters of municipalities and years,
     * replots the table and shows it on the screen.
     */
    void tableReplot();

    /**
     * a private method.
     * @brief Initializes all information to create a table
     */
    void startTable();

    /**
     * a private member.
     * @brief Uses all currently selected filters of municipalities and years,
     * replots the line graph and shows it on the screen.
     */
    void lineGraphReplot();

    /**
     * a private method.
     * @brief Initializes all information to create a line graph
     */
    void startLineGraph();

    /**
     * @brief A public method that calculates the min value in the chart.
     */
    void FindMin();

    /**
     * @brief A public method that calculates the max value of the chart
     */
    void FindMax();

    /**
     * A public method that calculates the mean value of the data shown in the graph.
     * @brief MainWindow::FindMean
     */
    void FindMean();
    
    /**
     * @brief Returns mean value of the data as a double.
     */
    double FindDoubleMean();


private slots:
    /**
     * @brief Changes the chart number when user changes radio buttons.
     */
    void changeChartNum();

    /**
     * @brief Shows MainScreen when user clicks the Start button.
     */
    void showMainScreen();

    /**
     * @brief Shows MainScreen when user clicks back to main menu.
     */
    void backToMainMenu();

    /**
     * @brief Changes selected service area when user changes the service area selector.
     */
    void userSelectArea();
    
    /**
     * @brief Changes the selected service measure when user changes the service measure selector.
     */
    void measureChanged();
    
    /**
     * @brief Changes the service area selector when user clicks the fire icon button.
     */
    void userSelectFire();
    
    /**
     * @brief Changes the service area selector when user clicks the library icon button.
     */
    void userSelectLibrary();
    
    /**
     * @brief Changes the service area selector when user clicks the parks icon button.
     */
    void userSelectParks();
    
    /**
     * @brief Changes the service area selector when user clicks the police icon button.
     */
    void userSelectPolice();
    
    /**
     * @brief Changes the service area selector when user clicks the waste icon button.
     */
    void userSelectWaste();
    
    /**
     * @brief Changes the service area selector when user clicks the water icon button.
     */
    void userSelectWater();

    /**
    * a private slot.
    * Updates the selected municipalities QVector and dynamically replots the bar graph.
    */
    void muniButtonPressed();

    /**
    * a private slot.
    * @brief Updates the selected years QVector and dynamically replots the bar graph.
    */
    void yearButtonPressed();

    /**
     * a private slot.
     * @brief Saves the bar graph as a png when the user selects the Save Image button, displays
     * a message whether the save was successful or not
     */
    void imageButtonPressed();

    /**
     * a private slot.
     * @brief Toggles the mean button in order to show or hide it
     */
    void toggleMeanLine();
    
    /**
     * a private slot.
     * @brief Toggles the legend to hide/show it on the line graph
     */
    void toggleLegend();

    /**
     * a private slot.
     * @brief Calculates the mean value of the selected cells and displays it to the user
     * Displays an error message if empty cells are selected
     */
    void meanButtonPressed();

    /**
     * a private slot.
     * @brief Calculates the min value of the selected cells and displays it to the user
     * Displays an error message if empty cells are selected
     */
    void minButtonPressed();

    /**
     * a private slot.
     * @brief Calculates the max value of the selected cells and displays it to the user
     * Displays an error message if empty cells are selected
     */
    void maxButtonPressed();

    /**
     * @brief Saves the user data to UserData.txt
     */
    void saveUserData();

    /**
     * A private method
     * @brief Saves the selected data into a csv file.
     */
    void toCSVFile();

};

#endif // MAINWINDOW_H
