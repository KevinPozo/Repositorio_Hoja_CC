package uce.edu.web.api.matricula.infrastructure;

import io.quarkus.hibernate.orm.panache.PanacheRepository;
import jakarta.enterprise.context.ApplicationScoped;
import uce.edu.web.api.matricula.domain.Producto;

@ApplicationScoped
public class ProductoRepository implements PanacheRepository<Producto> {
    public Producto findByCodigoBarras(String codigoBarras) {
        return find("codigoBarras", codigoBarras).firstResult();
    }
}
