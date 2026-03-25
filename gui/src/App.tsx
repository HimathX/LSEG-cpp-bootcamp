import { useEffect, useState } from "react";
import { Header } from "./components/Header";
import { UploadSection } from "./components/UploadSection";
import { ExecutionBlotter } from "./components/ExecutionBlotter";
import type { ExecutionReport } from "./components/ExecutionBlotter";
import { VolumeChart } from "./components/VolumeChart";
import { motion } from "framer-motion";

type ThemeMode = "light" | "dark";

const THEME_STORAGE_KEY = "flower-exchange-theme";

export default function App() {
  const [theme, setTheme] = useState<ThemeMode>(() => {
    if (typeof window === "undefined") {
      return "light";
    }

    const savedTheme = window.localStorage.getItem(THEME_STORAGE_KEY);
    return savedTheme === "dark" ? "dark" : "light";
  });
  const [executionsList, setExecutionsList] = useState<ExecutionReport[]>([]);
  const [rejectionsList, setRejectionsList] = useState<ExecutionReport[]>([]);
  const [summary, setSummary] = useState<string | null>(null);
  const [datasetVersion, setDatasetVersion] = useState(0);
  const [volumeData, setVolumeData] = useState([
    { name: "Rose", volume: 0 },
    { name: "Lavender", volume: 0 },
    { name: "Lotus", volume: 0 },
    { name: "Tulip", volume: 0 },
    { name: "Orchid", volume: 0 },
  ]);

  useEffect(() => {
    document.documentElement.classList.toggle("dark", theme === "dark");
    window.localStorage.setItem(THEME_STORAGE_KEY, theme);
  }, [theme]);

  const handleRunComplete = (data: { summary: string; executionData: ExecutionReport[]; rejectedData: ExecutionReport[] }) => {
    // Data is now pre-parsed by the backend into JSON
    const executions = data.executionData || [];
    const rejects = data.rejectedData || [];

    setSummary(data.summary);
    setExecutionsList([...executions].sort((a, b) => a.transactionTime.localeCompare(b.transactionTime)));
    setRejectionsList([...rejects].sort((a, b) => a.transactionTime.localeCompare(b.transactionTime)));
    setDatasetVersion((current) => current + 1);

    // Properly compute executed volumes! (Avoid double-counting buyer and seller leg)
    const volMap: Record<string, number> = {
      Rose: 0, Lavender: 0, Lotus: 0, Tulip: 0, Orchid: 0
    };
    
    executions.forEach((rep) => {
      // We only count volume once for trades. We can just pick BUY side.
      if ((rep.status === 2 || rep.status === 3) && rep.side === 1) {
        if (volMap[rep.instrument] !== undefined) {
          volMap[rep.instrument] += rep.quantity;
        }
      }
    });

    setVolumeData([
      { name: "Rose", volume: volMap["Rose"] },
      { name: "Lavender", volume: volMap["Lavender"] },
      { name: "Lotus", volume: volMap["Lotus"] },
      { name: "Tulip", volume: volMap["Tulip"] },
      { name: "Orchid", volume: volMap["Orchid"] },
    ]);
  };

  const toggleTheme = () => {
    setTheme((currentTheme) => (currentTheme === "dark" ? "light" : "dark"));
  };

  return (
    <div className="min-h-screen bg-background text-foreground font-sans selection:bg-primary/20 flex flex-col">
      <Header theme={theme} onToggleTheme={toggleTheme} />

      <main className="flex-1 p-6 max-w-[1600px] mx-auto w-full space-y-6">
        {/* Top Section: Upload */}
        <motion.div
          initial={{ opacity: 0, y: -20 }}
          animate={{ opacity: 1, y: 0 }}
          transition={{ duration: 0.5 }}
        >
          <UploadSection onRunComplete={handleRunComplete} />
        </motion.div>

        {/* Engine Summary */}
        {summary && (
          <motion.div
            initial={{ opacity: 0, scale: 0.95 }}
          animate={{ opacity: 1, scale: 1 }}
          transition={{ duration: 0.3 }}
          className="w-full"
        >
            <div className="rounded-xl border border-border bg-card p-4 text-sm font-mono text-card-foreground whitespace-pre-wrap shadow-sm">
              <span className="mb-2 block border-b border-border pb-2 font-sans text-xs font-bold tracking-wider text-muted-foreground">
                MATCHING ENGINE OUTPUT
              </span>
              {summary}
            </div>
          </motion.div>
        )}

        {/* Bottom Section: Dashboard Layout */}
        <motion.div
          initial={{ opacity: 0, y: 20 }}
          animate={{ opacity: 1, y: 0 }}
          transition={{ duration: 0.5, delay: 0.2 }}
          className="space-y-6"
        >
          <ExecutionBlotter
            datasetVersion={datasetVersion}
            executions={executionsList}
            rejections={rejectionsList}
          />
          <VolumeChart data={volumeData} theme={theme} />
        </motion.div>
      </main>
    </div>
  );
}
