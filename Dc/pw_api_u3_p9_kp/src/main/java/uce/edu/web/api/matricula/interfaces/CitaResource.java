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
import uce.edu.web.api.matricula.application.CitaService;
import uce.edu.web.api.matricula.application.representation.CitaRepresentation;
import uce.edu.web.api.matricula.application.representation.CitaRequest;
import uce.edu.web.api.matricula.application.representation.LinkDto;

@Path("/citas")
public class CitaResource {
    @Inject
    private CitaService citaService;
    @Context
    private UriInfo uriInfo;

    @GET
    @RolesAllowed({ "user", "admin" })
    @Produces(MediaType.APPLICATION_JSON)
    public List<CitaRepresentation> listar() {
        return citaService.listar().stream()
                .map(this::construirLinks)
                .toList();
    }

    @POST
    @RolesAllowed({ "user", "admin" })
    @Consumes(MediaType.APPLICATION_JSON)
    @Produces(MediaType.APPLICATION_JSON)
    public Response agendar(CitaRequest request) {
        CitaRepresentation creada = citaService.agendar(request);
        if (creada == null) {
            return Response.status(Response.Status.NOT_FOUND)
                    .entity("Doctor o Paciente no encontrado").build();
        }
        return Response.status(Response.Status.CREATED)
                .entity(construirLinks(creada))
                .build();
    }

    private CitaRepresentation construirLinks(CitaRepresentation rep) {
        String self = uriInfo.getBaseUriBuilder().path(CitaResource.class)
                .path(String.valueOf(rep.getId())).build().toString();
        rep.setLinks(List.of(new LinkDto(self, "self")));
        return rep;
    }
}
