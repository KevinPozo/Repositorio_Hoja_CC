package uce.edu.web.api.matricula.application;

import java.util.List;
import java.util.stream.Collectors;

import jakarta.enterprise.context.ApplicationScoped;
import jakarta.inject.Inject;
import jakarta.transaction.Transactional;
import uce.edu.web.api.matricula.application.representation.CitaRepresentation;
import uce.edu.web.api.matricula.application.representation.CitaRequest;
import uce.edu.web.api.matricula.domain.Cita;
import uce.edu.web.api.matricula.domain.Doctor;
import uce.edu.web.api.matricula.domain.Paciente;
import uce.edu.web.api.matricula.infraestructure.CitaRepository;
import uce.edu.web.api.matricula.infraestructure.DoctorRepository;
import uce.edu.web.api.matricula.infraestructure.PacienteRepository;

@ApplicationScoped
public class CitaService {
    @Inject
    private CitaRepository citaRepository;
    @Inject
    private DoctorRepository doctorRepository;
    @Inject
    private PacienteRepository pacienteRepository;

    @Transactional
    public CitaRepresentation agendar(CitaRequest request) {
        Doctor doctor = doctorRepository.find("cedula", request.getCedulaDoctor()).firstResult();
        Paciente paciente = pacienteRepository.find("cedula", request.getCedulaPaciente()).firstResult();

        if (doctor == null || paciente == null) {
            return null; // Handle error appropriately in Resource
        }

        Cita cita = new Cita();
        cita.doctor = doctor;
        cita.paciente = paciente;
        cita.fechaCita = request.getFechaCita();

        citaRepository.persist(cita);
        return mapToRepresentation(cita);
    }

    public List<CitaRepresentation> listar() {
        return citaRepository.listAll().stream()
                .map(this::mapToRepresentation)
                .collect(Collectors.toList());
    }

    private CitaRepresentation mapToRepresentation(Cita c) {
        CitaRepresentation rep = new CitaRepresentation();
        rep.setId(c.id.intValue());
        rep.setNombrePaciente(c.paciente.nombre);
        rep.setApellidoPaciente(c.paciente.apellido);
        rep.setCedulaPaciente(c.paciente.cedula);
        rep.setFechaCita(c.fechaCita);
        rep.setNombreDoctor(c.doctor.nombre); // Exam req: Nombre Doctor
        return rep;
    }
}
