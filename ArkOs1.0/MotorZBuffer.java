import java.awt.*;
import javax.swing.*;

public class MotorZBuffer extends JPanel {

    private static final int WIDTH = 800;
    private static final int HEIGHT = 600;

    private final Color[][] colorBuffer = new Color[WIDTH][HEIGHT];
    private final float[][] depthBuffer = new float[WIDTH][HEIGHT];

    private float zCyan = 0.4f;
    private float zMagenta = 0.6f;

    public MotorZBuffer() {
        actualizarEscena();
    }

    private void actualizarEscena() {

        for (int x = 0; x < WIDTH; x++) {
            for (int y = 0; y < HEIGHT; y++) {
                colorBuffer[x][y] = Color.DARK_GRAY;
                depthBuffer[x][y] = 1.0f;
            }
        }

        pintarCuadrado(250, 200, 200, zMagenta, Color.MAGENTA);
        pintarCuadrado(100, 100, 200, zCyan, Color.CYAN);

        repaint();
    }

    private int borde(int px, int py, int ax, int ay, int bx, int by) {
        return (px - ax) * (by - ay) - (py - ay) * (bx - ax);
    }

    private void pintarCuadrado(int x, int y, int lado, float z, Color color) {

        int x1 = x;
        int y1 = y;

        int x2 = x;
        int y2 = y + lado;

        int x3 = x + lado;
        int y3 = y + lado;

        int x4 = x + lado;
        int y4 = y;

        int inicioX = Math.max(0, x);
        int finX = Math.min(WIDTH, x + lado);

        int inicioY = Math.max(0, y);
        int finY = Math.min(HEIGHT, y + lado);

        for (int i = inicioX; i < finX; i++) {
            for (int j = inicioY; j < finY; j++) {

                int e1 = borde(i, j, x1, y1, x2, y2);
                int e2 = borde(i, j, x2, y2, x3, y3);
                int e3 = borde(i, j, x3, y3, x4, y4);
                int e4 = borde(i, j, x4, y4, x1, y1);

                if (e1 >= 0 && e2 >= 0 && e3 >= 0 && e4 >= 0) {

                    if (z < depthBuffer[i][j]) {
                        depthBuffer[i][j] = z;
                        colorBuffer[i][j] = color;
                    }
                }
            }
        }
    }

    @Override
    protected void paintComponent(Graphics g) {
        super.paintComponent(g);

        for (int x = 0; x < WIDTH; x++) {
            for (int y = 0; y < HEIGHT; y++) {
                g.setColor(colorBuffer[x][y]);
                g.fillRect(x, y, 1, 1);
            }
        }
    }

    public static void main(String[] args) {

        JFrame frame = new JFrame("Z-Buffer");

        MotorZBuffer panel = new MotorZBuffer();

        JPanel menu = new JPanel();
        menu.setPreferredSize(new Dimension(200, HEIGHT));
        menu.setLayout(new GridLayout(4, 1, 5, 5));

        JLabel l1 = new JLabel("Profundidad Cian");
        JLabel l2 = new JLabel("Profundidad Magenta");

        JSlider cyan = new JSlider(10, 90, 40);
        JSlider magenta = new JSlider(10, 90, 60);

        cyan.addChangeListener(e -> {
            panel.zCyan = cyan.getValue() / 100f;
            panel.actualizarEscena();
        });

        magenta.addChangeListener(e -> {
            panel.zMagenta = magenta.getValue() / 100f;
            panel.actualizarEscena();
        });

        menu.add(l1);
        menu.add(cyan);
        menu.add(l2);
        menu.add(magenta);

        frame.setLayout(new BorderLayout());
        frame.add(panel, BorderLayout.CENTER);
        frame.add(menu, BorderLayout.EAST);

        frame.setSize(WIDTH + 200, HEIGHT);
        frame.setLocationRelativeTo(null);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setVisible(true);
    }
}