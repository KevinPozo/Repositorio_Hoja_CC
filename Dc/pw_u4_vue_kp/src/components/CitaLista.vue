<template>
  <div class="lista-component">
    <h3>Citas Agendadas</h3>
    <table>
      <thead>
        <tr>
          <th>Nombre Paciente</th>
          <th>Apellido Paciente</th>
          <th>Cédula Paciente</th>
          <th>Fecha Cita</th>
          <th>Nombre Doctor</th>
        </tr>
      </thead>
      <tbody>
        <tr v-for="cita in citas" :key="cita.id">
          <td>{{ cita.nombrePaciente }}</td>
          <td>{{ cita.apellidoPaciente }}</td>
          <td>{{ cita.cedulaPaciente }}</td>
          <td>{{ cita.fechaCita }}</td>
          <td>{{ cita.nombreDoctor }}</td>
        </tr>
        <tr v-if="citas.length === 0">
          <td colspan="5">No hay citas agendadas</td>
        </tr>
      </tbody>
    </table>
  </div>
</template>

<script>
export default {
  props: {
    citas: {
      type: Array,
      required: true,
    },
  },
};
</script>

<style scoped>
.lista-component {
  margin-top: 20px;
}
h3 {
  color: #343a40;
  margin-bottom: 20px;
  text-align: center;
}
table {
  width: 100%;
  border-collapse: collapse;
  background-color: #fff;
  box-shadow: 0 1px 3px rgba(0, 0, 0, 0.1);
}
th {
  background-color: #007bff;
  color: white;
  font-weight: 600;
  padding: 12px;
  text-align: left;
}
td {
  padding: 10px 12px;
  border-bottom: 1px solid #dee2e6;
  color: #212529;
}
tr:nth-child(even) {
  background-color: #f8f9fa;
}
tr:hover {
  background-color: #e9ecef;
}
</style>
