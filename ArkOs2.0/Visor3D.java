import com.jogamp.opengl.*;
import com.jogamp.opengl.awt.GLJPanel;
import javax.swing.*;
import java.awt.*;

public class Visor3D extends JFrame {
    private GLJPanel panelOpenGL;
    private Motor3D motor;

    private JSlider[] slidersZ;
    private JSlider[] slidersAlpha;
    private JCheckBox cbDepthTest;
    private JCheckBox cbVerProfundidad;

    public Visor3D() {
        super("Visor de Planos 3D");
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setSize(800, 600);
        setLayout(new BorderLayout());

        motor = new Motor3D();

        panelOpenGL = new GLJPanel(new GLCapabilities(GLProfile.get(GLProfile.GL2)));
        panelOpenGL.addGLEventListener(motor);
        add(panelOpenGL, BorderLayout.CENTER);

        JPanel panelControles = crearPanelControles();
        add(panelControles, BorderLayout.SOUTH);

        Timer timer = new Timer(16, e -> panelOpenGL.repaint());
        timer.start();
    }

    private JPanel crearPanelControles() {
        JPanel panelSouth = new JPanel(new GridLayout(2, 4, 10, 10));
        panelSouth.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10));

        slidersZ = new JSlider[3];
        slidersAlpha = new JSlider[3];

        for (int i = 0; i < 3; i++) {
            JPanel p = new JPanel(new GridLayout(2, 1));
            
            slidersZ[i] = new JSlider(-100, 0, -50 + i * 15);
            slidersZ[i].setBorder(BorderFactory.createTitledBorder("Z Plano " + (i + 1)));
            int finalI = i;
            slidersZ[i].addChangeListener(e -> motor.setZ(finalI, slidersZ[finalI].getValue() / 10.0f));
            
            slidersAlpha[i] = new JSlider(0, 100, 100);
            slidersAlpha[i].setBorder(BorderFactory.createTitledBorder("Alpha Plano " + (i + 1)));
            slidersAlpha[i].addChangeListener(e -> motor.setAlpha(finalI, slidersAlpha[finalI].getValue() / 100.0f));
            
            p.add(slidersZ[i]);
            p.add(slidersAlpha[i]);
            panelSouth.add(p);
        }

        JPanel panelChecks = new JPanel(new GridLayout(2, 1));
        cbDepthTest = new JCheckBox("Depth Test (Z-Buffer)", true);
        cbDepthTest.addActionListener(e -> motor.setDepthTest(cbDepthTest.isSelected()));
        
        cbVerProfundidad = new JCheckBox("Ver Profundidad", false);
        cbVerProfundidad.addActionListener(e -> motor.setVerProfundidad(cbVerProfundidad.isSelected()));

        panelChecks.add(cbDepthTest);
        panelChecks.add(cbVerProfundidad);
        panelSouth.add(panelChecks);

        return panelSouth;
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> {
            new Visor3D().setVisible(true);
        });
    }
}
