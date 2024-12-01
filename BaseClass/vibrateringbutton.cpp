#include "vibrateringbutton.h"
#include <QAndroidJniObject>
#include <QtAndroidExtras/QAndroidJniEnvironment>
#include "Kit/LogInfo/clog.h"

VibrateRingButton::VibrateRingButton(QObject * parent)
{
    Q_UNUSED(parent);
    connect(this,&VibrateRingButton::clicked,this,&VibrateRingButton::onClicked);
}

void VibrateRingButton::onClicked(bool checked)
{
    Q_UNUSED(checked);
    LOG("VibrateRingButton::onClicked");
    vibrate(m_vibrateMilliseconds);
}

void VibrateRingButton::vibrate(long milliseconds)
{
//    QAndroidJniObject vibrator = QAndroidJniObject::callStaticObjectMethod("android/os/Vibrator", "getSystemService", "(Ljava/lang/String;)Ljava/lang/Object;",QAndroidJniObject::fromString("vibrator").object<jobject>());
//    if (vibrator.isValid()){
//        QAndroidJniEnvironment env;
//        jclass longClass = env->FindClass("java/lang/Long");
//        jobjectArray argArray = env->NewObjectArray(1, longClass, QAndroidJniObject::fromString(QString::number(milliseconds)).object<jobject>());
//        vibrator.callMethod<void>("vibrate", "(J)V", argArray);
//    }

    QAndroidJniEnvironment env;
    QAndroidJniObject activity = QAndroidJniObject::callStaticObjectMethod("org/qtproject/qt5/android/QtNative", "activity", "()Landroid/app/Activity;");
    if (!activity.isValid()) {
        LOG("Failed to get valid Activity object");
        return;
    }
    QAndroidJniObject name = QAndroidJniObject::getStaticObjectField("android/content/Context", "VIBRATOR_SERVICE", "Ljava/lang/String;");
    QAndroidJniObject vibrateService = activity.callObjectMethod("getSystemService", "(Ljava/lang/String;)Ljava/lang/Object;", name.object<jstring>());
    jlong duration = 50;
    vibrateService.callMethod<void>("vibrate", "(J)V", duration);


//    QAndroidJniObject javaToast = QAndroidJniObject::fromString("QT调用Android原生Toast");
//    QAndroidJniObject::callStaticMethod<void>("org/qtproject/example/SharkBookkeepingBook/MainActivity",
//                                                      "makeToast",
//                                                      "(Ljava/lang/String;)V",
//                                                      javaToast.object<jstring>());


//    QAndroidJniObject activity = QAndroidJniObject::callStaticObjectMethod("org/qtproject/qt5/android/QtNative", "activity", "()Landroid/app/Activity;");
//    if (!activity.isValid()) {
//            LOG("Failed to get valid Activity object");
//            return;
//    }
//    activity.callMethod<void>("vibrate", "(J)V", milliseconds);

}

void VibrateRingButton::ring(int milliseconds)
{

}
