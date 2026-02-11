import axios from "axios";

const URL_AUTH = "http://localhost:8082/auth";
const URL_API = "http://localhost:8081/matricula/api/v1.0";

export const generarTokenFacade = async (semilla, tiempo) => {
  const response = await axios.post(`${URL_AUTH}/generate-token`, null, {
    params: {
      semilla: semilla,
      tiempo: tiempo,
    },
  });
  return response.data.accessToken;
};

export const guardarEstudianteFacade = async (estudiante, token) => {
  const response = await axios.post(`${URL_API}/estudiantes`, estudiante, {
    headers: {
      Authorization: `Bearer ${token}`,
    },
  });
  return response.data;
};

export const listarEstudiantesFacade = async (token) => {
  const response = await axios.get(`${URL_API}/estudiantes`, {
    headers: {
      Authorization: `Bearer ${token}`,
    },
  });
  return response.data;
};
