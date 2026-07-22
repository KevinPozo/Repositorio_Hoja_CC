import com.jogamp.opengl.*;
import com.jogamp.opengl.glu.GLU;
import com.jogamp.opengl.util.texture.Texture;
import com.jogamp.opengl.util.texture.awt.AWTTextureIO;
import javax.imageio.ImageIO;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;

public class Motor3D implements GLEventListener {
    private GLU glu = new GLU();
    private Texture[] texturas = new Texture[3];
    
    private float[] posZ = { -5.0f, -3.5f, -2.0f };
    private float[] alpha = { 1.0f, 1.0f, 1.0f };
    private boolean depthTestEnabled = true;
    private boolean verProfundidad = false;

    public void setZ(int index, float z) { posZ[index] = z; }
    public void setAlpha(int index, float a) { alpha[index] = a; }
    public void setDepthTest(boolean enabled) { depthTestEnabled = enabled; }
    public void setVerProfundidad(boolean enabled) { verProfundidad = enabled; }

    @Override
    public void init(GLAutoDrawable drawable) {
        GL2 gl = drawable.getGL().getGL2();
        
        String[] archivos = {"Imagenes/img1.jpg", "Imagenes/img2.jpg", "Imagenes/img3.jpg"};
        for(int i = 0; i < 3; i++) {
            try {
                File f = new File(archivos[i]);
                if (f.exists()) {
                    BufferedImage img = ImageIO.read(f);
                    if (img != null) {
                        texturas[i] = AWTTextureIO.newTexture(GLProfile.get(GLProfile.GL2), img, true);
                    }
                }
            } catch (Exception e) {
                System.out.println("No se pudo cargar la imagen " + archivos[i]);
            }
        }

        gl.glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        gl.glEnable(GL2.GL_DEPTH_TEST);
        
        gl.glEnable(GL2.GL_BLEND);
        gl.glBlendFunc(GL2.GL_SRC_ALPHA, GL2.GL_ONE_MINUS_SRC_ALPHA);
    }

    @Override
    public void dispose(GLAutoDrawable drawable) {
        for (Texture t : texturas) {
            if (t != null) t.destroy(drawable.getGL());
        }
    }

    @Override
    public void display(GLAutoDrawable drawable) {
        GL2 gl = drawable.getGL().getGL2();

        if (depthTestEnabled) {
            gl.glEnable(GL2.GL_DEPTH_TEST);
        } else {
            gl.glDisable(GL2.GL_DEPTH_TEST);
        }

        gl.glClear(GL.GL_COLOR_BUFFER_BIT | GL.GL_DEPTH_BUFFER_BIT);
        gl.glLoadIdentity();

        gl.glTranslatef(0.0f, 0.0f, -4.0f);
        
        gl.glRotatef(25.0f, 0.0f, 1.0f, 0.0f);
        gl.glRotatef(15.0f, 1.0f, 0.0f, 0.0f);

        int[] orden = {0, 1, 2};
        for (int i = 0; i < 3; i++) {
            for (int j = i + 1; j < 3; j++) {
                if (posZ[orden[i]] > posZ[orden[j]]) {
                    int temp = orden[i];
                    orden[i] = orden[j];
                    orden[j] = temp;
                }
            }
        }

        for (int idx : orden) {
            dibujarPlano(gl, idx);
        }
    }

    private void dibujarPlano(GL2 gl, int idx) {
        gl.glPushMatrix();
        
        float offsetX = (idx - 1) * 0.5f; 
        float offsetY = (1 - idx) * 0.5f; 
        
        gl.glTranslatef(offsetX, offsetY, posZ[idx]);

        if (verProfundidad) {
            gl.glDisable(GL2.GL_TEXTURE_2D);
            float gris = (posZ[idx] + 10.0f) / 10.0f;
            if (gris < 0) gris = 0; if (gris > 1) gris = 1;
            gl.glColor4f(gris, gris, gris, alpha[idx]);
        } else {
            if (texturas[idx] != null) {
                gl.glColor4f(1f, 1f, 1f, alpha[idx]);
                gl.glEnable(GL2.GL_TEXTURE_2D);
                texturas[idx].bind(gl);
            } else {
                if (idx == 0) gl.glColor4f(1f, 0f, 0f, alpha[idx]);
                else if (idx == 1) gl.glColor4f(0f, 1f, 0f, alpha[idx]);
                else gl.glColor4f(0f, 0f, 1f, alpha[idx]);
                gl.glDisable(GL2.GL_TEXTURE_2D);
            }
        }

        gl.glBegin(GL2.GL_QUADS);
        
        float x1 = -1f, y1 = -1f;
        float x2 =  1f, y2 =  1f;

        float zIzq = 0f;
        float zDer = 0f;
        if (idx == 1 && !verProfundidad) {
            zIzq = 0.5f;
            zDer = -0.5f;
        }

        gl.glTexCoord2f(0f, 0f); gl.glVertex3f(x1, y1, zIzq);
        gl.glTexCoord2f(1f, 0f); gl.glVertex3f(x2, y1, zDer);
        gl.glTexCoord2f(1f, 1f); gl.glVertex3f(x2, y2, zDer);
        gl.glTexCoord2f(0f, 1f); gl.glVertex3f(x1, y2, zIzq);

        gl.glEnd();
        
        if (!verProfundidad && texturas[idx] != null) {
            gl.glDisable(GL2.GL_TEXTURE_2D);
        }

        gl.glPopMatrix();
    }

    @Override
    public void reshape(GLAutoDrawable drawable, int x, int y, int width, int height) {
        GL2 gl = drawable.getGL().getGL2();
        if (height <= 0) height = 1;
        float aspect = (float) width / height;

        gl.glMatrixMode(GL2.GL_PROJECTION);
        gl.glLoadIdentity();
        glu.gluPerspective(45.0f, aspect, 0.1f, 100.0f);
        
        gl.glMatrixMode(GL2.GL_MODELVIEW);
        gl.glLoadIdentity();
    }
}
