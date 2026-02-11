<template>
  <div class="container">
    <h2>Realizar Venta</h2>

    <div v-if="mensaje" :class="['mensaje', tipoMensaje]">
      {{ mensaje }}
    </div>

    <div class="card header-section">
      <div class="inputs-container">
        <div class="form-group">
          <label>Número Venta:</label>
          <input v-model="numeroVenta" type="text" placeholder="Ej: 001" />
        </div>
        <div class="form-group">
          <label>Cédula:</label>
          <input
            v-model="cedulaCliente"
            type="text"
            placeholder="Ingrese cédula"
          />
        </div>
      </div>
      <div class="button-container">
        <button @click="guardarVenta" class="btn-guardar">GUARDAR</button>
      </div>
    </div>

    <div class="card detail-section">
      <div class="form-row">
        <div class="form-group half">
          <label>Código Barras:</label>
          <input
            v-model="codigoBarras"
            type="text"
            placeholder="Código producto"
          />
        </div>
        <div class="form-group half">
          <label>Cantidad:</label>
          <input
            v-model.number="cantidad"
            type="number"
            min="1"
            placeholder="Cant"
          />
        </div>
        <button @click="agregarProducto" class="btn-agregar">Agregar</button>
      </div>

      <table class="table">
        <thead>
          <tr>
            <th>C.Barras</th>
            <th>Nombre</th>
            <th>Cantidad</th>
            <th>Precio</th>
            <th>Subtotal</th>
          </tr>
        </thead>
        <tbody>
          <tr v-for="(item, index) in detalles" :key="index">
            <td>{{ item.codigoBarras }}</td>
            <td>{{ item.nombre }}</td>
            <td>{{ item.cantidad }}</td>
            <td>{{ item.precioUnitario }}</td>
            <td>{{ item.subtotal.toFixed(2) }}</td>
          </tr>
        </tbody>
      </table>

      <div class="total-row">
        <strong>TOTAL: {{ totalVenta.toFixed(2) }}</strong>
      </div>
    </div>
  </div>
</template>

<script>
import {
  buscarProductoFacade,
  realizarVentaFacade,
} from "../clients/FacturacionClient";

export default {
  data() {
    return {
      numeroVenta: "",
      cedulaCliente: "",
      codigoBarras: "",
      cantidad: 1,
      detalles: [],
      mensaje: "",
      tipoMensaje: "",
    };
  },
  computed: {
    totalVenta() {
      return this.detalles.reduce((sum, item) => sum + item.subtotal, 0);
    },
  },
  methods: {
    async agregarProducto() {
      const token = localStorage.getItem("token");
      if (!this.codigoBarras || this.cantidad <= 0) {
        this.mostrarMensaje("Ingrese código y cantidad válida", "error");
        return;
      }

      const producto = await buscarProductoFacade(this.codigoBarras, token);

      if (!producto) {
        this.mostrarMensaje("Producto no encontrado", "error");
        return;
      }

      const existingInList = this.detalles.filter(
        (d) => d.codigoBarras === this.codigoBarras,
      );
      const totalQtyInList = existingInList.reduce(
        (sum, d) => sum + d.cantidad,
        0,
      );

      if (producto.stock < totalQtyInList + this.cantidad) {
        this.mostrarMensaje(
          `Stock no disponible. Stock actual: ${producto.stock}`,
          "error",
        );
        return;
      }

      const subtotal = this.cantidad * producto.precioUnitario;

      this.detalles.push({
        codigoBarras: producto.codigoBarras,
        nombre: producto.nombre,
        cantidad: this.cantidad,
        precioUnitario: producto.precioUnitario,
        subtotal: subtotal,
      });

      this.codigoBarras = "";
      this.cantidad = 1;
      this.mostrarMensaje("Producto agregado", "success");
    },
    async guardarVenta() {
      const token = localStorage.getItem("token");
      if (
        !this.numeroVenta ||
        !this.cedulaCliente ||
        this.detalles.length === 0
      ) {
        this.mostrarMensaje(
          "Complete los datos de venta y agregue productos",
          "error",
        );
        return;
      }

      const body = {
        numeroVenta: this.numeroVenta,
        cedulaCliente: this.cedulaCliente,
        detalles: this.detalles.map((d) => ({
          codigoBarras: d.codigoBarras,
          cantidad: d.cantidad,
        })),
      };

      try {
        await realizarVentaFacade(body, token);
        this.mostrarMensaje("Venta realizada exitosamente", "success");
        this.limpiar();
      } catch (e) {
        console.error(e);
        this.mostrarMensaje(
          "Error al realizar venta (Verifique stock)",
          "error",
        );
      }
    },
    mostrarMensaje(msg, type) {
      this.mensaje = msg;
      this.tipoMensaje = type;
      setTimeout(() => {
        if (this.mensaje === msg) this.mensaje = "";
      }, 5000);
    },
    limpiar() {
      this.numeroVenta = "";
      this.cedulaCliente = "";
      this.detalles = [];
      this.codigoBarras = "";
      this.cantidad = 1;
    },
  },
};
</script>

<style scoped>
.container {
  padding: 20px;
  max-width: 800px;
  margin: 0 auto;
}
h2 {
  text-align: center;
  color: #333;
}
.card {
  background: white;
  border: 1px solid black; /* Black border */
  border-radius: 8px;
  padding: 20px;
  margin-bottom: 20px;
}
.header-section {
  display: flex;
  justify-content: space-between;
  align-items: center;
  padding: 30px;
}
.inputs-container {
  display: flex;
  flex-direction: column;
  gap: 15px;
  width: 60%;
}
.button-container {
  display: flex;
  justify-content: center;
  align-items: center;
  width: 35%;
}
.form-row {
  display: flex;
  gap: 15px;
  align-items: flex-end;
  flex-wrap: wrap;
  width: 100%;
}
.form-group {
  margin-bottom: 0; /* Handled by gap in inputs-container */
  width: 100%;
}
.half {
  flex: 1;
  min-width: 150px;
}
label {
  display: block;
  font-weight: bold;
  color: #000; /* Black Label */
  margin-bottom: 5px;
}
input {
  width: 100%;
  padding: 8px;
  border: 1px solid #000; /* Black Border input */
  border-radius: 4px;
  box-sizing: border-box;
}
.btn-guardar,
.btn-agregar {
  background-color: white;
  color: #000;
  border: 1px solid #000;
  padding: 8px 16px;
  font-weight: bold;
  border-radius: 15px; /* Keeps consistent rounded style but black */
  cursor: pointer;
  transition: all 0.3s;
  height: 40px;
}
.btn-guardar:hover,
.btn-agregar:hover {
  background-color: #000;
  color: white;
}
.table {
  width: 100%;
  border-collapse: collapse;
  margin-top: 15px;
}
.table th {
  color: #000;
  text-align: left;
  padding: 8px;
  border-bottom: 1px solid #000;
}
.table td {
  padding: 8px;
  border-bottom: 1px solid #ccc;
  color: #000;
}
.total-row {
  text-align: right;
  margin-top: 15px;
  font-size: 1.2em;
  color: #000;
}
.mensaje {
  text-align: center;
  padding: 10px;
  margin-bottom: 15px;
  border-radius: 4px;
}
.success {
  background: #dff0d8;
  color: #3c763d;
  border: 1px solid #d6e9c6;
}
.error {
  background: #f2dede;
  color: #a94442;
  border: 1px solid #ebccd1;
}
</style>
