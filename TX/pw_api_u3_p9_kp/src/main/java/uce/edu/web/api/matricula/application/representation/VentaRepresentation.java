package uce.edu.web.api.matricula.application.representation;

import java.math.BigDecimal;
import java.util.ArrayList;
import java.util.List;

public class VentaRepresentation {
    public String numeroVenta;
    public String cedulaCliente;
    public BigDecimal totalVenta;
    public List<DetalleVentaRepresentation> detalles;
    public List<LinkDto> links = new ArrayList<>();

    public void addLink(String rel, String href) {
        this.links.add(new LinkDto(rel, href));
    }
}
