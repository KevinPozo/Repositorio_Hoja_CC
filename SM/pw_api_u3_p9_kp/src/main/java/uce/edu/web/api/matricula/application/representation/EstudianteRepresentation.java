package uce.edu.web.api.matricula.application.representation;

public class EstudianteRepresentation {
    public Long id;
    public String nombre;
    public String apellido;
    public String cedula;

    public EstudianteRepresentation() {
    }

    public EstudianteRepresentation(Long id, String nombre, String apellido, String cedula) {
        this.id = id;
        this.nombre = nombre;
        this.apellido = apellido;
        this.cedula = cedula;
    }
}
