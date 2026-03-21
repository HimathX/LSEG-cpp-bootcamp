import { useState } from "react";
import { Card, CardContent } from "@/components/ui/card";
import { Button } from "@/components/ui/button";
import { Input } from "@/components/ui/input";
import { Upload, Play, Loader2 } from "lucide-react";

interface UploadSectionProps {
  onRunComplete: () => void;
}

export function UploadSection({ onRunComplete }: UploadSectionProps) {
  const [file, setFile] = useState<File | null>(null);
  const [isRunning, setIsRunning] = useState(false);

  const handleRunEngine = async () => {
    if (!file && !import.meta.env.DEV) {
      // Allow running without file in dev/demo mode purely for UI testing
      // alert("Please select a file first.");
      // return;
    }

    setIsRunning(true);
    // Placeholder to call Node.js proxy to execute 'exchange.exe'
    
    // Simulate engine processing delay
    setTimeout(() => {
      setIsRunning(false);
      onRunComplete();
    }, 2000);
  };

  return (
    <Card className="bg-zinc-900 border-white/10">
      <CardContent className="flex flex-col md:flex-row gap-4 items-center justify-between p-6">
        <div className="flex-1 w-full">
          <h2 className="text-lg font-semibold text-slate-200 mb-2 flex items-center gap-2">
            <Upload size={18} className="text-slate-400" />
            Upload Orders
          </h2>
          <p className="text-sm text-slate-500 mb-4">
            Select an orders.csv file to process through the matching engine.
          </p>
          
          <div className="w-full max-w-sm">
            <Input 
              type="file" 
              accept=".csv"
              onChange={(e) => setFile(e.target.files?.[0] || null)}
              className="text-sm bg-zinc-900 text-slate-200 border-white/10"
            />
          </div>
        </div>

        <div className="flex shrink-0">
          <Button
            size="lg"
            className="font-bold tracking-wide shadow-lg min-w-[200px] bg-blue-600 hover:bg-blue-700 text-white"
            onClick={handleRunEngine}
            disabled={isRunning}
          >
            {isRunning ? (
              <span className="flex items-center gap-2">
                <Loader2 size={18} className="animate-spin" />
                Processing trades...
              </span>
            ) : (
              <span className="flex items-center gap-2">
                <Play size={18} />
                RUN MATCHING ENGINE
              </span>
            )}
          </Button>
        </div>
      </CardContent>
    </Card>
  );
}
