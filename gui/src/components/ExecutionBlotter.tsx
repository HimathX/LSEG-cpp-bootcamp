import { Button } from "@heroui/react";
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

      <div className="w-full overflow-x-auto rounded-xl border border-white/10 bg-zinc-900 shadow-xl max-h-[400px]">
        <table className="w-full text-left border-collapse">
          <thead className="bg-black/40 text-slate-400 font-semibold tracking-wider text-xs border-b border-white/10 sticky top-0 z-10 backdrop-blur-md">
            <tr>
              <th className="py-3 px-4 font-semibold">TIME</th>
              <th className="py-3 px-4 font-semibold">ORDER ID</th>
              <th className="py-3 px-4 font-semibold">CLIENT ID</th>
              <th className="py-3 px-4 font-semibold">INSTRUMENT</th>
              <th className="py-3 px-4 font-semibold">SIDE</th>
              <th className="py-3 px-4 font-semibold text-right">QTY</th>
              <th className="py-3 px-4 font-semibold text-right">PRICE</th>
              <th className="py-3 px-4 font-semibold">STATUS</th>
            </tr>
          </thead>
          <tbody className="divide-y divide-white/5">
            {reports.length === 0 ? (
              <tr className="border-b border-white/5/0">
                <td colSpan={8} className="py-8 text-center text-slate-500 text-sm">
                  No execution reports available.
                </td>
              </tr>
            ) : reports.map((report) => (
              <tr key={report.orderId} className="hover:bg-white/5 transition-colors font-mono text-sm group">
                <td className="text-slate-500 text-xs py-3 px-4 whitespace-nowrap">{report.transactionTime}</td>
                <td className="text-slate-300 py-3 px-4">{report.orderId}</td>
                <td className="text-slate-400 py-3 px-4">{report.clientOrderId}</td>
                <td className="font-sans font-medium text-slate-200 py-3 px-4">{report.instrument}</td>
                <td className="py-3 px-4">{getSideText(report.side)}</td>
                <td className="text-right text-slate-300 py-3 px-4">{report.quantity}</td>
                <td className="text-right text-slate-300 py-3 px-4">{report.price.toFixed(2)}</td>
                <td className="py-3 px-4">{getStatusChip(report.status, report.reason)}</td>
              </tr>
            ))}
          </tbody>
        </table>
      </div>
    </div>
  );
}
