package uce.edu.web.api.matricula.application;

import jakarta.enterprise.context.ApplicationScoped;
import jakarta.inject.Inject;
import jakarta.transaction.Transactional;
import uce.edu.web.api.matricula.domain.Producto;
import uce.edu.web.api.matricula.infrastructure.ProductoRepository;

@ApplicationScoped
public class ProductoService {

    @Inject
    ProductoRepository productoRepository;

    @Transactional
    public void ingresarProducto(Producto producto) {
        Producto existente = productoRepository.findByCodigoBarras(producto.codigoBarras);
        if (existente != null) {
            existente.stock += producto.stock;

        } else {
            productoRepository.persist(producto);
        }
    }

    public Producto buscarPorCodigo(String codigo) {
        return productoRepository.findByCodigoBarras(codigo);
    }
}
