import { Table, TableHeader, TableColumn, TableBody, TableRow, TableCell, Button } from "@heroui/react";
import { Download } from "lucide-react";

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
  reports: ExecutionReport[];
}

export function ExecutionBlotter({ reports }: ExecutionBlotterProps) {
  
  const getStatusChip = (status: number, reason?: string) => {
    switch (status) {
      case 0:
        return <span className="px-2 py-0.5 rounded-full text-xs font-semibold bg-blue-500/20 text-blue-400 border border-blue-500/30">New</span>;
      case 1:
        return (
          <span className="px-2 py-0.5 rounded-full text-xs font-semibold bg-rose-500/20 text-rose-400 border border-rose-500/30 cursor-help" title={reason || "Unknown reason"}>
            Rejected
          </span>
        );
      case 2:
        return <span className="px-2 py-0.5 rounded-full text-xs font-semibold bg-emerald-500 text-black">Fill</span>;
      case 3:
        return <span className="px-2 py-0.5 rounded-full text-xs font-semibold bg-amber-500 text-black">PFill</span>;
      default:
        return <span className="px-2 py-0.5 rounded-full text-xs font-semibold bg-slate-500/20 text-slate-400">Unknown</span>;
    }
  };

  const getSideText = (side: number) => {
    return side === 1 
      ? <span className="text-success font-bold">BUY</span> 
      : <span className="text-danger font-bold">SELL</span>;
  };

  const handleDownload = () => {
    if (reports.length === 0) return;
    
    const headers = "OrderID,ClientOrderID,Instrument,Side,Price,Quantity,Status,Reason,Time\n";
    const csvContent = reports.map(r => 
      `${r.orderId},${r.clientOrderId},${r.instrument},${r.side === 1 ? 'Buy' : 'Sell'},${r.price},${r.quantity},${r.status},${r.reason || ''},${r.transactionTime}`
    ).join("\n");
    
    const blob = new Blob([headers + csvContent], { type: 'text/csv;charset=utf-8;' });
    const link = document.createElement("a");
    const url = URL.createObjectURL(blob);
    link.setAttribute("href", url);
    link.setAttribute("download", "execution_report.csv");
    link.style.visibility = 'hidden';
    document.body.appendChild(link);
    link.click();
    document.body.removeChild(link);
  };

  return (
    <div className="flex flex-col gap-4">
      <div className="flex justify-between items-end">
        <div>
          <h2 className="text-xl font-bold text-slate-100 flex items-center gap-2">
            Execution Blotter
            <span className="text-xs font-normal text-slate-400 bg-white/5 px-2 py-0.5 rounded-full">
              {reports.length} Records
            </span>
          </h2>
        </div>
        <Button 
          size="sm" 
          variant="outline" 
          className="border border-white/10"
          onClick={handleDownload}
        >
          <Download size={14} /> Download Report
        </Button>
      </div>

      <Table 
        aria-label="Execution Report Blotter"
        className="max-h-[400px] bg-zinc-900 border border-white/10 p-0 shadow-xl"
      >
        <TableHeader className="bg-black/40 text-slate-400 font-semibold tracking-wider text-xs border-b border-white/10">
          <TableColumn>TIME</TableColumn>
          <TableColumn>ORDER ID</TableColumn>
          <TableColumn>CLIENT ID</TableColumn>
          <TableColumn>INSTRUMENT</TableColumn>
          <TableColumn>SIDE</TableColumn>
          <TableColumn className="text-right">QTY</TableColumn>
          <TableColumn className="text-right">PRICE</TableColumn>
          <TableColumn>STATUS</TableColumn>
        </TableHeader>
        <TableBody>
          {reports.length === 0 ? (
            <TableRow>
              <TableCell>...</TableCell>
              <TableCell>...</TableCell>
              <TableCell>...</TableCell>
              <TableCell>No execution reports available.</TableCell>
              <TableCell>...</TableCell>
              <TableCell>...</TableCell>
              <TableCell>...</TableCell>
              <TableCell>...</TableCell>
            </TableRow>
          ) : reports.map((report) => (
            <TableRow key={report.orderId} className="border-b border-white/5 last:border-0 hover:bg-white/5 transition-colors font-mono text-sm group">
              <TableCell className="text-slate-500 text-xs py-3">{report.transactionTime}</TableCell>
              <TableCell className="text-slate-300 py-3">{report.orderId}</TableCell>
              <TableCell className="text-slate-400 py-3">{report.clientOrderId}</TableCell>
              <TableCell className="font-sans font-medium text-slate-200 py-3">{report.instrument}</TableCell>
              <TableCell className="py-3">{getSideText(report.side)}</TableCell>
              <TableCell className="text-right text-slate-300 py-3">{report.quantity}</TableCell>
              <TableCell className="text-right text-slate-300 py-3">{report.price.toFixed(2)}</TableCell>
              <TableCell className="py-3">{getStatusChip(report.status, report.reason)}</TableCell>
            </TableRow>
          ))}
        </TableBody>
      </Table>
    </div>
  );
}
