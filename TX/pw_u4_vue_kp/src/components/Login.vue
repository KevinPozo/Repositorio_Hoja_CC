<template>
  <div class="login-card">
    <h2>Inicio de Sesión</h2>
    <p class="subtitle">Ingrese sus credenciales</p>

    <div class="form-group">
      <label for="username">Usuario</label>
      <input
        type="text"
        id="username"
        v-model="username"
        placeholder="Usuario"
        @keyup.enter="login"
      />
    </div>

    <div class="form-group">
      <label for="password">Contraseña</label>
      <input
        type="password"
        id="password"
        v-model="password"
        placeholder="Contraseña"
        @keyup.enter="login"
      />
      <p v-if="error" class="error-msg">{{ error }}</p>
    </div>

    <button @click="login" :disabled="loading" class="login-btn">
      {{ loading ? "Cargando..." : "Ingresar" }}
    </button>
  </div>
</template>

<script>
import { getTokenFacade } from "../clients/AuthClient";

export default {
  name: "Login",
  inject: ["addNotification"],
  data() {
    return {
      username: "",
      password: "",
      loading: false,
      error: null,
    };
  },
  methods: {
    async login() {
      this.error = null;

      if (!this.username || !this.password) {
        this.error = "Por favor ingrese usuario y contraseña.";
        return;
      }

      this.loading = true;

      try {
        const token = await getTokenFacade(this.username, this.password);
        if (token) {
          localStorage.setItem("token", token);
          localStorage.setItem("estaAutenticado", true);
          localStorage.setItem("username", this.username);

          this.addNotification(`Bienvenido, ${this.username}!`, "success");
          this.$router.push("/");
        } else {
          this.error = "Credenciales incorrectas.";
        }
      } catch (err) {
        console.error(err);
        this.addNotification(
          "Error de conexión o credenciales inválidas.",
          "error",
        );
      } finally {
        this.loading = false;
      }
    },
  },
};
</script>

<style scoped>
.login-card {
  width: 300px;
  padding: 20px;
  border: 1px solid black;
  border-radius: 8px;
  text-align: center;
}
h2 {
  margin: 0 0 10px;
  color: #2c3e50;
  font-size: 28px;
}

.subtitle {
  color: #7f8c8d;
  font-size: 16px;
  margin-bottom: 30px;
}

.form-group {
  margin-bottom: 25px;
  text-align: left;
}

label {
  display: block;
  margin-bottom: 10px;
  color: #34495e;
  font-weight: 600;
  font-size: 15px;
}

input {
  width: 100%;
  padding: 14px 16px;
  border: 1px solid #dcdcdc;
  border-radius: 8px;
  font-size: 16px;
  transition: border-color 0.2s;
  box-sizing: border-box;
  background-color: #f9fafb;
}

input:focus {
  border-color: #52475c;
  outline: none;
  box-shadow: 0 0 0 3px rgba(118, 75, 162, 0.1);
  background-color: white;
}

.error-msg {
  color: #e74c3c;
  font-size: 14px;
  margin-top: 8px;
  text-align: left;
  font-weight: 500;
}

.login-btn {
  width: 100%;
  padding: 16px;
  background-color: #34495e;
  color: white;
  border: none;
  border-radius: 8px;
  font-size: 18px;
  font-weight: 600;
  cursor: pointer;
  margin-top: 20px;
}

.login-btn:hover:not(:disabled) {
  background-color: #667eea;
  transform: translateY(-2px);
  box-shadow: 0 5px 15px rgba(102, 126, 234, 0.4);
}

.login-btn:disabled {
  background-color: #a8d5c2;
  cursor: not-allowed;
  transform: none;
  box-shadow: none;
}
</style>
