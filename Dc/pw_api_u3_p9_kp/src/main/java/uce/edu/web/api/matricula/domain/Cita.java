package uce.edu.web.api.matricula.domain;

import jakarta.persistence.Entity;
import jakarta.persistence.GeneratedValue;
import jakarta.persistence.GenerationType;
import jakarta.persistence.Id;
import jakarta.persistence.ManyToOne;
import io.quarkus.hibernate.orm.panache.PanacheEntityBase;
import java.time.LocalDateTime;

@Entity
public class Cita extends PanacheEntityBase {
    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    public Long id;

    @ManyToOne
    public Doctor doctor;

    @ManyToOne
    public Paciente paciente;

    public LocalDateTime fechaCita;

    public Cita() {
    }

    public Cita(Doctor doctor, Paciente paciente, LocalDateTime fechaCita) {
        this.doctor = doctor;
        this.paciente = paciente;
        this.fechaCita = fechaCita;
    }
}
