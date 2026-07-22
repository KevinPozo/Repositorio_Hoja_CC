import java.awt.*;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import javax.imageio.ImageIO;

public class Fragmentos {

    public static void main(String[] args) {

        try {

            BufferedImage paisaje = ImageIO.read(new File("imagenes/paisaje.png"));
            BufferedImage universoOriginal = ImageIO.read(new File("imagenes/universo.png"));

            int ancho = paisaje.getWidth();
            int alto = paisaje.getHeight();

            Image escala = universoOriginal.getScaledInstance(ancho, alto, Image.SCALE_FAST);
            BufferedImage universo = new BufferedImage(ancho, alto, BufferedImage.TYPE_INT_ARGB);

            Graphics2D g = universo.createGraphics();
            g.drawImage(escala, 0, 0, null);
            g.dispose();

            BufferedImage resultado = new BufferedImage(ancho, alto, BufferedImage.TYPE_INT_ARGB);

            float[][] bufferZ = new float[ancho][alto];

            for (int x = 0; x < ancho; x++) {
                for (int y = 0; y < alto; y++) {
                    bufferZ[x][y] = 9999.0f;
                }
            }

            int cx = ancho / 2;
            int cy = alto / 2;
            int radio = Math.min(ancho, alto) / 3;

            float zPaisaje = 10.0f;
            float zUniverso = 5.0f;

            for (int y = 0; y < alto; y++) {
                for (int x = 0; x < ancho; x++) {

                    if (zPaisaje < bufferZ[x][y]) {
                        bufferZ[x][y] = zPaisaje;
                        resultado.setRGB(x, y, paisaje.getRGB(x, y));
                    }

                    int px = x - cx;
                    int py = y - cy;

                    if (px * px + py * py <= radio * radio) {

                        Color color = new Color(universo.getRGB(x, y), true);
                        int brillo = (color.getRed() + color.getGreen() + color.getBlue()) / 3;

                        if (brillo > 128) {

                            if (zUniverso < bufferZ[x][y]) {
                                bufferZ[x][y] = zUniverso;
                                resultado.setRGB(x, y, universo.getRGB(x, y));
                            }
                        }
                    }
                }
            }

            ImageIO.write(resultado, "png", new File("imagenes/ResultadoTema8.png"));
            System.out.println("Proceso finalizado.");

        } catch (IOException e) {
            System.out.println("Error: " + e.getMessage());
        }
    }
}