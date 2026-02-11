package uce.edu.web.api.auth.application;

import uce.edu.web.api.auth.domain.Usuario;
import uce.edu.web.api.auth.infraestructure.UsuarioRepository;
import uce.edu.web.api.auth.application.representation.TokenRepresentation;

import java.time.Instant;
import java.util.Set;

import jakarta.enterprise.context.ApplicationScoped;
import jakarta.inject.Inject;
import io.smallrye.jwt.build.Jwt;

@ApplicationScoped
public class AuthService {

    @Inject
    UsuarioRepository usuarioRepository;

    public TokenRepresentation authenticate(String user, String password) {
        if (user == null || password == null) {
            return null;
        }

        Usuario usuario = usuarioRepository.findByUsernameAndPassword(user, password);

        if (usuario != null) {
            String issuer = "matricula-auth";
            long ttl = 3600;

            Instant now = Instant.now();
            Instant exp = now.plusSeconds(ttl);

            String tokenRole = usuario.role;

            String jwt = Jwt.issuer(issuer)
                    .subject(user)
                    .groups(Set.of(tokenRole))
                    .issuedAt(now)
                    .expiresAt(exp)
                    .sign();

            return new TokenRepresentation(jwt, exp.getEpochSecond(), tokenRole);
        }
        return null;
    }
}
