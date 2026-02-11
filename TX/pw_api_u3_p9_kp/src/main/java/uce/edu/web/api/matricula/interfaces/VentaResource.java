package uce.edu.web.api.matricula.interfaces;

import jakarta.annotation.security.RolesAllowed;
import jakarta.inject.Inject;
import jakarta.ws.rs.Consumes;
import jakarta.ws.rs.POST;
import jakarta.ws.rs.Path;
import jakarta.ws.rs.Produces;
import jakarta.ws.rs.core.MediaType;
import jakarta.ws.rs.core.Response;
import uce.edu.web.api.matricula.application.VentaService;
import uce.edu.web.api.matricula.application.representation.VentaRepresentation;

@Path("/ventas")
@RolesAllowed({ "user", "admin" })
public class VentaResource {

    @Inject
    VentaService ventaService;

    @POST
    @Consumes(MediaType.APPLICATION_JSON)
    @Produces(MediaType.APPLICATION_JSON)
    public Response realizarVenta(VentaRepresentation ventaRep) {
        try {
            ventaService.realizarVenta(ventaRep);
            return Response.status(Response.Status.CREATED).build();
        } catch (RuntimeException e) {
            return Response.status(Response.Status.BAD_REQUEST).entity(e.getMessage()).build();
        }
    }
}
