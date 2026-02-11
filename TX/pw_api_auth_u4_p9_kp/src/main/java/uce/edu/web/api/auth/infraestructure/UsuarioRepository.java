package uce.edu.web.api.auth.infraestructure;

import io.quarkus.hibernate.orm.panache.PanacheRepository;
import jakarta.enterprise.context.ApplicationScoped;
import uce.edu.web.api.auth.domain.Usuario;

@ApplicationScoped
public class UsuarioRepository implements PanacheRepository<Usuario> {

    public Usuario findByUsernameAndPassword(String username, String password) {
        return find("username = ?1 and password = ?2", username, password).firstResult();
    }
}
