import axios from "axios";

const authAxios = axios.create({
  baseURL: "http://localhost:8082/auth",
});

const getToken = async (username, password) => {
  return authAxios
    .get("/token", {
      params: {
        user: username,
        password: password,
      },
    })
    .then((response) => response.data.accessToken);
};

export const getTokenFacade = async (username, password) => {
  return await getToken(username, password);
};
