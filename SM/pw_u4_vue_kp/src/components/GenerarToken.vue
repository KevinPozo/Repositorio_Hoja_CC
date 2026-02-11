<template>
  <div class="container">
    <div class="card">
      <h2 class="title">TOKEN</h2>

      <div class="form-group">
        <label>Semilla</label>
        <input
          v-model="semilla"
          type="text"
          class="form-control"
          placeholder="Ingrese semilla"
        />
      </div>

      <div class="form-group">
        <label>Tiempo de vigencia</label>
        <input
          v-model.number="tiempo"
          type="number"
          class="form-control"
          placeholder="Segundos"
        />
      </div>

      <div class="form-group">
        <label>Token</label>
        <textarea
          v-model="token"
          class="form-control token-area"
          readonly
        ></textarea>
      </div>

      <div class="button-container">
        <button @click="generarToken" class="btn-generar">Generar</button>
      </div>
    </div>
  </div>
</template>

<script>
import { generarTokenFacade } from "@/clients/InscripcionClient";

export default {
  inject: ["addNotification"],
  data() {
    return {
      semilla: "",
      tiempo: "",
      token: "",
    };
  },
  methods: {
    async generarToken() {
      if (!this.semilla || !this.tiempo) {
        this.addNotification("Ingrese semilla y tiempo", "error");
        return;
      }
      try {
        this.token = await generarTokenFacade(this.semilla, this.tiempo);
        this.addNotification("Token generado exitosamente", "info");
      } catch (error) {
        console.error(error);
        this.addNotification("Error al generar token", "error");
      }
    },
  },
};
</script>

<style scoped>
.container {
  display: flex;
  justify-content: center;
  margin-top: 50px;
}
.card {
  width: 400px;
  padding: 20px;
  border: 1px solid black;
  border-radius: 8px;
  box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1);
}
.title {
  text-align: center;
  color: #333; /* Black/Dark title */
  font-weight: bold;
}
.form-group {
  margin-bottom: 15px;
}
.label {
  color: #333; /* Black/Dark label */
  display: block;
  margin-bottom: 5px;
}
.form-control {
  width: 100%;
  padding: 8px;
  border: 1px solid #000;
  border-radius: 4px;
}
.token-area {
  height: 100px;
  resize: none;
}
.button-container {
  text-align: center;
}
.btn-generar {
  background-color: #007bff;
  color: white;
  border: none;
  padding: 10px 30px;
  border-radius: 5px;
  cursor: pointer;
  font-size: 16px;
}
.btn-generar:hover {
  background-color: #0056b3;
}
label {
  color: #333; /* Black/Dark label */
  font-weight: bold;
}
</style>
