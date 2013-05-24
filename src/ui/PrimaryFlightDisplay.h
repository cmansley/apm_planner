#ifndef PRIMARYFLIGHTDISPLAY_H
#define PRIMARYFLIGHTDISPLAY_H

#include <QWidget>
#include <QPen>
#include "UASInterface.h"

#define LINEWIDTH 0.0032f

//#define TAPES_TEXT_SIZE 0.028
//#define AI_TEXT_SIZE 0.040
//#define AI_TEXT_MIN_PIXELS 12
//#define AI_TEXT_MAX_PIXELS 36
//#define PANELS_TEXT_SIZE 0.030
//#define COMPASS_SCALE_TEXT_SIZE 0.16

#define SMALL_TEXT_SIZE 0.03f
#define MEDIUM_TEXT_SIZE (SMALL_TEXT_SIZE*1.2f)
#define LARGE_TEXT_SIZE (MEDIUM_TEXT_SIZE*1.2f)

#define SHOW_ZERO_ON_SCALES true

// all in units of display height
#define ROLL_SCALE_RADIUS 0.42f
#define ROLL_SCALE_TICKMARKLENGTH 0.04f
#define ROLL_SCALE_MARKERWIDTH 0.06f
#define ROLL_SCALE_MARKERHEIGHT 0.04f
// scale max. degrees
#define ROLL_SCALE_RANGE 60

// fraction of height to translate for each degree of pitch.
#define PITCHTRANSLATION 65.0
// 10 degrees for each line
#define PITCH_SCALE_RESOLUTION 5
#define PITCH_SCALE_MAJORLENGTH 0.08
#define PITCH_SCALE_MINORLENGTH 0.04
#define PITCH_SCALE_HALFRANGE 20

// The number of degrees to either side of the heading to draw the compass disk.
// 180 is valid, this will draw a complete disk. If the disk is partly clipped
// away, less will do.

#define COMPASS_DISK_MAJORTICK 10
#define COMPASS_DISK_ARROWTICK 45
#define COMPASS_DISK_MAJORLINEWIDTH 0.006
#define COMPASS_DISK_MINORLINEWIDTH 0.004
#define COMPASS_DISK_SPAN 180
#define COMPASS_DISK_RESOLUTION 15
#define COMPASS_DISK_MARKERWIDTH 0.2
#define COMPASS_DISK_MARKERHEIGHT 0.133


#define ALTIMETER_VVI_SPAN 10
#define ALTIMETER_VVI_LOGARITHMIC true

// The altitude difference between top and bottom of scale
#define ALTIMETER_LINEAR_SPAN 35
// every 5 meters there is a tick mark
#define ALTIMETER_LINEAR_RESOLUTION 5
// every 10 meters there is a number
#define ALTIMETER_LINEAR_MAJOR_RESOLUTION 10
// min. and max. vertical velocity

// The altitude difference between top and bottom of scale
#define ALTIMETER_PROJECTED_SPAN 50
// every 5 meters there is a tick mark
#define ALTIMETER_PROJECTED_RESOLUTION 5
// every 10 meters there is a number
#define ALTIMETER_PROJECTED_MAJOR_RESOLUTION 10
// min. and max. vertical velocity
//#define ALTIMETER_PROJECTED

#define UNKNOWN_BATTERY -1

class PrimaryFlightDisplay : public QWidget
{
    Q_OBJECT
public:
    PrimaryFlightDisplay(int width = 640, int height = 480, QWidget* parent = NULL);
    ~PrimaryFlightDisplay();

public slots:
    /** @brief Attitude from main autopilot / system state */
    void updateAttitude(UASInterface* uas, double roll, double pitch, double yaw, quint64 timestamp);
    /** @brief Attitude from one specific component / redundant autopilot */
    void updateAttitude(UASInterface* uas, int component, double roll, double pitch, double yaw, quint64 timestamp);
//    void updateAttitudeThrustSetPoint(UASInterface*, double rollDesired, double pitchDesired, double yawDesired, double thrustDesired, quint64 usec);
    void updateBattery(UASInterface*, double, double, double, int);
    void receiveHeartbeat(UASInterface*);
    void updateThrust(UASInterface*, double);
    void updateLocalPosition(UASInterface*,double,double,double,quint64);
    void updateGlobalPosition(UASInterface*,double,double,double,quint64);
    void updateSpeed(UASInterface*,double,double,double,quint64);
    void updateState(UASInterface*,QString);
    void updateMode(int id,QString mode, QString description);
    void updateLoad(UASInterface*, double);
    void selectWaypoint(int uasId, int id);

protected:
    enum Layout {
        FEATUREPANELS_IN_CORNERS,       // For a wide and low container.
        FEATUREPANELS_AT_BOTTOM,        // For higher container.
        COMPASS_SEPARATED               // For a very high container. Feature panels are at bottom.
    };

    enum Style {
        OPAGUE_TAPES,                   // Hzon not visible through tapes nor through feature panels. Frames with margins between.
        TRANSLUCENT_TAPES               // Hzon visible through tapes and (frameless) feature panels.
    };

    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *e);

    // from HUD.h:

    /** @brief Preferred Size */
    QSize sizeHint() const;
    /** @brief Start updating widget */
    void showEvent(QShowEvent* event);
    /** @brief Stop updating widget */
    void hideEvent(QHideEvent* event);

    // dongfang: We have no context menu. Viewonly.
    // void contextMenuEvent (QContextMenuEvent* event);

protected:
    // dongfang: What is that?
    // dongfang: OK it's for UI interaction. Presently, there is none.
    void createActions();

public slots:
    /** @brief Set the currently monitored UAS */
    virtual void setActiveUAS(UASInterface* uas);

protected slots:

signals:
    void visibilityChanged(bool visible);

private:
    //void prepareTransform(QPainter& painter, qreal width, qreal height);
    //void transformToGlobalSystem(QPainter& painter, qreal width, qreal height, float roll, float pitch);

    void drawTextCenter(QPainter& painter, QString text, float fontSize, float x, float y);
    void drawTextLeftCenter(QPainter& painter, QString text, float fontSize, float x, float y);
    void drawTextRightCenter(QPainter& painter, QString text, float fontSize, float x, float y);
    void drawTextCenterBottom(QPainter& painter, QString text, float fontSize, float x, float y);
    void drawTextCenterTop(QPainter& painter, QString text, float fontSize, float x, float y);
    void drawAIGlobalFeatures(QPainter& painter, QRectF area);
    void drawAIAirframeFixedFeatures(QPainter& painter, QRectF area);
    void drawPitchScale(QPainter& painter, QRectF area, bool drawNumbersLeft, bool drawNumbersRight);
    void drawRollScale(QPainter& painter, QRectF area, bool drawTicks, bool drawNumbers);
    void drawAIAttitudeScales(QPainter& painter, QRectF area);
    void drawAICompassDisk(QPainter& painter, QRectF area);
    void drawSeparateCompassDisk(QPainter& painter, QRectF area);

    void drawAltimeter(QPainter& painter, QRectF area, float altitude, float maxAltitude, float vv);

    void fillInstrumentBackground(QPainter& painter, QRectF edge);
    void fillInstrumentOpagueBackground(QPainter& painter, QRectF edge);
    void drawInstrumentBackground(QPainter& painter, QRectF edge);

    void drawLinkStatsPanel(QPainter& painter, QRectF area);
    void drawSysStatsPanel(QPainter& painter, QRectF area);
    void drawMissionStatsPanel(QPainter& painter, QRectF area);
    void drawSensorsStatsPanel(QPainter& painter, QRectF area);

    void doPaint();

    float roll;
    float pitch;
    float heading;

    // APM: GPS and baro mix. From the GLOBAL_POSITION_INT or VFR_HUD messages.
    float aboveASLAltitude;
    float GPSAltitude;

    // APM: GPS and baro mix above home (GPS) altitude. This value comes from the GLOBAL_POSITION_INT message.
    // Do !!!NOT!!! ever do altitude calculations at the ground station. There are enough pitfalls already.
    // The MP "set home altitude" button will not be repeated here if it did that.
    float aboveHomeAltitude;

    float groundSpeed;
    float airSpeed;
    float verticalVelocity;

    QString mode;
    QString state;
    float load;         //

    Layout layout;      // The display layout.
    Style style;        // The AI style (tapes translusent or opague)

    /* This idea did not work
    QPen whitePen;
    QPen redPen;
    QPen amberPen;
    QPen greenPen;
    QPen blackPen;
    */

    QColor redColor;
    QColor amberColor;
    QColor greenColor;

    qreal lineWidth;
    qreal fineLineWidth;

    qreal smallTestSize;
    qreal mediumTextSize;
    qreal largeTextSize;

    // Globally used stuff only.
    QPen instrumentEdgePen;
    QBrush instrumentBackground;
    QBrush instrumentOpagueBackground;

    QFont font;

    QTimer* refreshTimer;       ///< The main timer, controls the update rate

    UASInterface* uas;          ///< The uas currently monitored

    //QString energyStatus; ///< Current fuel level / battery voltage
    double batteryVoltage;
    double batteryCurrent;
    double batteryCharge;

    static const int tickValues[];
    static const QString compassWindNames[];

    static const int updateInterval = 40;


signals:
    
public slots:
    
};

#endif // PRIMARYFLIGHTDISPLAY_H