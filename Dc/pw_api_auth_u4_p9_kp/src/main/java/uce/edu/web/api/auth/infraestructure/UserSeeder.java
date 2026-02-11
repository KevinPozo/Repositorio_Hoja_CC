package uce.edu.web.api.auth.infraestructure;

import io.quarkus.runtime.StartupEvent;
import jakarta.enterprise.context.ApplicationScoped;
import jakarta.enterprise.event.Observes;
import jakarta.inject.Inject;
import jakarta.transaction.Transactional;
import uce.edu.web.api.auth.domain.Usuario;
import uce.edu.web.api.auth.infraestructure.UsuarioRepository;

@ApplicationScoped
public class UserSeeder {

    @Inject
    UsuarioRepository usuarioRepository;

    @Transactional
    public void onStart(@Observes StartupEvent ev) {
        if (usuarioRepository.count() == 0) {
            Usuario admin = new Usuario();
            admin.username = "admin";
            admin.password = "admin";
            admin.role = "admin";
            usuarioRepository.persist(admin);

            Usuario user = new Usuario();
            user.username = "usuario";
            user.password = "1234";
            user.role = "user";
            usuarioRepository.persist(user);

            System.out.println("Usuarios iniciales creados exitosamente.");
        }
    }
}
