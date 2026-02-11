import axios from "./axios-config";
const URL_DOCTOR = "http://localhost:8081/matricula/api/v1.0/doctores";
const URL_PACIENTE = "http://localhost:8081/matricula/api/v1.0/pacientes";
const URL_CITA = "http://localhost:8081/matricula/api/v1.0/citas";

const guardarDoctor = async (body, token) => {
  return axios
    .post(URL_DOCTOR, body, { headers: { Authorization: `Bearer ${token}` } })
    .then((r) => r.data);
};
const guardarPaciente = async (body, token) => {
  return axios
    .post(URL_PACIENTE, body, { headers: { Authorization: `Bearer ${token}` } })
    .then((r) => r.data);
};
const agendarCita = async (body, token) => {
  return axios
    .post(URL_CITA, body, { headers: { Authorization: `Bearer ${token}` } })
    .then((r) => r.data);
};
const listarCitas = async (token) => {
  return axios
    .get(URL_CITA, { headers: { Authorization: `Bearer ${token}` } })
    .then((r) => r.data);
};

export const guardarDoctorFacade = async (body, token) =>
  await guardarDoctor(body, token);
export const guardarPacienteFacade = async (body, token) =>
  await guardarPaciente(body, token);
export const agendarCitaFacade = async (body, token) =>
  await agendarCita(body, token);
export const listarCitasFacade = async (token) => await listarCitas(token);
