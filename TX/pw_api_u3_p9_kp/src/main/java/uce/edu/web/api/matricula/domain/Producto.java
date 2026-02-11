package uce.edu.web.api.matricula.domain;

import jakarta.persistence.Entity;
import jakarta.persistence.GeneratedValue;
import jakarta.persistence.GenerationType;
import jakarta.persistence.Id;
import io.quarkus.hibernate.orm.panache.PanacheEntityBase;
import java.math.BigDecimal;

@Entity
public class Producto extends PanacheEntityBase {
    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    public Long id;

    public String codigoBarras;
    public String nombre;
    public String categoria;
    public Integer stock;
    public BigDecimal precioUnitario;

    public Producto() {
    }

    public Producto(String codigoBarras, String nombre, String categoria, Integer stock, BigDecimal precioUnitario) {
        this.codigoBarras = codigoBarras;
        this.nombre = nombre;
        this.categoria = categoria;
        this.stock = stock;
        this.precioUnitario = precioUnitario;
    }
}
