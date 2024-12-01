// 根据包名在AndroidManifest.xml中指定Activity的name路径（=包名.类名）
package org.qtproject.example.SharkBookkeepingBook;
import org.qtproject.qt5.android.bindings.QtActivity;
import android.view.WindowManager;
import android.os.Bundle;
import android.util.Log;
import android.content.Intent;
import android.widget.Toast;
import android.os.Handler;
import android.os.Message;
import android.content.Context;
import android.os.Build;
import android.os.Vibrator;
import android.app.PendingIntent;
import android.graphics.Color;
import android.view.View;
import android.view.Window;

public class MainActivity extends QtActivity {
   private final static String TAG = "QtFullscreen";
   private static MainActivity m_instance;
        private static Handler m_handler = new Handler() {
             @Override
             public void handleMessage(Message msg) {
                 switch (msg.what) {
                 case 1:
                     Toast toast = Toast.makeText(m_instance,(String)msg.obj, Toast.LENGTH_SHORT);
                     toast.show();
                     break;
                 };
             }
         };
        @Override
	// 重写 onCreate 方法
        public void onCreate(Bundle savedInstanceState) {
              super.onCreate(savedInstanceState);
              // 设置全屏并且状态栏悬浮程序之上
              // getWindow().addFlags(WindowManager.LayoutParams.FLAG_TRANSLUCENT_STATUS);
              // getWindow().getDecorView().setFitsSystemWindows(true);
              Window window = getWindow();
              // 移除透明状态栏标志
              window.clearFlags(WindowManager.LayoutParams.FLAG_TRANSLUCENT_STATUS);
              // 设置系统 UI 可见性标志，让布局适应系统窗口
              window.getDecorView().setSystemUiVisibility(View.SYSTEM_UI_FLAG_LAYOUT_STABLE);
              // 可以设置状态栏颜色
              window.setStatusBarColor(Color.parseColor("#F9DB61"));

        }
         public MainActivity(){
                 m_instance = this;
             }
         public static void makeToast(String s){
             m_handler.sendMessage(m_handler.obtainMessage(1, s));
         }
        public void vibrate(long milliseconds) {
            Vibrator vibrator = (Vibrator) getSystemService(Context.VIBRATOR_SERVICE);
            vibrator.vibrate(milliseconds);
        }
}