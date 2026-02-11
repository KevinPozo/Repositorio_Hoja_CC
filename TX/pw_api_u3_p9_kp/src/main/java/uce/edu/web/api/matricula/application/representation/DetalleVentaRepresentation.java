package uce.edu.web.api.matricula.application.representation;

import java.math.BigDecimal;

public class DetalleVentaRepresentation {
    public String codigoBarras;
    public Integer cantidad;
    public BigDecimal precioUnitario;
    public BigDecimal subtotal;

    public DetalleVentaRepresentation() {
    }

    public DetalleVentaRepresentation(String codigoBarras, Integer cantidad) {
        this.codigoBarras = codigoBarras;
        this.cantidad = cantidad;
    }
}
