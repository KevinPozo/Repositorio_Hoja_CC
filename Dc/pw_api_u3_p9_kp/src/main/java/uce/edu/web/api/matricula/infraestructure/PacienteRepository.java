package uce.edu.web.api.matricula.infraestructure;

import io.quarkus.hibernate.orm.panache.PanacheRepository;
import jakarta.enterprise.context.ApplicationScoped;
import uce.edu.web.api.matricula.domain.Paciente;

@ApplicationScoped
public class PacienteRepository implements PanacheRepository<Paciente> {

}
