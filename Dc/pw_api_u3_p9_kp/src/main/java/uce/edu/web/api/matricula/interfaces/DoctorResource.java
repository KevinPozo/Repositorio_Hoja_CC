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
import uce.edu.web.api.matricula.application.DoctorService;
import uce.edu.web.api.matricula.application.representation.DoctorRepresentation;
import uce.edu.web.api.matricula.application.representation.LinkDto;

@Path("/doctores")
public class DoctorResource {
    @Inject
    private DoctorService doctorService;
    @Context
    private UriInfo uriInfo;

    @GET
    @RolesAllowed({ "user", "admin" })
    @Produces(MediaType.APPLICATION_JSON)
    public List<DoctorRepresentation> listar() {
        return doctorService.listar().stream()
                .map(this::construirLinks)
                .toList();
    }

    @POST
    @RolesAllowed({ "user", "admin" })
    @Consumes(MediaType.APPLICATION_JSON)
    @Produces(MediaType.APPLICATION_JSON)
    public Response crear(DoctorRepresentation rep) {
        DoctorRepresentation creado = doctorService.crear(rep);
        return Response.status(Response.Status.CREATED)
                .entity(construirLinks(creado))
                .build();
    }

    private DoctorRepresentation construirLinks(DoctorRepresentation rep) {
        String self = uriInfo.getBaseUriBuilder().path(DoctorResource.class)
                .path(String.valueOf(rep.getId())).build().toString();
        rep.setLinks(List.of(new LinkDto(self, "self")));
        return rep;
    }
}
