import { useState, useEffect } from "react";
import { Clock } from "lucide-react";

export function Header() {
  const [time, setTime] = useState(new Date());

  useEffect(() => {
    const timer = setInterval(() => setTime(new Date()), 1000);
    return () => clearInterval(timer);
  }, []);

  return (
    <header className="sticky top-0 z-50 flex items-center justify-between px-6 py-3 border-b border-border bg-background/95 backdrop-blur supports-[backdrop-filter]:bg-background/60">
      <div className="flex items-center gap-4">
        <h1 className="text-xl font-bold tracking-widest text-foreground">
          FLOWER EXCHANGE TERMINAL
        </h1>
      </div>
      
      <div className="flex items-center gap-6">
        <div className="flex items-center gap-2 text-muted-foreground">
          <Clock size={16} className="text-green-500" />
          <span className="font-mono text-sm tracking-wider">
            {time.toLocaleTimeString("en-US", { hour12: false })}
          </span>
        </div>
        
        <div className="flex items-center gap-2">
          <div className="w-2 h-2 rounded-full bg-green-500 animate-pulse shadow-[0_0_8px_#22c55e]" />
          <span className="text-xs font-semibold tracking-wide text-muted-foreground uppercase">
            System Online
          </span>
        </div>
      </div>
    </header>
  );
}
