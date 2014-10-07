/*Created by Alex Payne
 *mainwindow.h is a header file containing a interface for the MainWindow object class
 *October 7, 2013
*/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QNetworkReply>
#include <weathermodel.h>
#include <fstream>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);   //Constructor for MainWindow object
    ~MainWindow();      //Deconstructor for MainWindow object

private:
    Ui::MainWindow *ui;     //User interface attribute
    WeatherModel _data;     //Weathermodel data attribute
    bool _buttonSwitch = false;     //Boolean switch used to show when getWeatherButton has been clicked
    void saveUserData();    //Method used to save user data

private slots:
    void weatherButtonClicked();    //Slot to get new weather information
    void updateWeatherUI();         //Slot to update GUI with new weather information
    void celsiusButtonClicked();    //Slot to select celsius radio button
    void fahrenheitButtonClicked(); //Slot to select fahrenheit radio button
    void kelvinButtonClicked();     //Slot to select kelvin radio button
    void cityBoxChanged();          //Slot to query new city
};

#endif // MAINWINDOW_H
