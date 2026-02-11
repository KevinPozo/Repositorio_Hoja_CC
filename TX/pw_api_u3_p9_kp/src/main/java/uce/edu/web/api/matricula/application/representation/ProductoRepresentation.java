package uce.edu.web.api.matricula.application.representation;

import java.math.BigDecimal;
import java.util.ArrayList;
import java.util.List;

public class ProductoRepresentation {
    public Long id;
    public String codigoBarras;
    public String nombre;
    public String categoria;
    public Integer stock;
    public BigDecimal precioUnitario;
    public List<LinkDto> links = new ArrayList<>();

    public void addLink(String rel, String href) {
        this.links.add(new LinkDto(rel, href));
    }
}
