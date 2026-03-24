import type { ExecutionReport } from "../components/ExecutionBlotter";

export function parseExecutionCSV(csvData: string): ExecutionReport[] {
  if (!csvData) return [];
  
  const lines = csvData.trim().split("\n");
  if (lines.length <= 1) return []; // Only headers or empty

  const reports: ExecutionReport[] = [];
  
  // Skip the header row (index 0) and parse the rest
  for (let i = 1; i < lines.length; i++) {
    const line = lines[i].trim();
    if (!line) continue;
    
    // OrderID,ClientOrderID,Instrument,Side,Price,Quantity,Status,Reason,Time
    const parts = line.split(",");
    
    // Safety check in case of malformed CSV
    if (parts.length < 9) continue;
    
    const sideText = parts[3].trim().toLowerCase();
    const side = sideText === "buy" || sideText === "1" ? 1 : 2;
    
    const statusVal = parseInt(parts[6].trim(), 10);
    
    reports.push({
      orderId: parts[0].trim(),
      clientOrderId: parts[1].trim(),
      instrument: parts[2].trim(),
      side: side,
      price: parseFloat(parts[4].trim()),
      quantity: parseInt(parts[5].trim(), 10),
      status: isNaN(statusVal) ? 0 : statusVal,
      reason: parts[7].trim() || undefined,
      transactionTime: parts[8].trim()
    });
  }
  
  return reports;
}
