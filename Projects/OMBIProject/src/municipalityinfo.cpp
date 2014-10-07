#include "municipalityinfo.h"

#include <string>
#include <QString>

using namespace std;

MunicipalityInfo::MunicipalityInfo(QString name, QString abbreviation,
                                   QString geographicArea, int population,
                                   int populationDensity, int houseHoldsNumber)
{
 QString _name = name;
 QString _abbreviation = abbreviation;
 QString _geographicArea = geographicArea;
 int _population= population;
 int _populationDensity= populationDensity;
 int _houseHoldsNumber= houseHoldsNumber;
}

QString MunicipalityInfo::getName()
{
    return _name;
}


QString MunicipalityInfo::getMunicipalAbbreviation()
{
   return _abbreviation;
}

QString MunicipalityInfo::getGeographicArea()
{
    return _geographicArea;
}


int MunicipalityInfo::getPopulation()
{
    return _population;
}

int MunicipalityInfo::getPopulationDensity()
{
    return _populationDensity;

}

int MunicipalityInfo::getHouseHoldsNumber()
{
    return _houseHoldsNumber;
}

MunicipalityInfo::~MunicipalityInfo()
{

}
