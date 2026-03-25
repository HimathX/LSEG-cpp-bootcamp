import { useState } from "react";
import { Header } from "./components/Header";
import { UploadSection } from "./components/UploadSection";
import { ExecutionBlotter } from "./components/ExecutionBlotter";
import type { ExecutionReport } from "./components/ExecutionBlotter";
import { VolumeChart } from "./components/VolumeChart";
import { motion } from "framer-motion";

export default function App() {
  const [executionsList, setExecutionsList] = useState<ExecutionReport[]>([]);
  const [rejectionsList, setRejectionsList] = useState<ExecutionReport[]>([]);
  const [summary, setSummary] = useState<string | null>(null);
  const [volumeData, setVolumeData] = useState([
    { name: "Rose", volume: 0 },
    { name: "Lavender", volume: 0 },
    { name: "Lotus", volume: 0 },
    { name: "Tulip", volume: 0 },
    { name: "Orchid", volume: 0 },
  ]);

  const handleRunComplete = (data: { summary: string; executionData: ExecutionReport[]; rejectedData: ExecutionReport[] }) => {
    
    // Data is now pre-parsed by the backend into JSON
    const executions = data.executionData || [];
    const rejects = data.rejectedData || [];
    
    setSummary(data.summary);
    setExecutionsList(executions.sort((a, b) => a.transactionTime.localeCompare(b.transactionTime)));
    setRejectionsList(rejects.sort((a, b) => a.transactionTime.localeCompare(b.transactionTime)));

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

  return (
    <div className="min-h-screen bg-slate-950 text-slate-50 font-sans selection:bg-success/30 display-flex flex-col">
      <Header />
      
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
            <div className="bg-zinc-900 border border-white/10 p-4 rounded-xl text-sm font-mono text-zinc-300 whitespace-pre-wrap shadow-sm">
              <span className="text-zinc-500 block mb-2 font-sans text-xs font-bold tracking-wider border-b border-white/5 pb-2">
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
          <ExecutionBlotter executions={executionsList} rejections={rejectionsList} />
          <VolumeChart data={volumeData} />
        </motion.div>
      </main>
    </div>
  );
}