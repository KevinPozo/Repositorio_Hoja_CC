package uce.edu.web.api.matricula.interfaces;

import uce.edu.web.api.matricula.application.EstudianteService;
import uce.edu.web.api.matricula.application.representation.EstudianteRepresentation;
import jakarta.annotation.security.RolesAllowed;
import jakarta.inject.Inject;
import jakarta.ws.rs.*;
import jakarta.ws.rs.core.MediaType;
import jakarta.ws.rs.core.Response;
import java.util.List;

@Path("/estudiantes")
@Produces(MediaType.APPLICATION_JSON)
@Consumes(MediaType.APPLICATION_JSON)
@RolesAllowed({ "user", "admin" })
public class EstudianteResource {

    @Inject
    EstudianteService estudianteService;

    @POST
    public Response guardar(EstudianteRepresentation rep) {
        estudianteService.guardarEstudiante(rep);
        return Response.status(Response.Status.CREATED).build();
    }

    @GET
    public List<EstudianteRepresentation> listar() {
        return estudianteService.listarEstudiantes();
    }
}
