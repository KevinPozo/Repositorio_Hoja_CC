import { createRouter, createWebHashHistory } from "vue-router";

const routes = [
  {
    path: "/",
    redirect: "/productos/nuevo",
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
    path: "/productos/nuevo",
    name: "ProductoIngresar",
    component: () => import("../views/ProductoIngresarView.vue"),
    meta: { requiresAuth: true },
  },
  {
    path: "/ventas/nueva",
    name: "VentaRealizar",
    component: () => import("../views/VentaRealizarView.vue"),
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
