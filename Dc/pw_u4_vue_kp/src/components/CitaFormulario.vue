<template>
  <div class="form-component">
    <h3>Agendar Cita</h3>
    <div class="form-group">
      <label>Cédula Doctor:</label>
      <input v-model="cita.cedulaDoctor" type="text" />
    </div>
    <div class="form-group">
      <label>Cédula Paciente:</label>
      <input v-model="cita.cedulaPaciente" type="text" />
    </div>
    <div class="form-group">
      <label>Fecha Cita:</label>
      <input v-model="cita.fechaCita" type="datetime-local" />
    </div>
    <button @click="agendar" class="btn-agendar">Agendar</button>
    <div v-if="mensaje" :class="['mensaje', tipoMensaje]">{{ mensaje }}</div>
  </div>
</template>

<script>
import { agendarCitaFacade } from "../clients/Matriculaclient";

export default {
  data() {
    return {
      cita: {
        cedulaDoctor: "",
        cedulaPaciente: "",
        fechaCita: "",
      },
      mensaje: "",
      tipoMensaje: "",
    };
  },
  methods: {
    async agendar() {
      const token = localStorage.getItem("token");
      if (!token) {
        this.mensaje = "Debe iniciar sesión";
        this.tipoMensaje = "error";
        return;
      }

      let body = { ...this.cita };
      if (body.fechaCita && body.fechaCita.length === 16) {
        body.fechaCita += ":00";
      }

      try {
        await agendarCitaFacade(body, token);
        this.mensaje = "Cita agendada";
        this.tipoMensaje = "success";
        this.$emit("citaAgendada");
        this.limpiar();
      } catch (e) {
        console.error(e);
        this.mensaje = "Error al agendar (Verifique cédulas)";
        this.tipoMensaje = "error";
      }
      setTimeout(() => (this.mensaje = ""), 5000);
    },
    limpiar() {
      this.cita = { cedulaDoctor: "", cedulaPaciente: "", fechaCita: "" };
    },
  },
};
</script>

<style scoped>
.form-component {
  background-color: #fff;
  padding: 30px;
  border-radius: 8px;
  box-shadow: 0 2px 5px rgba(0, 0, 0, 0.1);
}
h3 {
  color: #343a40;
  margin-bottom: 20px;
  text-align: center;
}
.form-group {
  margin-bottom: 15px;
  text-align: left;
}
label {
  display: block;
  font-weight: 600;
  margin-bottom: 5px;
  color: #495057;
}
input {
  width: 100%;
  padding: 8px 12px;
  border: 1px solid #ced4da;
  border-radius: 4px;
  font-size: 14px;
  transition: border-color 0.15s ease-in-out;
}
input:focus {
  border-color: #34495e;
  outline: 0;
}
.btn-agendar {
  display: block;
  width: auto;
  min-width: 200px;
  margin: 20px auto 0;
  text-align: center;
  cursor: pointer;
  background-color: #34495e;
  border: 1px solid #34495e;
  color: #fff;
  padding: 8px 16px;
  font-size: 16px;
  border-radius: 4px;
  transition: background-color 0.15s;
}
.btn-agendar:hover {
  background-color: #5d6d7e;
}
.mensaje {
  margin-top: 15px;
  padding: 10px;
  border-radius: 4px;
  text-align: center;
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
