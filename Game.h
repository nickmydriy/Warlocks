#include "WMath.h"
#include "Warlock.h"
#include "GameWindow.h"
#include "Player.h"
#include "Bot.h"
#include "Settings.h"

#define WARLOCK_SPEED 5.0
#define WARLOCK_ROTATION_SPEED 6.0
#define FIREBALL_SPEED 12.0
#define MAGMA_SPEED 0.2
#define MAGMA_DAMAGE 8
#define FIREBALL_DAMAGE 5

class Game{
public:
    /**
     * Constuct game.
     * Opened game window
     */
    Game();
    /**
     * Destroy game
     */
    ~Game();
    /**
     * Starts game
     */
    bool gameProcess();
private:
    GameWindow *gameWindow;
    Warlock *warlock;
    Model *warlockModel, *barrModel, *fieldModel, *magmaModel, *fireBallModel, *hpBarModel;
    int barrsCount, botCount;
    bool fullScreen;
    float magmaRadius;
    std::vector<std::pair<wsm::vec2, float>> barrs;
    std::vector<Warlock*> enemyWarlocks;
    std::vector<Bot> bots;
    Player *player;
    /**
     * Resets game
     * \param addBots Count of bots to add
     * \param newBarrCount Count of barrs to add on game field, default -1 dont changed barrs count
     */
    void resetGame(int addBots, int newBarrCount  = -1);
    /*
     * Game logic
     * \param proctime Time of current game frame
     */
    void gameLogic(double proctime);
    /**
     * Draws game frame
     */
    void gameDraw();
    /**
     * Paused game
     * \param isResumePossible true if player warlock stil alive, else false
     * \param message Message to show
     * \return 0 - close window, 1 - continue game, 2 - need to reload game
     */
    int onPause(bool isResumePossible, std::string message);
    /**
     * Destroys game and all elements
     */
    void destroyGame();
    /**
     * Check clashes
     * \param warlock1 First warlock
     * \param warlock2 Second warlock
     * \param proctime Time of current game frame
     */
    void clashCheck(Warlock *warlock1, Warlock *warlock2, double proctime);
    /**
     * Check clashes warlocks fireball and barr
     * \param warlock Warlock
     */
    void barrClashCheck(Warlock *warlock);
    /**
     * Check clashes fireball and warlocks
     * \param thisWarlock Warlock whos fireball check
     * \param enemyWarlock Warlock to check clash with fireball
     */
    void fireClashCheck(Warlock *thisWarlock, Warlock *enemyWarlock);
    /**
     * Check clash warlock and magma
     * \param warlock Warlock to check
     * \param proctime Time of current game frame
     */
    void magmaClashCheck(Warlock *warlock, double proctime);
    /**
     * Check healthpoint of all warlocks
     * \return true if game over
     */
    bool gameOverCheck();
    /**
     * Relocates warlock
     * \param warlock Warlock to relocate
     * \param proctime Time of current game frame
     */
    void relocate(Warlock *warlock, double proctime);
    /**
     * Updates animations
     * \param proctime Time of current game frame
     */
    void animationUpdate(double proctime);
    /**
     * Opens a settings menu
     * \return true if game need to reload
     */
    bool onSettings();
    /**
     * Gets settings from file
     * \return [0] - window width, [1] - window height, [2] - barrs count, [3] - is fullscreen?
     */
    std::vector<int> getSettings();
    /**
     * Opens a start menu
     * \return false to request to close window
     */
    bool onStart();
    /**
     * Updates records
     */
    void recordsUpdate();
    /**
     * Gets records from file
     * \return string with record to show
     */
    std::string getRecords();
};