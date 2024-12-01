package org.qtproject.example.SharkBookkeepingBook;
import org.qtproject.qt5.android.bindings.QtApplication;
import android.app.Application;
import android.content.Context;
 
public class MyApplication extends QtApplication {
    private static Context mContext;
    @Override
    public void onCreate() {
        super.onCreate();
        // getApplicationContext();
        mContext = this;
    }
    public static Context getContext() {
        return mContext;
    }
}