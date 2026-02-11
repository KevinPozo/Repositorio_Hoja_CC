package uce.edu.web.api.matricula.application;

import java.util.List;
import java.util.stream.Collectors;

import jakarta.enterprise.context.ApplicationScoped;
import jakarta.inject.Inject;
import jakarta.transaction.Transactional;
import uce.edu.web.api.matricula.application.representation.DoctorRepresentation;
import uce.edu.web.api.matricula.domain.Doctor;
import uce.edu.web.api.matricula.infraestructure.DoctorRepository;

@ApplicationScoped
public class DoctorService {
    @Inject
    private DoctorRepository doctorRepository;

    @Transactional
    public DoctorRepresentation crear(DoctorRepresentation rep) {
        Doctor doctor = new Doctor();
        doctor.cedula = rep.getCedula();
        doctor.nombre = rep.getNombre();
        doctor.apellido = rep.getApellido();
        doctor.genero = rep.getGenero();

        doctorRepository.persist(doctor);
        return mapToRepresentation(doctor);
    }

    public List<DoctorRepresentation> listar() {
        return doctorRepository.listAll().stream()
                .map(this::mapToRepresentation)
                .collect(Collectors.toList());
    }

    private DoctorRepresentation mapToRepresentation(Doctor d) {
        DoctorRepresentation rep = new DoctorRepresentation();
        rep.setId(d.id.intValue());
        rep.setCedula(d.cedula);
        rep.setNombre(d.nombre);
        rep.setApellido(d.apellido);
        rep.setGenero(d.genero);
        return rep;
    }
}
