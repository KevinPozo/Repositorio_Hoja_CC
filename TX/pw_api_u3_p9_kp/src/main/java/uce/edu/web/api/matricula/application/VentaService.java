package uce.edu.web.api.matricula.application;

import jakarta.enterprise.context.ApplicationScoped;
import jakarta.inject.Inject;
import jakarta.transaction.Transactional;
import uce.edu.web.api.matricula.application.representation.DetalleVentaRepresentation;
import uce.edu.web.api.matricula.application.representation.VentaRepresentation;
import uce.edu.web.api.matricula.domain.DetalleVenta;
import uce.edu.web.api.matricula.domain.Producto;
import uce.edu.web.api.matricula.domain.Venta;
import uce.edu.web.api.matricula.infrastructure.ProductoRepository;
import uce.edu.web.api.matricula.infrastructure.VentaRepository;

import java.math.BigDecimal;
import java.util.ArrayList;

@ApplicationScoped
public class VentaService {

    @Inject
    VentaRepository ventaRepository;

    @Inject
    ProductoRepository productoRepository;

    @Transactional
    public void realizarVenta(VentaRepresentation ventaRep) {
        Venta venta = new Venta();
        venta.numeroVenta = ventaRep.numeroVenta;
        venta.cedulaCliente = ventaRep.cedulaCliente;
        venta.detalles = new ArrayList<>();

        BigDecimal total = BigDecimal.ZERO;

        if (ventaRep.detalles != null) {
            for (DetalleVentaRepresentation dRep : ventaRep.detalles) {
                Producto producto = productoRepository.findByCodigoBarras(dRep.codigoBarras);
                if (producto == null) {
                    throw new RuntimeException("Producto no encontrado: " + dRep.codigoBarras);
                }
                if (producto.stock < dRep.cantidad) {
                    throw new RuntimeException("Stock no disponible para: " + producto.nombre);
                }

                producto.stock -= dRep.cantidad;
                productoRepository.persist(producto);

                DetalleVenta detalle = new DetalleVenta();
                detalle.cantidad = dRep.cantidad;
                detalle.precioUnitario = producto.precioUnitario;
                detalle.subtotal = detalle.precioUnitario.multiply(new BigDecimal(detalle.cantidad));
                detalle.producto = producto;
                detalle.venta = venta;

                venta.detalles.add(detalle);
                total = total.add(detalle.subtotal);
            }
        }

        venta.totalVenta = total;
        ventaRepository.persist(venta);
    }
}
