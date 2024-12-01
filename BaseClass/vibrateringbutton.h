#ifndef VIBRATERINGBUTTON_H
#define VIBRATERINGBUTTON_H
#include <QPushButton>

class VibrateRingButton : public QPushButton
{
    Q_OBJECT
    enum RingType{};
public:
    VibrateRingButton(QObject * parent = nullptr);
    void setEnableVibrate(bool enable) {m_enableVibrate = enable;}
    bool enableVibrate() {return m_enableVibrate;}
    void setEnableRing(bool enable) {m_enableRing = enable;}
    bool enableRing() {return m_enableRing;}
    void setVibrateMilliseconds(int milliseconds) {m_vibrateMilliseconds = milliseconds;}
    int VibrateMilliseconds() {return m_vibrateMilliseconds;}
    void setRingMilliseconds(int milliseconds) {m_ringMillseconds = milliseconds;}

private slots:
    void onClicked(bool);
private:
    void vibrate(long milliseconds);
    void ring(int milliseconds);

private:
    bool m_enableVibrate = false;
    bool m_enableRing = false;
    long m_vibrateMilliseconds = 1000;
    int m_ringMillseconds = 500;
};

#endif // VIBRATERINGBUTTON_H
