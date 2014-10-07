/**
  * A class that stores the Information of each municipality.
  */

#ifndef MUNICIPALITYINFO_H
#define MUNICIPALITYINFO_H

#include <string>
#include <QString>

class MunicipalityInfo
{
public:
    /**
     * A public constructor
     */
    MunicipalityInfo(QString name, QString abbreviation,
                                       QString geographicArea, int population,
                                       int populationDensity, int houseHoldsNumber);

    /**
    * A destructor.
    */
    ~MunicipalityInfo();

    /**
     * A public getter:
     * @return the Name of the Municipality.
     */
    QString getName();

    /**
     * A public getter:
     * @return the Abbreviation of the Municipality.
     */
    QString getMunicipalAbbreviation();

    /**
     * A public getter:
     * @return the Geographic Area of the Municipality.
     */
    QString getGeographicArea();

    /**
     * A public getter:
     * @return the Population of the Municipality.
     */
    int getPopulation();

    /**
     * A public getter:
     * @return the number of households in the Municipality.
     */
    int getHouseHoldsNumber();

    /**
     * A public getter:
     * @return the Population Density of the Municipality.
     */
    int getPopulationDensity();


private:

    /**
     * A private member: Stores the name of the municipality.
     */
    QString _name;

    /**
     * A private member: Stores the abbreviation of the municipality.
     */
    QString _abbreviation;

    /**
     * A private member: Stores the Geographic Area of the municipality.
     */
    QString _geographicArea;

    /**
     * A private member: Stores the population Area of the municipality.
     */
    int _population;

    /**
     * A private member: Stores the number of households in the municipality.
     */
    int _houseHoldsNumber;

    /**
     * A private member: Stores the population density Area of the municipality.
     */
    int _populationDensity;

};

#endif // MUNICIPALITYINFO_H
