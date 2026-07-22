import javax.imageio.ImageIO;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;

public class FiltroDiaNoche {

    public static void main(String[] args) {

        try {

            BufferedImage imagenOriginal = ImageIO.read(new File("src/imagenes/taza.jpg"));

            int ancho = imagenOriginal.getWidth();
            int alto = imagenOriginal.getHeight();

            float[] intensidad = {1.0f, 0.75f, 0.50f, 0.25f};

            for (float factor : intensidad) {

                float[] rojo = new float[ancho * alto];
                float[] verde = new float[ancho * alto];
                float[] azul = new float[ancho * alto];

                for (int y = 0; y < alto; y++) {
                    for (int x = 0; x < ancho; x++) {

                        int posicion = y * ancho + x;

                        int pixel = imagenOriginal.getRGB(x, y);

                        rojo[posicion] = (pixel >> 16) & 255;
                        verde[posicion] = (pixel >> 8) & 255;
                        azul[posicion] = pixel & 255;
                    }
                }

                for (int i = 0; i < rojo.length; i++) {

                    rojo[i] *= factor;
                    verde[i] *= factor;
                    azul[i] *= factor;

                }

                BufferedImage salida = new BufferedImage(ancho, alto, BufferedImage.TYPE_INT_RGB);

                for (int y = 0; y < alto; y++) {
                    for (int x = 0; x < ancho; x++) {

                        int posicion = y * ancho + x;

                        int r = Math.clamp((int) rojo[posicion], 0, 255);
                        int g = Math.clamp((int) verde[posicion], 0, 255);
                        int b = Math.clamp((int) azul[posicion], 0, 255);

                        salida.setRGB(x, y, (r << 16) | (g << 8) | b);
                    }
                }

                String archivo = "src/imagenes/taza_factor_" + factor + ".png";

                ImageIO.write(salida, "png", new File(archivo));

                System.out.println("Generada: " + archivo);
            }

        } catch (IOException e) {
            System.out.println(e.getMessage());
        }
    }
}