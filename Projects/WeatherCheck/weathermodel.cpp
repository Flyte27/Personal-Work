/*Created by Alex Payne
 *weathermodel.cpp is a c++ file to query OpenWeatherMap.com
 *for weather data, which it then stores in this class.
 *October 7, 2013
*/


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
#include <string>
#include <QStringList>

using std::string;
/*Name: WeatherModel
 *Description: Constructor used to initiate a new WeatherModel object with a list of towns
 */
WeatherModel::WeatherModel()
{
    _townList.append("Ajax");
    _townList.append("Kingston");
    _townList.append("London");
    _townList.append("Newmarket");
    _townList.append("Thunder Bay");
}

/*Name: getTowns
 *Description: Getter method used to retrieve the list of towns to query
 *Return Description: A QStringList object of the list of towns
 */
QStringList WeatherModel::getTowns(){
    return _townList;
}

/*Name: getTemp
 *Description: Getter method used to retrieve the weather temperature
 *Return Description: A double variable of the temperature
 */
double WeatherModel::getTemp(){
    return _temp;
}

/*Name: getHumidity
 *Description: Getter method used to retrieve the weather humidity
 *Return Description: A double variable of the humidity
 */
double WeatherModel::getHumidity(){
    return _humidity;
}

/*Name: getPressure
 *Description: Getter method used to retrieve the weather pressure
 *Return Description: A double variable of the pressure
 */
double WeatherModel::getPressure(){
    return _pressure;
}

/*Name: getWindSpeed
 *Description: Getter method used to retrieve the wind speed
 *Return Description: A double variable of the wind speed
 */
double WeatherModel::getWindSpeed(){
    return _windSpeed;
}

/*Name: getWindDir
 *Description: Getter method used to retrieve the wind direction
 *Return Description: A double variable of the wind direction
 */
double WeatherModel::getWindDir(){
    return _windDir;
}

/*Name: getSunrise
 *Description: Getter method used to retrieve the time of sunrise
 *Return Description: A integer variable of the time of sunrise
 */
int WeatherModel::getSunrise(){
    return _sunrise;
}

/*Name: getSunset
 *Description: Getter method used to retrieve the time of sunset
 *Return Description: A integer variable of the time of sunset
 */
int WeatherModel::getSunset(){
    return _sunset;
}

/*Name: queryWeather
 *Description: Function used to query OpenWeatherMap.com to retrieve WeatherModel
 *Parameter Description: Integer representing current town selected from list
 */
void WeatherModel::queryWeather(int townNum){
    //the network manager will post and recieve our HTTP requests
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);

    //the host of the webservice
    QUrl url("http://openweathermap.org/data/2.5/weather");

    //set up the request to post
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    //the additional query data to enter
    QUrlQuery query(url);
    query.addQueryItem("APPID","291cc02999b830ce1d7bf176d3d49172"); //our CS3307 APPID
    query.addQueryItem("q", _townList.at(townNum) + ",can");
    query.addQueryItem("units", "metric");
    url.setQuery(query);

    //update the request with the new query information.
    request.setUrl(url);

    //post the request
    manager->post(request, url.toEncoded());

    //connect the "finished" signal from the manager to our response handling method
    QObject::connect(manager, SIGNAL(finished(QNetworkReply *)), this, SLOT(replyFinished(QNetworkReply *)));
}

/*Name: replyFinished
 *Description: Slot function called when query has been received, updates weather attributes
 *Parameter Description: QNetworkReply object storing all retrieved weather data
 */
void WeatherModel::replyFinished(QNetworkReply * reply){
    if ( reply->error() != QNetworkReply::NoError ){
        // A communication error has occurred
        return;
    }

    //We read the JSON response into a QJsonObject
    QJsonObject obj = QJsonDocument::fromJson(reply->readAll()).object();

    //We find subsections of the QJsonObject
    QJsonObject main = obj["main"].toObject();
    QJsonObject wind = obj["wind"].toObject();
    QJsonObject sys = obj["sys"].toObject();

    //Retrieving weather information from QJson objects
    _temp = main["temp"].toDouble();
    _humidity = main["humidity"].toDouble();
    _pressure = main["pressure"].toDouble();
    _windSpeed = wind["speed"].toDouble();
    _windDir = wind["deg"].toDouble();
    _sunrise = (int)sys["sunrise"].toDouble();
    _sunset = (int)sys["sunset"].toDouble();
    emit updateUI();    //Emits signal to MainWindow that query has returned
}
