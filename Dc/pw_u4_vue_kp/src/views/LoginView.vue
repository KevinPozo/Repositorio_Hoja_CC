<template>
  <div class="login-page">
    <Login />
  </div>
</template>

<script>
import Login from "@/components/Login.vue";

export default {
  name: "LoginView",
  components: {
    Login,
  },
};
</script>

<style scoped>
.login-page {
  display: flex;
  justify-content: center;
  align-items: center;
  height: 100vh;
  font-family: "Avenir", Helvetica, Arial, sans-serif;
}
</style>
