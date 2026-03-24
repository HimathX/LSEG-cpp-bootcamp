import { useState } from "react";
import { Header } from "./components/Header";
import { UploadSection } from "./components/UploadSection";
import { OrderBook } from "./components/OrderBook";
import { ExecutionBlotter } from "./components/ExecutionBlotter";
import type { ExecutionReport } from "./components/ExecutionBlotter";
import { VolumeChart } from "./components/VolumeChart";
import { QuickEntry } from "./components/QuickEntry";
import { motion } from "framer-motion";
import { parseExecutionCSV } from "./lib/csvParser";

export default function App() {
  const [reports, setReports] = useState<ExecutionReport[]>([]);
  const [volumeData, setVolumeData] = useState([
    { name: "Rose", volume: 0 },
    { name: "Lavender", volume: 0 },
    { name: "Lotus", volume: 0 },
    { name: "Tulip", volume: 0 },
    { name: "Orchid", volume: 0 },
  ]);

  // Mock data for the 5 order books
  const generateMockBooks = () => {
    return ["Rose", "Lavender", "Lotus", "Tulip", "Orchid"].map((instrument) => {
      const basePrice = Math.random() * 50 + 10;
      return {
        instrument,
        bids: Array.from({ length: 3 }).map((_, i) => ({ price: basePrice - i * 0.5, qty: Math.floor(Math.random() * 100) + 10 })),
        asks: Array.from({ length: 3 }).map((_, i) => ({ price: basePrice + 0.5 + i * 0.5, qty: Math.floor(Math.random() * 100) + 10 })),
      };
    });
  };

  const [books, setBooks] = useState(generateMockBooks());

  const handleRunComplete = (data: { summary: string; executionData: string; rejectedData: string }) => {
    
    // Parse the incoming CSV strings fully on the frontend
    const executions = parseExecutionCSV(data.executionData);
    const rejects = parseExecutionCSV(data.rejectedData);
    
    const allReports = [...executions, ...rejects].sort((a, b) => 
      a.transactionTime.localeCompare(b.transactionTime)
    );

    setReports(allReports);

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

    // Randomize books just to show matching activity visually 
    setBooks(generateMockBooks()); 
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

        {/* Middle Section: Order Books */}
        <motion.div 
          initial={{ opacity: 0 }}
          animate={{ opacity: 1 }}
          transition={{ duration: 0.5, delay: 0.2 }}
          className="grid grid-cols-2 lg:grid-cols-5 gap-4"
        >
          {books.map((book) => (
            <OrderBook 
              key={book.instrument} 
              instrument={book.instrument} 
              bids={book.bids} 
              asks={book.asks} 
            />
          ))}
        </motion.div>

        {/* Bottom Section: Dashboard Layout */}
        <div className="grid grid-cols-1 lg:grid-cols-4 gap-6 items-start">
          
          {/* Main Blotter spans 3 columns */}
          <motion.div 
            initial={{ opacity: 0, x: -20 }}
            animate={{ opacity: 1, x: 0 }}
            transition={{ duration: 0.5, delay: 0.4 }}
            className="lg:col-span-3 space-y-6"
          >
            <ExecutionBlotter reports={reports} />
            <VolumeChart data={volumeData} />
          </motion.div>

          {/* Sidebar spans 1 column */}
          <motion.div 
            initial={{ opacity: 0, x: 20 }}
            animate={{ opacity: 1, x: 0 }}
            transition={{ duration: 0.5, delay: 0.6 }}
            className="lg:col-span-1 h-[730px]"
          >
            <QuickEntry />
          </motion.div>
          
        </div>
      </main>
    </div>
  );
}