package uce.edu.web.api.matricula.infrastructure;

import uce.edu.web.api.matricula.domain.Estudiante;
import io.quarkus.hibernate.orm.panache.PanacheRepository;
import jakarta.enterprise.context.ApplicationScoped;

@ApplicationScoped
public class EstudianteRepository implements PanacheRepository<Estudiante> {
}
