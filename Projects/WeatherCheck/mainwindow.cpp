/*Created by Alex Payne
 *mainwindow.cpp is a c++ file used as a controller to handle
 *both the data elements of the weather as well
 *as the GUI shown to the user.
 *October 7, 2013
*/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "weathermodel.h"
#include <iostream>

#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <QUrl>
#include <QUrlQuery>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <QDebug>
#include <QFontDatabase>
#include <string>
#include <QStringList>
#include <fstream>

/*Name: MainWindow
 *Description: The constructor which initiates a MainWindow object
 *Parameter Description: The main widget which holds the user interface
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)  //Creates and initiates the user interface
{
    ui->setupUi(this);      //Sets up the user interface on to the MainWindow class

    ui->sourceText->setVisible(false);                      //Hides the text sourcing OpenWeatherMap.com

    QImage image(":/seasons4 (1).jpg");                         //Creates a QImage object of seasons4 (1).jpg
    ui->backgroundPic->setPixmap(QPixmap::fromImage(image));    //Applies said images to backGroundPic label, causing it to become the background image

    QFont font1("Liberation Mono", 12, QFont::Bold);
    QFont font2("Waree", 16, QFont::Bold);
    QFont font3("TeX Gyre Bonum", 12, QFont::Bold);
    QFont font4("Serif", 12, QFont::Bold);
    ui->tempText->setFont(font2);           //Applies second font type to temperature text label
    ui->pressureText->setFont(font1);       //Applies first font type to pressure text label
    ui->humidText->setFont(font1);          //Applies first font type to humidity text label
    ui->windSpeedText->setFont(font3);      //Applies third font type to wind speed text label
    ui->windDirText->setFont(font3);        //Applies third font type to wind direction text label
    ui->sunriseText->setFont(font4);        //Applies fourth font type to sunrise time
    ui->sunsetText->setFont(font4);         //Applies fourth font type to sunset time

    QStringList townList(_data.getTowns());     //Retrieves list of query-able towns from data class
    ui->cityBox->addItems(townList);            //Adds list to drop down box to be selected

    std::ifstream userFile;                                     //Creates input file stream to read in user data
    userFile.open("/home/student/Downloads/userData.txt");      //Reads in user data
    if (userFile.fail())                                        //Prints error message if unable to open file
    {
    std::cout << "Unable to open file" << std::endl;
    }
    int saveTown;
    userFile >> saveTown;   //Reads in integer representing town number index
    std::string saveType;
    userFile >> saveType;   //Reads in string representing temperature display type
    userFile.close();

    ui->cityBox->setCurrentIndex(saveTown);     //Sets current town to stored index
    if (saveType == "celsius"){ ui->celsiusButton->setChecked(true); }              //Sets radio button to celsius if stored
    else if (saveType == "fahrenheit"){ ui->fahrenheitButton->setChecked(true); }   //Sets radio button to fahrenheit if stored
    else { ui->kelvinButton->setChecked(true); }                                    //Sets radio button to kelvin if stored
    connect(ui->weatherButton,SIGNAL(clicked()),this,SLOT(weatherButtonClicked()));         //Signals weatherButtonClicked slot when user attempts to retrieve weather
    connect(ui->celsiusButton,SIGNAL(clicked()),this,SLOT(celsiusButtonClicked()));         //Signals celsiusButtonClicked slot if user switches to celsius radio button
    connect(ui->fahrenheitButton,SIGNAL(clicked()),this,SLOT(fahrenheitButtonClicked()));   //Signals fahrenheitButtonClicked slot if user switches to fahrenheit radio button
    connect(ui->kelvinButton,SIGNAL(clicked()),this,SLOT(kelvinButtonClicked()));           //Signals kelvinButtonClicked slot if user switches to kelvin radio button
    connect(ui->cityBox, SIGNAL(currentIndexChanged(int)), this, SLOT(cityBoxChanged()));   //Signals cityBoxChanged slot if user switches displayed city
    connect( &_data, SIGNAL(updateUI()), this, SLOT(updateWeatherUI()));                    //Signals updateWeatherUI slot after retrieving data to update the user interface
}

/*Name: ~MainWindow
 *Description: Deconstructor used to reallocated memory of a MainWindow object
 */
MainWindow::~MainWindow()
{
    delete ui;  //Deallocates MainWindow object memory
}

/*Name: saveUserData
 *Description: Function used to save user information, such as town and type of temperature
 */
void MainWindow::saveUserData(){
    std::ofstream userFile;
    userFile.open("/home/student/Downloads/userData.txt");  //Opens an output file to user data file
    int cInt = ui->cityBox->currentIndex();     //Sets integer variable to current index of town list
    userFile << cInt << std::endl;              //Streams integer into output file
    if (ui->celsiusButton->isChecked()){ userFile << "celsius" << std::endl;}               //If celsius radio button is currently selected, writes celsius to file
    else if (ui->fahrenheitButton->isChecked()){ userFile << "fahrenheit" << std::endl;}    //If fahrenheit radio button is currently selected, writes fahrenheit to file
    else {userFile << "kelvin" << std::endl;}                                               //If kelvin radio button is currently selected, writes kelvin to file
    userFile.close();
}

/*Name: weatherButtonClicked
 *Description: Slot function which calls the data object to query
 */
void MainWindow::weatherButtonClicked()
{
    _data.queryWeather(ui->cityBox->currentIndex());    //Calls data object to query

}

/*Name: updateWeatherUI
 *Description: Function used to update the user interface when the weather information changes
 */
void MainWindow::updateWeatherUI()
{
    ui->sourceText->setVisible(true);   //Shows source text to OpenWeatherMap

    if (ui->celsiusButton->isChecked()){
        QString tempDisplay("Temperature = " + QString::number(_data.getTemp(), 'f', 2) + " °C");   //Gets celsius temperature from weathermodel object
        ui->tempText->setText("<font color='darkblue'>" + tempDisplay+"</font>");                   //Stores it in temperature text label
    }
    else if (ui->fahrenheitButton->isChecked()){
        QString tempDisplay("Temperature = " + QString::number((_data.getTemp()*9/5+32), 'f', 2) + " °F");  //Gets fahrenheit temperature from weathermodel object
        ui->tempText->setText("<font color='darkblue'>" + tempDisplay+"</font>");                           //Stores it in temperature text label
    }
    else{
        QString tempDisplay("Temperature = " + QString::number(_data.getTemp()+273.15, 'f', 2) + " K");     //Gets kelvin temperature from weathermodel object
        ui->tempText->setText("<font color='darkblue'>" + tempDisplay+"</font>");                           //Stores it in temperature text label
    }

    QString humidDisplay("Humidity = " + QString::number(_data.getHumidity(), 'f', 0) + "%");   //Gets humidity from weathermodel object
    ui->humidText->setText("<font color='darkred'>" + humidDisplay +"</font>");                 //Stores it in humidity text label
    QString presDisplay("Pressure = " + QString::number(_data.getPressure(), 'f', 0) + " kPa"); //Gets pressure from weathermodel object
    ui->pressureText->setText("<font color='darkred'>" + presDisplay +"</font>");               //Stores it in pressure text label
    QString windSpeedDisplay("Wind Speed = " + QString::number(_data.getWindSpeed(), 'f', 1) + " km/h");    //Gets wind speed from weathermodel object
    ui->windSpeedText->setText("<font color='forestgreen'>" + windSpeedDisplay +"</font>");                 //Stores it in wind speed text label
    QString windDirDisplay("Wind Direction = " + QString::number(_data.getWindDir(), 'f', 0) + " deg");     //Gets wind direction from weathermodel object
    ui->windDirText->setText("<font color='forestgreen'>" + windDirDisplay +"</font>");                     //Stores it in wind direction text label
    int sunrise = ((_data.getSunrise() + (3600*20))%86400); //Converts sunrise into seconds per day
    int sunriseHours = sunrise/3600;                        //Calculates hours until sunrise
    int sunriseMinutes = ((sunrise%3600)/60);               //Calculates minutes between hours
    QString sunriseDisplay("Sunrise at " + QString::number(sunriseHours, 'f', 0) + ":" + QString::number(sunriseMinutes, 'f', 0));
    ui->sunriseText->setText("<font color='darkgoldenrod'>" + sunriseDisplay +"</font>");   //Stores sunrise string in sunrise text label
    int sunset = ((_data.getSunset() + (3600*20))%86400);   //Converts sunset into seconds per day
    int sunsetHours = sunset/3600;                          //Calculates hours until sunset
    int sunsetMinutes = ((sunset%3600)/60);                 //Calculates minutes between hours
    QString sunsetDisplay("Sunset at " + QString::number(sunsetHours, 'f', 0) + ":" + QString::number(sunsetMinutes, 'f', 0));
    ui->sunsetText->setText("<font color='darkgoldenrod'>" + sunsetDisplay +"</font>");     //Stores sunset string in sunset text label
    _buttonSwitch = true;   //Sets switch to true to stop ui from showing unwanted data
    ui->weatherButton->hide();  //Hides get weather button
    saveUserData();         //Calls user data save function
}

/*Name: celsiusButtonClicked
 *Description: Slot function used to change the temperature to celsius
 */
void MainWindow::celsiusButtonClicked()
{
    if (_buttonSwitch){     //If getWeather button has been clicked
    QString tempDisplay("Temperature = " + QString::number(_data.getTemp(), 'f', 2) + " °C");       //Gets temperature in celsius format
    ui->tempText->setText("<font color='darkblue'>" + tempDisplay+"</font>");}                      //Stores in temperature text label
    saveUserData();     //Calls user data save function
}

/*Name: fahrenButtonClicked
 *Description: Slot function used to change the temperature to fahrenheit
 */
void MainWindow::fahrenheitButtonClicked()
{
    if (_buttonSwitch){     //If getWeather button has been clicked
    QString tempDisplay("Temperature = " + QString::number((_data.getTemp()*9/5+32), 'f', 2) + " °F");  //Gets temperature in fahrenheit format
    ui->tempText->setText("<font color='darkblue'>" + tempDisplay+"</font>");}                          //Stores in temperature text label
    saveUserData();     //Calls user data save function
}

/*Name: kelvinButtonClicked
 *Description: Slot function used to change the temperature to kelvin
 */
void MainWindow::kelvinButtonClicked()
{
    if (_buttonSwitch){     //If getWeather button has been clicked
    QString tempDisplay("Temperature = " + QString::number(_data.getTemp()+273.15, 'f', 2) + " K");     //Gets temperature in kelvin format
    ui->tempText->setText("<font color='darkblue'>" + tempDisplay+"</font>");}                          //Stores in temperature text label
    saveUserData();     //Calls user data save function
}

/*Name: cityBoxChanged
 *Description: Slot function used to query weather data after switching cities
 */
void MainWindow::cityBoxChanged()
{
    if (_buttonSwitch){     //If getWeather button has been clicked
    _data.queryWeather(ui->cityBox->currentIndex());    //Calls weathermodel object to query new city weather
    }
}
