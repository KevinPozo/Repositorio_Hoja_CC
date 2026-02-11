<template>
  <div class="page-container">
    <div class="component-wrapper">
      <EstudianteForm @estudiante-guardado="refreshList" ref="form" />
    </div>

    <div class="component-wrapper">
      <EstudianteList ref="list" />
    </div>
  </div>
</template>

<script>
import EstudianteForm from "../components/EstudianteForm.vue";
import EstudianteList from "../components/EstudianteList.vue";

export default {
  components: {
    EstudianteForm,
    EstudianteList,
  },
  methods: {
    refreshList() {
      const token = this.$refs.form.token;
      this.$refs.list.listar(token);
    },
  },
};
</script>

<style scoped>
.page-container {
  padding: 20px;
}
.component-wrapper {
  margin-bottom: 40px;
}
</style>
