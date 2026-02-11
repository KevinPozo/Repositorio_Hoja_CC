package uce.edu.web.api.matricula.application;

import uce.edu.web.api.matricula.application.representation.EstudianteRepresentation;
import uce.edu.web.api.matricula.domain.Estudiante;
import uce.edu.web.api.matricula.infrastructure.EstudianteRepository;
import jakarta.enterprise.context.ApplicationScoped;
import jakarta.inject.Inject;
import jakarta.transaction.Transactional;
import java.util.List;
import java.util.stream.Collectors;

@ApplicationScoped
public class EstudianteService {

    @Inject
    EstudianteRepository estudianteRepository;

    @Transactional
    public void guardarEstudiante(EstudianteRepresentation rep) {
        Estudiante estudiante = new Estudiante(rep.nombre, rep.apellido, rep.cedula);
        estudianteRepository.persist(estudiante);
    }

    public List<EstudianteRepresentation> listarEstudiantes() {
        return estudianteRepository.listAll().stream()
                .map(e -> new EstudianteRepresentation(e.id, e.nombre, e.apellido, e.cedula))
                .collect(Collectors.toList());
    }
}
