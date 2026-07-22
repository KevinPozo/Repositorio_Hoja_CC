import java.awt.Color;
import java.awt.Graphics2D;
import java.awt.image.BufferedImage;
import java.io.File;
import javax.imageio.ImageIO;

public class OperacionesImagenes {

    public static void main(String[] args) {

        try {

            BufferedImage imagenA = ImageIO.read(new File("src/Imagenes/Minion.jpg"));
            BufferedImage imagenB = ImageIO.read(new File("src/Imagenes/perro.jpg"));

            BufferedImage mascara = generarMascara(imagenA.getWidth(), imagenA.getHeight());

            BufferedImage imagenStencil = realizarStencil(imagenA, mascara);
            ImageIO.write(imagenStencil, "jpg", new File("src/Imagenes/Stencil.jpg"));

            BufferedImage imagenEscalada = new BufferedImage(
                    imagenStencil.getWidth(),
                    imagenStencil.getHeight(),
                    BufferedImage.TYPE_INT_RGB);

            Graphics2D grafico = imagenEscalada.createGraphics();
            grafico.drawImage(imagenB, 0, 0,
                    imagenStencil.getWidth(),
                    imagenStencil.getHeight(),
                    null);
            grafico.dispose();

            BufferedImage imagenBlend = mezclarImagenes(imagenStencil, imagenEscalada, 0.6f);
            ImageIO.write(imagenBlend, "jpg", new File("src/Imagenes/Blending.jpg"));

            BufferedImage resultado = aplicarOperacionXOR(imagenBlend, imagenEscalada);
            ImageIO.write(resultado, "jpg", new File("src/Imagenes/ResultadoFinal.jpg"));

            System.out.println("Imágenes generadas correctamente.");

        } catch (Exception e) {
            System.out.println(e.getMessage());
        }
    }

    public static BufferedImage generarMascara(int ancho, int alto) {

        BufferedImage mascara = new BufferedImage(ancho, alto, BufferedImage.TYPE_INT_RGB);

        Graphics2D g = mascara.createGraphics();

        g.setColor(Color.BLACK);
        g.fillRect(0, 0, ancho, alto);

        g.setColor(Color.WHITE);
        g.fillRect(ancho / 4, alto / 4, ancho / 2, alto / 2);

        g.dispose();

        return mascara;
    }

    public static BufferedImage realizarStencil(BufferedImage imagen, BufferedImage mascara) {

        int ancho = imagen.getWidth();
        int alto = imagen.getHeight();

        BufferedImage salida = new BufferedImage(ancho, alto, BufferedImage.TYPE_INT_RGB);

        for (int y = 0; y < alto; y++) {

            for (int x = 0; x < ancho; x++) {

                if (mascara.getRGB(x, y) == Color.WHITE.getRGB()) {
                    salida.setRGB(x, y, imagen.getRGB(x, y));
                } else {
                    salida.setRGB(x, y, Color.BLACK.getRGB());
                }

            }
        }

        return salida;
    }

    public static BufferedImage mezclarImagenes(BufferedImage fondo,
                                                BufferedImage frente,
                                                float alpha) {

        int ancho = fondo.getWidth();
        int alto = fondo.getHeight();

        BufferedImage salida = new BufferedImage(ancho, alto, BufferedImage.TYPE_INT_RGB);

        for (int y = 0; y < alto; y++) {

            for (int x = 0; x < ancho; x++) {

                int color1 = fondo.getRGB(x, y);
                int color2 = frente.getRGB(x, y);

                int r1 = (color1 >> 16) & 255;
                int g1 = (color1 >> 8) & 255;
                int b1 = color1 & 255;

                int r2 = (color2 >> 16) & 255;
                int g2 = (color2 >> 8) & 255;
                int b2 = color2 & 255;

                int r = (int) (r2 * alpha + r1 * (1 - alpha));
                int g = (int) (g2 * alpha + g1 * (1 - alpha));
                int b = (int) (b2 * alpha + b1 * (1 - alpha));

                salida.setRGB(x, y, (r << 16) | (g << 8) | b);

            }
        }

        return salida;
    }

    public static BufferedImage aplicarOperacionXOR(BufferedImage imagen1,
                                                    BufferedImage imagen2) {

        int ancho = imagen1.getWidth();
        int alto = imagen1.getHeight();

        BufferedImage salida = new BufferedImage(ancho, alto, BufferedImage.TYPE_INT_RGB);

        for (int y = 0; y < alto; y++) {

            for (int x = 0; x < ancho; x++) {

                int p1 = imagen1.getRGB(x, y);
                int p2 = imagen2.getRGB(x, y);

                int r = ((p1 >> 16) & 255) ^ ((p2 >> 16) & 255);
                int g = ((p1 >> 8) & 255) ^ ((p2 >> 8) & 255);
                int b = (p1 & 255) ^ (p2 & 255);

                salida.setRGB(x, y, (r << 16) | (g << 8) | b);

            }
        }

        return salida;
    }
}