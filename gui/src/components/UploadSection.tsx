import { useState } from "react";
import { Card, CardContent } from "@/components/ui/card";
import { Button } from "@/components/ui/button";
import { Input } from "@/components/ui/input";
import { Upload, Play, Loader2 } from "lucide-react";
import { API_ENDPOINTS } from "../lib/api";
import type { ExecutionReport } from "./ExecutionBlotter";

interface UploadSectionProps {
  onRunComplete: (data: { summary: string; executionData: ExecutionReport[]; rejectedData: ExecutionReport[] }) => void;
}

export function UploadSection({ onRunComplete }: UploadSectionProps) {
  const [file, setFile] = useState<File | null>(null);
  const [isRunning, setIsRunning] = useState(false);

  const handleRunEngine = async () => {
    if (!file) {
      if (!import.meta.env.DEV) {
        alert("Please select a file first.");
        return;
      }
      return; 
    }

    setIsRunning(true);
    
    try {
      const formData = new FormData();
      formData.append("file", file);

      const response = await fetch(API_ENDPOINTS.EXECUTE, {
        method: "POST",
        body: formData,
      });

      if (!response.ok) {
        throw new Error(`API error: ${response.statusText}`);
      }

      const data = await response.json();
      if (!data.success) {
        throw new Error(data.error || "Failed to execute");
      }

      onRunComplete({
        summary: data.summary,
        executionData: data.executionData,
        rejectedData: data.rejectedData,
      });
      
    } catch (error) {
      console.error("Execution failed:", error);
      alert("Failed to run engine. Make sure the FastAPI server is running.");
    } finally {
      setIsRunning(false);
    }
  };

  return (
    <Card className="border-border bg-card">
      <CardContent className="flex flex-col md:flex-row gap-4 items-center justify-between p-6">
        <div className="flex-1 w-full">
          <h2 className="text-lg font-semibold text-card-foreground mb-2 flex items-center gap-2">
            <Upload size={18} className="text-muted-foreground" />
            Upload Orders
          </h2>
          <p className="text-sm text-muted-foreground mb-4">
            Select an orders.csv file to process through the matching engine.
          </p>
          
          <div className="w-full max-w-sm">
            <Input 
              type="file" 
              accept=".csv"
              onChange={(e) => setFile(e.target.files?.[0] || null)}
              className="text-sm"
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
