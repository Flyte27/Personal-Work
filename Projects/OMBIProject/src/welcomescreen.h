//////////////////// WelcomeScreen ////////////////////

/**
 *  WelcomeScreen class is the welcome screen for
 *  the OMBI project.
 *  @author Vince Marchese
 **/

#ifndef WELCOMESCREEN_H
#define WELCOMESCREEN_H

#include <QWidget>
#include <QPushButton>
#include <QMovie>

namespace Ui {
    class WelcomeScreen;
}

class WelcomeScreen : public QWidget {
    Q_OBJECT

public:
    /**
     * @brief   Constructor.
     */
    explicit WelcomeScreen(QWidget *parent = 0);

    /**
     * @brief   Destructor.
     */
    ~WelcomeScreen();

    /**
     * @brief   Gets the Start Button.
     * @return  A pointer to the QPushButton.
     */
    QPushButton* getStartButton();

private:
    /**
     * A pointer to the UI.
     */
    Ui::WelcomeScreen *ui;

    /**
     * A pointer to the Start Button.
     */
    QPushButton *_start;

private slots:
    /**
     * @brief   Shows/hides the information on how to get started.
     */
    void toggleInfo();
};

#endif // WELCOMESCREEN_H
