package uce.edu.web.api.matricula.domain;

import jakarta.persistence.Entity;
import jakarta.persistence.GeneratedValue;
import jakarta.persistence.GenerationType;
import jakarta.persistence.Id;
import io.quarkus.hibernate.orm.panache.PanacheEntityBase;
import java.time.LocalDate;

@Entity
public class Paciente extends PanacheEntityBase {
    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    public Long id;

    public String cedula;
    public String nombre;
    public String apellido;
    public LocalDate fechaNacimiento;

    public Paciente() {
    }

    public Paciente(String cedula, String nombre, String apellido, LocalDate fechaNacimiento) {
        this.cedula = cedula;
        this.nombre = nombre;
        this.apellido = apellido;
        this.fechaNacimiento = fechaNacimiento;
    }
}
