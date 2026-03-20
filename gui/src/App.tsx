import { useState } from "react";
import { Header } from "./components/Header";
import { UploadSection } from "./components/UploadSection";
import { OrderBook } from "./components/OrderBook";
import { ExecutionBlotter } from "./components/ExecutionBlotter";
import type { ExecutionReport } from "./components/ExecutionBlotter";
import { VolumeChart } from "./components/VolumeChart";
import { QuickEntry } from "./components/QuickEntry";
import { motion } from "framer-motion";

export default function App() {
  const [reports, setReports] = useState<ExecutionReport[]>([]);
  const [hasRun, setHasRun] = useState(false);

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

  const handleRunComplete = () => {
    setHasRun(true);
    // Set some mock execution reports to simulate the parsed CSV
    const mockReports: ExecutionReport[] = [
      { orderId: "ord_001", clientOrderId: "cl_1001", instrument: "Rose", side: 1, price: 55.5, quantity: 100, status: 2, transactionTime: "09:30:01.123" },
      { orderId: "ord_002", clientOrderId: "cl_1002", instrument: "Tulip", side: 2, price: 42.0, quantity: 50, status: 1, reason: "Insufficient funds", transactionTime: "09:30:02.450" },
      { orderId: "ord_003", clientOrderId: "cl_1003", instrument: "Lotus", side: 1, price: 88.2, quantity: 200, status: 3, transactionTime: "09:30:04.991" },
      { orderId: "ord_004", clientOrderId: "cl_1004", instrument: "Orchid", side: 2, price: 120.0, quantity: 10, status: 0, transactionTime: "09:30:05.100" },
      { orderId: "ord_005", clientOrderId: "cl_1005", instrument: "Lavender", side: 1, price: 30.5, quantity: 500, status: 2, transactionTime: "09:30:06.002" },
      { orderId: "ord_006", clientOrderId: "cl_1006", instrument: "Rose", side: 2, price: 55.6, quantity: 100, status: 2, transactionTime: "09:30:07.123" },
    ];
    setReports(mockReports);
    setBooks(generateMockBooks()); // Randomize books to show activity
  };

  const mockVolumeData = [
    { name: "Rose", volume: hasRun ? 4000 : 0 },
    { name: "Lavender", volume: hasRun ? 3200 : 0 },
    { name: "Lotus", volume: hasRun ? 2800 : 0 },
    { name: "Tulip", volume: hasRun ? 1500 : 0 },
    { name: "Orchid", volume: hasRun ? 500 : 0 },
  ];

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
            <VolumeChart data={mockVolumeData} />
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