import { createRouter, createWebHashHistory } from "vue-router";

const routes = [
  {
    path: "/",
    redirect: "/citas/agendar",
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
    path: "/doctor/nuevo",
    name: "DoctorCrear",
    component: () => import("../views/DoctorCrearView.vue"),
    meta: { requiresAuth: true },
  },
  {
    path: "/paciente/nuevo",
    name: "PacienteCrear",
    component: () => import("../views/PacienteCrearView.vue"),
    meta: { requiresAuth: true },
  },
  {
    path: "/citas/agendar",
    name: "CitaAgendar",
    component: () => import("../views/CitaAgendarView.vue"),
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
