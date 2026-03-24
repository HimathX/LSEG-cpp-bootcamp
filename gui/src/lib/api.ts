// Base URL can be configured via environment variables (.env files in Vite)
export const API_BASE_URL = import.meta.env.VITE_API_URL || "http://127.0.0.1:8001";

export const API_ENDPOINTS = {
  EXECUTE: `${API_BASE_URL}/api/execute`,
};
