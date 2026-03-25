import { useState } from "react";
import { Download } from "lucide-react";
import { Button } from "@/components/ui/button";
import { Table, TableBody, TableCell, TableHead, TableHeader, TableRow } from "@/components/ui/table";
import { Badge } from "@/components/ui/badge";
import { API_ENDPOINTS } from "../lib/api";

export interface ExecutionReport {
  orderId: string;
  clientOrderId: string;
  instrument: string;
  side: number; // 1: Buy, 2: Sell
  price: number;
  quantity: number;
  status: number; // 0=New, 1=Rejected, 2=Fill, 3=PFill
  reason?: string;
  transactionTime: string;
}

interface ExecutionBlotterProps {
  datasetVersion: number;
  executions: ExecutionReport[];
  rejections: ExecutionReport[];
}

export function ExecutionBlotter({ datasetVersion, executions, rejections }: ExecutionBlotterProps) {
  const getStatusChip = (status: number, reason?: string) => {
    switch (status) {
      case 0:
        return <Badge variant="outline" className="bg-blue-500/10 text-blue-500 hover:bg-blue-500/20">New</Badge>;
      case 1:
        return (
          <Badge variant="destructive" className="cursor-help" title={reason || "Unknown reason"}>
            Rejected
          </Badge>
        );
      case 2:
        return <Badge className="bg-green-500 text-green-950 hover:bg-green-600">Fill</Badge>;
      case 3:
        return <Badge variant="secondary" className="bg-amber-500 text-amber-950 hover:bg-amber-600">PFill</Badge>;
      default:
        return <Badge variant="outline">Unknown</Badge>;
    }
  };

  const getSideText = (side: number) => {
    return side === 1 
      ? <span className="text-green-500 font-bold">BUY</span> 
      : <span className="text-red-500 font-bold">SELL</span>;
  };

  const handleDownloadExecs = () => {
    window.open(API_ENDPOINTS.DOWNLOAD_EXECS, "_blank");
  };

  const handleDownloadRejects = () => {
    window.open(API_ENDPOINTS.DOWNLOAD_REJECTS, "_blank");
  };

  const [activeTab, setActiveTab] = useState<"executions" | "rejections">("executions");
  const activeReports = activeTab === "executions" ? executions : rejections;
  const tableKey = `${datasetVersion}-${activeTab}`;

  return (
    <div className="flex flex-col gap-4">
      <div className="flex justify-between items-end">
        <div className="space-y-4">
          <h2 className="text-xl font-bold text-foreground flex items-center gap-2">
            Execution Blotter
            <Badge variant="secondary" className="font-normal">
              {executions.length + rejections.length} Records
            </Badge>
          </h2>
          <div className="flex w-fit gap-1 rounded-md border border-border bg-card p-1">
            <Button
              size="sm"
              variant={activeTab === "executions" ? "default" : "ghost"}
              onClick={() => setActiveTab("executions")}
              className={activeTab === "executions" ? "bg-primary text-primary-foreground" : "text-muted-foreground hover:text-foreground"}
            >
              Executions ({executions.length})
            </Button>
            <Button
              size="sm"
              variant={activeTab === "rejections" ? "default" : "ghost"}
              onClick={() => setActiveTab("rejections")}
              className={activeTab === "rejections" ? "bg-primary text-primary-foreground" : "text-muted-foreground hover:text-foreground"}
            >
              Rejections ({rejections.length})
            </Button>
          </div>
        </div>
        <div className="flex gap-2">
          <Button
            size="sm"
            variant="outline"
            onClick={handleDownloadExecs}
          >
            <Download className="mr-2 h-4 w-4" /> Executions
          </Button>
          <Button
            size="sm"
            variant="outline"
            onClick={handleDownloadRejects}
          >
            <Download className="mr-2 h-4 w-4" /> Rejections
          </Button>
        </div>
      </div>

      <div className="w-full overflow-x-auto rounded-xl border border-border bg-card shadow-sm max-h-[400px]">
        <Table key={tableKey}>
          <TableHeader className="sticky top-0 z-10 bg-muted/90 backdrop-blur-md">
            <TableRow>
              <TableHead className="py-3 px-4 w-16">NO</TableHead>
              <TableHead className="py-3 px-4">TIME</TableHead>
              <TableHead className="py-3 px-4">ORDER ID</TableHead>
              <TableHead className="py-3 px-4">CLIENT ID</TableHead>
              <TableHead className="py-3 px-4">INSTRUMENT</TableHead>
              <TableHead className="py-3 px-4">SIDE</TableHead>
              <TableHead className="py-3 px-4 text-right">QTY</TableHead>
              <TableHead className="py-3 px-4 text-right">PRICE</TableHead>
              <TableHead className="py-3 px-4">STATUS</TableHead>
              {activeTab === "rejections" && (
                <TableHead className="py-3 px-4">REASON</TableHead>
              )}
            </TableRow>
          </TableHeader>
          <TableBody key={tableKey}>
            {activeReports.length === 0 ? (
              <TableRow>
                <TableCell colSpan={activeTab === "rejections" ? 10 : 9} className="h-24 text-center text-muted-foreground">
                  No {activeTab} available.
                </TableCell>
              </TableRow>
            ) : activeReports.map((report, index) => (
              <TableRow
                key={`${datasetVersion}-${activeTab}-${report.orderId}-${report.clientOrderId}-${report.transactionTime}-${report.status}-${report.quantity}-${report.price}-${index}`}
                className="font-mono text-sm group"
              >
                <TableCell className="text-muted-foreground">{index + 1}</TableCell>
                <TableCell className="text-muted-foreground">{report.transactionTime}</TableCell>
                <TableCell>{report.orderId}</TableCell>
                <TableCell className="text-muted-foreground">{report.clientOrderId}</TableCell>
                <TableCell className="font-sans font-medium">{report.instrument}</TableCell>
                <TableCell>{getSideText(report.side)}</TableCell>
                <TableCell className="text-right">{report.quantity}</TableCell>
                <TableCell className="text-right">{report.price.toFixed(2)}</TableCell>
                <TableCell>{getStatusChip(report.status, report.reason)}</TableCell>
                {activeTab === "rejections" && (
                  <TableCell className="text-orange-400 font-bold text-xs">{report.reason}</TableCell>
                )}
              </TableRow>
            ))}
          </TableBody>
        </Table>
      </div>
    </div>
  );
}
