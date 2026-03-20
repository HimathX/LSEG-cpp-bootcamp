import React from "react";
import ReactDOM from "react-dom/client";
import App from "./App";
import "./index.css";

ReactDOM.createRoot(document.getElementById("root")!).render(
  <React.StrictMode>
    {/* Adding 'dark' class here gives it that sleek LSEG terminal look */}
    <main className="dark text-foreground bg-background min-h-screen">
      <App />
    </main>
  </React.StrictMode>
);