package uce.edu.web.api.matricula.application;

import java.util.List;
import java.util.stream.Collectors;

import jakarta.enterprise.context.ApplicationScoped;
import jakarta.inject.Inject;
import jakarta.transaction.Transactional;
import uce.edu.web.api.matricula.application.representation.PacienteRepresentation;
import uce.edu.web.api.matricula.domain.Paciente;
import uce.edu.web.api.matricula.infraestructure.PacienteRepository;

@ApplicationScoped
public class PacienteService {
    @Inject
    private PacienteRepository pacienteRepository;

    @Transactional
    public PacienteRepresentation crear(PacienteRepresentation rep) {
        Paciente paciente = new Paciente();
        paciente.cedula = rep.getCedula();
        paciente.nombre = rep.getNombre();
        paciente.apellido = rep.getApellido();
        paciente.fechaNacimiento = rep.getFechaNacimiento();

        pacienteRepository.persist(paciente);
        return mapToRepresentation(paciente);
    }

    public List<PacienteRepresentation> listar() {
        return pacienteRepository.listAll().stream()
                .map(this::mapToRepresentation)
                .collect(Collectors.toList());
    }

    private PacienteRepresentation mapToRepresentation(Paciente p) {
        PacienteRepresentation rep = new PacienteRepresentation();
        rep.setId(p.id.intValue());
        rep.setCedula(p.cedula);
        rep.setNombre(p.nombre);
        rep.setApellido(p.apellido);
        rep.setFechaNacimiento(p.fechaNacimiento);
        return rep;
    }
}
