//////////////////// MainScreen ////////////////////

/**
 *  MainScreen class is the main service selection
 *  screen for the OMBI project.
 *  @author Vince Marchese
 **/

#ifndef MAINSCREEN_H
#define MAINSCREEN_H

#include <QWidget>
#include <QRadioButton>
#include <QPushButton>
#include <QComboBox>
#include <QMovie>

namespace Ui {
    class MainScreen;
}

class MainScreen : public QWidget {
    Q_OBJECT

public:
    /**
     * @brief   Constructor.
     */
    explicit MainScreen(QWidget *parent = 0);

    /**
     * @brief Destructor.
     */
    ~MainScreen();

    /**
     * @brief   Gets the Service Area Selector.
     * @return  A pointer to the QComboBox.
     */
    QComboBox* getServiceSelector();

    /**
     * @brief   Gets the Fire Service Area Button.
     * @return  A pointer to the QPushButton.
     */
    QPushButton* getFireButton();

    /**
     * @brief   Gets the Library Service Area Button.
     * @return  A pointer to the QPushButton.
     */
    QPushButton* getLibraryButton();

    /**
     * @brief   Gets the Parks Service Area Button.
     * @return  A pointer to the QPushButton.
     */
    QPushButton* getParksButton();

    /**
     * @brief   Gets the Police Service Area Button.
     * @return  A pointer to the QPushButton.
     */
    QPushButton* getPoliceButton();

    /**
     * @brief   Gets the Waste Service Area Button.
     * @return  A pointer to the QPushButton.
     */
    QPushButton* getWasteButton();

    /**
     * @brief   Gets the Water Service Area Button.
     * @return  A pointer to the QPushButton.
     */
    QPushButton* getWaterButton();

    /**
     * @brief   Gets the Table Button.
     * @return  A pointer to the QRadioButton.
     */
    QRadioButton* getTableRadio();

    /**
     * @brief   Gets the Bar Chart Button.
     * @return  A pointer to the QRadioButton.
     */
    QRadioButton* getBarRadio();

    /**
     * @brief   Gets the Line Graph Button.
     * @return  A pointer to the QRadioButton.
     */
    QRadioButton* getLineRadio();

    /**
     * @brief   Gets the Display Button.
     * @return  A pointer to the QPushButton.
     */
    QPushButton* getDisplayButton();

    /**
     * @brief   Gets the Display Chart Type.
     * @return  1 if Bar Chart, 2 if Line Graph, 3 if Table.
     */
    int getSelectedRadio();

private:
    /**
     * A pointer to the UI.
     */
    Ui::MainScreen *ui;

    /**
     * A pointer to the Service Area Selector.
     */
    QComboBox *_serviceSelection;

    /**
     * A pointer to the Fire Service Area Button.
     */
    QPushButton *_fireButton;

    /**
     * A pointer to the Library Service Area Button.
     */
    QPushButton *_libraryButton;

    /**
     * A pointer to the Parks Service Area Button.
     */
    QPushButton *_parksButton;

    /**
     * A pointer to the Police Service Area Button.
     */
    QPushButton *_policeButton;

    /**
     * A pointer to the Waste Service Area Button.
     */
    QPushButton *_wasteButton;

    /**
     * A pointer to the Water Service Area Button.
     */
    QPushButton *_waterButton;

    /**
     * A pointer to the Table Radio Button.
     */
    QRadioButton *_tableRadio;

    /**
     * A pointer to the Bar Chart Radio Button.
     */
    QRadioButton *_barRadio;

    /**
     * A pointer to the Line Graph Radio Button.
     */
    QRadioButton *_lineRadio;

    /**
     * A pointer to the Display Button.
     */
    QPushButton *_displayButton;
};

#endif // MAINSCREEN_H
