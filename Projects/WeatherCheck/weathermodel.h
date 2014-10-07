/*Created by Alex Payne
 *weathermodel.h is a header file containing a interface for the WeaterModel object class
 *October 7, 2013
*/
#ifndef WEATHERMODEL_H
#define WEATHERMODEL_H

#include <QtNetwork/QNetworkReply>
#include <QObject>
#include <QStringList>

using std::string;

class WeatherModel : public QObject
{
    Q_OBJECT

public:
    explicit WeatherModel();    //Constructor for WeatherModel object
    QStringList getTowns();     //Getter method for townlist
    double getTemp();           //Getter method for temperature
    double getHumidity();       //Getter method for humidity
    double getPressure();       //Getter method for pressure
    double getWindSpeed();      //Getter method for windspeed
    double getWindDir();        //Getter method for wind direction
    int getSunrise();           //Getter method for sunrise unix timestamp
    int getSunset();            //Getter method for sunset unix timestamp
    void queryWeather(int);     //Function to query for a new set of weather data

signals:
    void updateUI();

private:
    QStringList _townList;      //QStringList which holds the list of available towns
    double _temp;               //Double holding the celsius temperature
    double _humidity;           //Double holding the humidity
    double _pressure;           //Double holding the pressure
    double _windSpeed;          //Double holding the wind speed
    double _windDir;            //Double holding wind direction
    int _sunrise;               //Int holding sunrise unix timestamp
    int _sunset;                //Int holding sunset unix timestamp

private slots:
    void replyFinished(QNetworkReply * reply);  //Slot used to update attributes
};

#endif
