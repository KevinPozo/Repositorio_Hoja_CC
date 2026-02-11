package uce.edu.web.api.matricula.domain;

import jakarta.persistence.CascadeType;
import jakarta.persistence.Entity;
import jakarta.persistence.GeneratedValue;
import jakarta.persistence.GenerationType;
import jakarta.persistence.Id;
import jakarta.persistence.OneToMany;
import java.math.BigDecimal;
import java.util.List;
import io.quarkus.hibernate.orm.panache.PanacheEntityBase;

@Entity
public class Venta extends PanacheEntityBase {
    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    public Long id;

    public String numeroVenta;
    public String cedulaCliente;
    public BigDecimal totalVenta;

    @OneToMany(mappedBy = "venta", cascade = CascadeType.ALL)
    public List<DetalleVenta> detalles;
}
