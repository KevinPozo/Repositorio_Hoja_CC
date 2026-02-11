<template>
  <div class="container">
    <div class="stack">
      <div class="component-wrapper">
        <CitaFormulario @citaAgendada="actualizarLista" />
      </div>
      <div class="component-wrapper">
        <CitaLista :citas="listaCitas" />
      </div>
    </div>
  </div>
</template>

<script>
import CitaFormulario from "./CitaFormulario.vue";
import CitaLista from "./CitaLista.vue";
import { listarCitasFacade } from "../clients/Matriculaclient";

export default {
  components: {
    CitaFormulario,
    CitaLista,
  },
  data() {
    return {
      listaCitas: [],
    };
  },
  async mounted() {
    this.actualizarLista();
  },
  methods: {
    async actualizarLista() {
      const token = localStorage.getItem("token");
      if (token) {
        try {
          this.listaCitas = await listarCitasFacade(token);
        } catch (e) {
          console.error("Error al listar citas", e);
        }
      }
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
.stack {
  display: flex;
  flex-direction: column;
  gap: 30px;
}
.component-wrapper {
  width: 100%;
}
</style>
