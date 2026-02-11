<template>
  <div class="container">
    <h2>Crear Nuevo Doctor</h2>
    <div class="form-container">
      <div v-if="mensaje" :class="['mensaje', tipoMensaje]">
        {{ mensaje }}
      </div>

      <div class="form-group">
        <label>Cédula:</label>
        <input
          v-model="doctor.cedula"
          type="text"
          placeholder="Ingrese cédula"
        />
      </div>
      <div class="form-group">
        <label>Nombre:</label>
        <input
          v-model="doctor.nombre"
          type="text"
          placeholder="Ingrese nombre"
        />
      </div>
      <div class="form-group">
        <label>Apellido:</label>
        <input
          v-model="doctor.apellido"
          type="text"
          placeholder="Ingrese apellido"
        />
      </div>
      <div class="form-group">
        <label>Género:</label>
        <select v-model="doctor.genero">
          <option disabled value="">Seleccione género</option>
          <option value="Hombre">Hombre</option>
          <option value="Mujer">Mujer</option>
        </select>
      </div>
      <button @click="guardar" class="btn-guardar">Guardar</button>
    </div>
  </div>
</template>

<script>
import { guardarDoctorFacade } from "../clients/Matriculaclient";

export default {
  data() {
    return {
      doctor: {
        cedula: "",
        nombre: "",
        apellido: "",
        genero: "",
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
      try {
        await guardarDoctorFacade(this.doctor, token);
        this.mostrarMensaje("Doctor guardado exitosamente", "success");
        this.limpiar();
      } catch (e) {
        console.error(e);
        this.mostrarMensaje("Error al guardar doctor", "error");
      }
    },
    mostrarMensaje(msg, type) {
      this.mensaje = msg;
      this.tipoMensaje = type;
      setTimeout(() => {
        this.mensaje = "";
      }, 10000);
    },
    limpiar() {
      this.doctor = { cedula: "", nombre: "", apellido: "", genero: "" };
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
input,
select {
  width: 100%;
  padding: 10px;
  border: 1px solid #ced4da;
  border-radius: 4px;
  font-size: 16px;
  box-sizing: border-box;
}
input:focus,
select:focus {
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
