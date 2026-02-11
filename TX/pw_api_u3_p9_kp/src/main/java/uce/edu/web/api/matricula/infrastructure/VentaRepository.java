package uce.edu.web.api.matricula.infrastructure;

import io.quarkus.hibernate.orm.panache.PanacheRepository;
import jakarta.enterprise.context.ApplicationScoped;
import uce.edu.web.api.matricula.domain.Venta;

@ApplicationScoped
public class VentaRepository implements PanacheRepository<Venta> {
}
