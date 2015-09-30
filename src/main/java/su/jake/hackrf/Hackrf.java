package su.jake.hackrf;

public class Hackrf {
    static { System.loadLibrary("hackrf"); }
    public static native int open();
    public static native int close();
    public static native int setFrequency(int freqency);
    public static native int setSampleRate(int sampleRate);
    public static native int startRx(RxCallback callback);
    public static native int stopRx();

    public static void main(String[] args) {
        int result = Hackrf.open();
        System.out.println(String.format("hackrf open: %d", result));
        result = Hackrf.close();
        System.out.println(String.format("hackrf close: %d", result));
    }
}
