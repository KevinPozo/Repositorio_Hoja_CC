package uce.edu.web.api.matricula.interfaces;

import jakarta.annotation.security.RolesAllowed;
import jakarta.inject.Inject;
import jakarta.ws.rs.Consumes;
import jakarta.ws.rs.POST;
import jakarta.ws.rs.GET;
import jakarta.ws.rs.Path;
import jakarta.ws.rs.PathParam;
import jakarta.ws.rs.Produces;
import jakarta.ws.rs.core.MediaType;
import jakarta.ws.rs.core.Response;
import uce.edu.web.api.matricula.domain.Producto;
import uce.edu.web.api.matricula.application.ProductoService;
import uce.edu.web.api.matricula.application.representation.ProductoRepresentation;

import java.util.List;
import java.util.stream.Collectors;

@Path("/productos")
@Produces(MediaType.APPLICATION_JSON)
@Consumes(MediaType.APPLICATION_JSON)
@RolesAllowed({ "user", "admin" })
public class ProductoResource {

    @Inject
    ProductoService productoService;

    @POST
    public Response ingresarProducto(Producto producto) {
        productoService.ingresarProducto(producto);
        return Response.status(Response.Status.CREATED).build();
    }

    @GET
    @Path("/{codigo}")
    public Response buscarPorCodigo(@PathParam("codigo") String codigo) {
        Producto p = productoService.buscarPorCodigo(codigo);
        if (p == null) {
            return Response.status(Response.Status.NOT_FOUND).build();
        }

        ProductoRepresentation rep = new ProductoRepresentation();
        rep.id = p.id;
        rep.codigoBarras = p.codigoBarras;
        rep.nombre = p.nombre;
        rep.categoria = p.categoria;
        rep.stock = p.stock;
        rep.precioUnitario = p.precioUnitario;

        rep.addLink("self", "/productos/" + p.codigoBarras);
        rep.addLink("parent", "/productos");

        return Response.ok(rep).build();
    }
}
