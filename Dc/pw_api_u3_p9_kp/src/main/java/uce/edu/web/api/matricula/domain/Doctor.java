package uce.edu.web.api.matricula.domain;

import jakarta.persistence.Entity;
import jakarta.persistence.GeneratedValue;
import jakarta.persistence.GenerationType;
import jakarta.persistence.Id;
import io.quarkus.hibernate.orm.panache.PanacheEntityBase;

@Entity
public class Doctor extends PanacheEntityBase {
    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    public Long id;

    public String cedula;
    public String nombre;
    public String apellido;
    public String genero;

    public Doctor() {
    }

    public Doctor(String cedula, String nombre, String apellido, String genero) {
        this.cedula = cedula;
        this.nombre = nombre;
        this.apellido = apellido;
        this.genero = genero;
    }
}
