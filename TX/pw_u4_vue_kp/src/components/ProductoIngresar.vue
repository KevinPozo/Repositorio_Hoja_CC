<template>
  <div class="container">
    <h2>Ingresar Producto</h2>
    <div class="form-container">
      <div v-if="mensaje" :class="['mensaje', tipoMensaje]">
        {{ mensaje }}
      </div>

      <div class="form-group">
        <label>Código Barras:</label>
        <input
          v-model="producto.codigoBarras"
          type="text"
          placeholder="Ingrese código"
        />
      </div>

      <div class="form-group">
        <label>Nombre:</label>
        <input
          v-model="producto.nombre"
          type="text"
          placeholder="Ingrese nombre"
        />
      </div>

      <div class="form-group">
        <label>Categoría:</label>
        <input
          v-model="producto.categoria"
          type="text"
          placeholder="Ingrese categoría"
        />
      </div>

      <div class="form-group">
        <label>Stock:</label>
        <input
          v-model.number="producto.stock"
          type="number"
          placeholder="Ingrese stock"
        />
      </div>

      <div class="form-group">
        <label>Precio Unitario:</label>
        <input
          v-model.number="producto.precioUnitario"
          type="number"
          step="0.01"
          placeholder="Ingrese precio"
        />
      </div>

      <button @click="guardar" class="btn-guardar">Guardar</button>
    </div>
  </div>
</template>

<script>
import { ingresarProductoFacade } from "../clients/FacturacionClient";

export default {
  data() {
    return {
      producto: {
        codigoBarras: "",
        nombre: "",
        categoria: "",
        stock: 0,
        precioUnitario: 0.0,
      },
      mensaje: "",
      tipoMensaje: "",
    };
  },
  methods: {
    async guardar() {
      const token = localStorage.getItem("token");
      if (!token) {
        this.mostrarMensaje("Debe iniciar sesión", "error");
        return;
      }

      if (
        !this.producto.codigoBarras ||
        !this.producto.nombre ||
        this.producto.stock < 0 ||
        this.producto.precioUnitario < 0
      ) {
        this.mostrarMensaje(
          "Por favor complete todos los campos correctamente",
          "error",
        );
        return;
      }

      try {
        await ingresarProductoFacade(this.producto, token);
        this.mostrarMensaje("Producto guardado exitosamente", "success");
        this.limpiar();
      } catch (e) {
        console.error(e);
        this.mostrarMensaje("Error al guardar producto", "error");
      }
    },
    mostrarMensaje(msg, type) {
      this.mensaje = msg;
      this.tipoMensaje = type;
      setTimeout(() => {
        this.mensaje = "";
      }, 5000);
    },
    limpiar() {
      this.producto = {
        codigoBarras: "",
        nombre: "",
        categoria: "",
        stock: 0,
        precioUnitario: 0.0,
      };
    },
  },
};
</script>

<style scoped>
.container {
  padding: 30px;
  max-width: 600px;
  margin: 20px auto;
  background-color: #fff;
  border-radius: 8px;
  box-shadow: 0 2px 5px rgba(0, 0, 0, 0.1);
  border: 2px solid black;
}
h2 {
  color: #343a40;
  margin-bottom: 20px;
  text-align: center;
}
.form-group {
  margin-bottom: 20px;
  text-align: left;
}
label {
  display: block;
  font-weight: 600;
  margin-bottom: 8px;
  color: #495057;
}
input {
  width: 100%;
  padding: 10px;
  border: 1px solid #ced4da;
  border-radius: 4px;
  font-size: 16px;
  box-sizing: border-box;
}
input:focus {
  border-color: #34495e;
  outline: none;
}
.btn-guardar {
  display: block;
  width: auto;
  min-width: 200px;
  margin: 20px auto 0;
  padding: 12px;
  background-color: #34495e;
  color: white;
  border: none;
  border-radius: 4px;
  cursor: pointer;
  font-size: 16px;
  font-weight: 600;
  transition: background-color 0.15s;
}
.btn-guardar:hover {
  background-color: #5d6d7e;
}
.mensaje {
  padding: 15px;
  border-radius: 4px;
  text-align: center;
  margin-bottom: 20px;
}
.success {
  background-color: #d4edda;
  color: #155724;
  border: 1px solid #c3e6cb;
}
.error {
  background-color: #f8d7da;
  color: #721c24;
  border: 1px solid #f5c6cb;
}
</style>
