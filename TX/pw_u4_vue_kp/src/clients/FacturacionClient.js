import axios from "./axios-config";

const URL_PRODUCTOS = "http://localhost:8081/matricula/api/v1.0/productos";
const URL_VENTAS = "http://localhost:8081/matricula/api/v1.0/ventas";

const ingresarProducto = async (body, token) => {
  return axios
    .post(URL_PRODUCTOS, body, {
      headers: { Authorization: `Bearer ${token}` },
    })
    .then((r) => r.data);
};

const buscarProducto = async (codigo, token) => {
  return axios
    .get(`${URL_PRODUCTOS}/${codigo}`, {
      headers: { Authorization: `Bearer ${token}` },
    })
    .then((r) => r.data)
    .catch((e) => null);
};

const realizarVenta = async (body, token) => {
  return axios
    .post(URL_VENTAS, body, { headers: { Authorization: `Bearer ${token}` } })
    .then((r) => r.data);
};

export const ingresarProductoFacade = async (body, token) =>
  await ingresarProducto(body, token);
export const buscarProductoFacade = async (codigo, token) =>
  await buscarProducto(codigo, token);
export const realizarVentaFacade = async (body, token) =>
  await realizarVenta(body, token);
