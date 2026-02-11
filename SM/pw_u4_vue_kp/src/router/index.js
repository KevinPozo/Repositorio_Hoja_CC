import { createRouter, createWebHashHistory } from "vue-router";

const routes = [
  {
    path: "/",
    redirect: "/generar-token",
    meta: {
      requiresAuth: true,
    },
  },
  {
    path: "/login",
    name: "login",
    component: () => import("../views/LoginView.vue"),
  },
  {
    path: "/generar-token",
    name: "GenerarToken",
    component: () => import("../views/GenerarTokenView.vue"),
    meta: { requiresAuth: true },
  },
  {
    path: "/guardar-estudiante",
    name: "GuardarEstudiante",
    component: () => import("../views/GuardarEstudianteView.vue"),
    meta: { requiresAuth: true },
  },
];
const router = createRouter({
  history: createWebHashHistory(),
  routes,
});

router.beforeEach((to, from, next) => {
  if (to.meta.requiresAuth) {
    const estaAutenticado = localStorage.getItem("estaAutenticado");
    const token = localStorage.getItem("token");

    if (!estaAutenticado || !token) {
      console.log("Redirigiendo al Login");
      next({ name: "login" });
    } else {
      next();
    }
  } else {
    console.log("Pase libre");
    next();
  }
});

export default router;
