<template>
  <div class="component-card dashed-border">
    <table class="table">
      <thead>
        <tr>
          <th>ID</th>
          <th>Nombre</th>
          <th>Apellido</th>
          <th>Cédula</th>
        </tr>
      </thead>
      <tbody>
        <tr v-for="est in estudiantes" :key="est.id">
          <td>{{ est.id }}</td>
          <td>{{ est.nombre }}</td>
          <td>{{ est.apellido }}</td>
          <td>{{ est.cedula }}</td>
        </tr>
      </tbody>
    </table>
  </div>
</template>

<script>
import { listarEstudiantesFacade } from "@/clients/InscripcionClient";

export default {
  props: {
    token: {
      type: String,
      required: false,
    },
  },
  data() {
    return {
      estudiantes: [],
    };
  },
  methods: {
    async listar(tokenToUse) {
      const t = tokenToUse || this.token;
      if (!t) return;

      try {
        this.estudiantes = await listarEstudiantesFacade(t);
      } catch (error) {
        console.error("Error al listar", error);
      }
    },
  },
};
</script>

<style scoped>
.component-card {
  width: 80%;
  margin: 20px auto;
}
.dashed-border {
  border: 1px solid black; /* Solid black border */
  padding: 20px;
  border-radius: 8px;
}
.table {
  width: 100%;
  border-collapse: collapse;
}
.table th,
.table td {
  border: 1px solid #333; /* Black borders */
  padding: 8px;
  text-align: left;
}
.table th {
  color: #333; /* Black text */
  font-weight: bold;
  text-align: center;
}
</style>
