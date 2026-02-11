package uce.edu.web.api.matricula.domain;

import jakarta.persistence.Entity;
import jakarta.persistence.GeneratedValue;
import jakarta.persistence.GenerationType;
import jakarta.persistence.Id;
import jakarta.persistence.ManyToOne;
import java.math.BigDecimal;
import io.quarkus.hibernate.orm.panache.PanacheEntityBase;

@Entity
public class DetalleVenta extends PanacheEntityBase {
    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    public Long id;

    public Integer cantidad;
    public BigDecimal precioUnitario;
    public BigDecimal subtotal;

    @ManyToOne
    public Venta venta;

    @ManyToOne
    public Producto producto;
}
