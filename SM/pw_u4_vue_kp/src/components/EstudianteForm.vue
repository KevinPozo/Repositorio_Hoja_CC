<template>
  <div class="component-card dashed-border">
    <h3 class="title">ESTUDIANTE</h3>

    <div class="form-group">
      <label>Token</label>
      <input
        v-model="token"
        type="text"
        class="form-control"
        placeholder="Pegue el token aquí"
      />
    </div>

    <div class="form-group">
      <label>Nombre</label>
      <input
        v-model="estudiante.nombre"
        type="text"
        class="form-control"
        placeholder="Ingrese su nombre"
      />
    </div>

    <div class="form-group">
      <label>Apellido</label>
      <input
        v-model="estudiante.apellido"
        type="text"
        class="form-control"
        placeholder="Ingrese su apellido"
      />
    </div>

    <div class="form-group">
      <label>Cédula</label>
      <input
        v-model="estudiante.cedula"
        type="text"
        class="form-control"
        placeholder="Ingrese su cédula"
      />
    </div>

    <div class="button-container">
      <button @click="guardar" class="btn-guardar">Guardar</button>
    </div>
  </div>
</template>

<script>
import { guardarEstudianteFacade } from "@/clients/InscripcionClient";

export default {
  inject: ["addNotification"],
  data() {
    return {
      token: "",
      estudiante: {
        nombre: "",
        apellido: "",
        cedula: "",
      },
    };
  },
  methods: {
    async guardar() {
      if (!this.token) {
        this.addNotification("Ingrese un token válido", "error");
        return;
      }
      try {
        await guardarEstudianteFacade(this.estudiante, this.token);
        this.addNotification("Estudiante guardado", "info");
        this.$emit("estudiante-guardado");
        this.estudiante = { nombre: "", apellido: "", cedula: "" };
      } catch (error) {
        console.error(error);
        this.addNotification("Error al guardar (Verifique token)", "error");
      }
    },
  },
};
</script>

<style scoped>
.component-card {
  padding: 20px;
  width: 400px;
  margin: 0 auto;
}
.dashed-border {
  border: 1px solid black; /* Solid black border */
  padding: 30px;
  border-radius: 8px; /* Optional: adds a bit of polish */
}
.title {
  text-align: center;
  color: #333; /* Black/Dark */
  font-weight: bold;
}
.form-group {
  margin-bottom: 15px;
}
label {
  color: #333; /* Black/Dark */
  font-weight: bold;
  display: block;
}
.form-control {
  width: 100%;
  padding: 8px;
  border: 1px solid #ccc;
  border-radius: 4px;
}
.btn-guardar {
  background-color: #007bff;
  color: white;
  border: none;
  padding: 10px 40px;
  border-radius: 5px;
  cursor: pointer;
}
.button-container {
  display: flex;
  justify-content: center;
}
</style>
