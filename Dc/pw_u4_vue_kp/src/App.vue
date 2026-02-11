<template>
  <div id="app">
    <nav v-if="!$route.path.includes('/login') && isAuthenticated">
      <router-link to="/doctor/nuevo">Nuevo Doctor</router-link> |
      <router-link to="/paciente/nuevo">Nuevo Paciente</router-link> |
      <router-link to="/citas/agendar">Agendar Citas</router-link>
    </nav>
    <NotificationContainer
      :notifications="notifications"
      @remove="removeNotification"
    />
    <router-view />
  </div>
</template>

<script>
import NotificationContainer from "@/components/NotificationContainer.vue";

export default {
  name: "App",
  components: {
    NotificationContainer,
  },
  data() {
    return {
      notifications: [],
      isAuthenticated: false,
    };
  },
  provide() {
    return {
      addNotification: this.addNotification,
    };
  },
  mounted() {
    this.isAuthenticated = !!localStorage.getItem("token");
  },
  methods: {
    removeNotification(id) {
      const index = this.notifications.findIndex((n) => n.id === id);
      if (index !== -1) {
        this.notifications.splice(index, 1);
      }
    },
    addNotification(message, type = "info") {
      const id = Date.now();
      this.notifications.push({ id, message, type });
    },
  },
  watch: {
    $route() {
      this.notifications = [];
      this.isAuthenticated = !!localStorage.getItem("token");
    },
  },
};
</script>

<style>
#app {
  font-family: Avenir, Helvetica, Arial, sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  text-align: center;
  color: #2c3e50;
}

nav {
  padding: 30px;
}

nav a {
  font-weight: bold;
  color: #2c3e50;
}

nav a.router-link-exact-active {
  color: #42b983;
}
</style>
