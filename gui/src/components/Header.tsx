import { useState, useEffect } from "react";
import { Clock } from "lucide-react";

export function Header() {
  const [time, setTime] = useState(new Date());

  useEffect(() => {
    const timer = setInterval(() => setTime(new Date()), 1000);
    return () => clearInterval(timer);
  }, []);

  return (
    <header className="flex items-center justify-between px-6 py-3 border-b border-white/10 bg-black/20 backdrop-blur-md">
      <div className="flex items-center gap-4">
        <h1 className="text-xl font-bold tracking-widest text-slate-100">
          FLOWER EXCHANGE TERMINAL
        </h1>
      </div>
      
      <div className="flex items-center gap-6">
        <div className="flex items-center gap-2 text-slate-300">
          <Clock size={16} className="text-success" />
          <span className="font-mono text-sm tracking-wider">
            {time.toLocaleTimeString("en-US", { hour12: false })}
          </span>
        </div>
        
        <div className="flex items-center gap-2">
          <div className="w-2 h-2 rounded-full bg-success animate-pulse shadow-[0_0_8px_#10b981]" />
          <span className="text-xs font-semibold tracking-wide text-slate-400 uppercase">
            System Online
          </span>
        </div>
      </div>
    </header>
  );
}
