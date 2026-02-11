package uce.edu.web.api.auth.interfaces;

import uce.edu.web.api.auth.application.AuthService;
import uce.edu.web.api.auth.application.representation.TokenRepresentation;

import jakarta.inject.Inject;
import jakarta.ws.rs.GET;
import jakarta.ws.rs.Path;
import jakarta.ws.rs.Produces;
import jakarta.ws.rs.QueryParam;
import jakarta.ws.rs.core.MediaType;
import jakarta.ws.rs.core.Response;

@Path("/auth")
public class AuthResource {

    @Inject
    AuthService authService;

    @GET
    @Path("/token")
    @Produces(MediaType.APPLICATION_JSON)
    public Response token(
            @QueryParam("user") String user,
            @QueryParam("password") String password) {

        TokenRepresentation token = authService.authenticate(user, password);

        if (token != null) {
            return Response.ok(token).build();
        }

        if (user == null || password == null) {
            return Response.status(Response.Status.BAD_REQUEST).build();
        }

        return Response.status(Response.Status.UNAUTHORIZED).build();
    }
}