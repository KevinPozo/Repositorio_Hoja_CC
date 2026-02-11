package uce.edu.web.api.matricula.interfaces;

import java.util.List;

import jakarta.annotation.security.RolesAllowed;
import jakarta.inject.Inject;
import jakarta.ws.rs.Consumes;
import jakarta.ws.rs.GET;
import jakarta.ws.rs.POST;
import jakarta.ws.rs.Path;
import jakarta.ws.rs.Produces;
import jakarta.ws.rs.core.Context;
import jakarta.ws.rs.core.MediaType;
import jakarta.ws.rs.core.Response;
import jakarta.ws.rs.core.UriInfo;
import uce.edu.web.api.matricula.application.PacienteService;
import uce.edu.web.api.matricula.application.representation.LinkDto;
import uce.edu.web.api.matricula.application.representation.PacienteRepresentation;

@Path("/pacientes")
public class PacienteResource {
    @Inject
    private PacienteService pacienteService;
    @Context
    private UriInfo uriInfo;

    @GET
    @RolesAllowed({ "user", "admin" })
    @Produces(MediaType.APPLICATION_JSON)
    public List<PacienteRepresentation> listar() {
        return pacienteService.listar().stream()
                .map(this::construirLinks)
                .toList();
    }

    @POST
    @RolesAllowed({ "user", "admin" })
    @Consumes(MediaType.APPLICATION_JSON)
    @Produces(MediaType.APPLICATION_JSON)
    public Response crear(PacienteRepresentation rep) {
        PacienteRepresentation creado = pacienteService.crear(rep);
        return Response.status(Response.Status.CREATED)
                .entity(construirLinks(creado))
                .build();
    }

    private PacienteRepresentation construirLinks(PacienteRepresentation rep) {
        String self = uriInfo.getBaseUriBuilder().path(PacienteResource.class)
                .path(String.valueOf(rep.getId())).build().toString();
        rep.setLinks(List.of(new LinkDto(self, "self")));
        return rep;
    }
}
