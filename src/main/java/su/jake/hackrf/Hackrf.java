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

        result = Hackrf.setFrequency(850000000);
        System.out.println(String.format("hackrf setFrequency: %d", result));

        result = Hackrf.setSampleRate(1000000);
        System.out.println(String.format("hackrf setSampleRate: %d", result));

        result = Hackrf.stopRx();
        System.out.println(String.format("hackrf stopRx: %d", result));

        result = Hackrf.startRx(new RxCallback() {
            @Override
            public void onData(byte[] data) {
                float[] floats = new float[data.length];
                System.out.print("{");
                for (int i=0; i<data.length; i++) {
                    floats[i] = ((float)(data[i] & 0xff)) / 255f;
                    System.out.print(String.format("%.2f, ", floats[i]));
                }
                System.out.print("}\n\n");
                System.out.println("onData()");
            }
        });
        System.out.println(String.format("hackrf startRx: %d", result));

        try {
            Thread.sleep(4000L);
        } catch (InterruptedException ie) {
            // whatevah
        }
        result = Hackrf.stopRx();
        System.out.println(String.format("hackrf stopRx: %d", result));

        result = Hackrf.close();
        System.out.println(String.format("hackrf close: %d", result));
    }
}
