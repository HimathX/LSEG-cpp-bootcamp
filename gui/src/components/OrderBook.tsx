import { Card, CardContent, CardHeader } from "@/components/ui/card";
import { Table, TableBody, TableCell, TableHead, TableHeader, TableRow } from "@/components/ui/table";
import { Separator } from "@/components/ui/separator";

export interface OrderLevel {
  price: number;
  qty: number;
}

interface OrderBookProps {
  instrument: string;
  bids: OrderLevel[]; // Best Buy
  asks: OrderLevel[]; // Best Sell
}

export function OrderBook({ instrument, bids, asks }: OrderBookProps) {
  // Ensure we always have exactly 3 levels to display
  const padLevels = (levels: OrderLevel[]) => {
    const padded = [...levels];
    while (padded.length < 3) {
      padded.push({ price: 0, qty: 0 });
    }
    return padded.slice(0, 3);
  };

  const displayBids = padLevels(bids);
  const displayAsks = padLevels(asks);

  return (
    <Card className="w-full min-w-[200px] border-border bg-card">
      <CardHeader className="py-2 px-3 flex justify-center items-center bg-muted/50">
        <h3 className="text-sm font-bold tracking-wide text-foreground uppercase">
          {instrument}
        </h3>
      </CardHeader>
      <Separator />
      <CardContent className="p-0">
        <Table className="max-w-full font-mono text-sm leading-none">
          <TableHeader>
            <TableRow className="hover:bg-transparent">
              <TableHead className="h-8 text-center text-green-500 font-semibold w-1/4 px-1">QTY</TableHead>
              <TableHead className="h-8 text-center text-green-500 font-semibold w-1/4 px-1 border-r border-border">BID</TableHead>
              <TableHead className="h-8 text-center text-red-500 font-semibold w-1/4 px-1">ASK</TableHead>
              <TableHead className="h-8 text-center text-red-500 font-semibold w-1/4 px-1">QTY</TableHead>
            </TableRow>
          </TableHeader>
          <TableBody>
            {displayBids.map((bid, i) => {
              const ask = displayAsks[i];
              return (
                <TableRow key={i} className="border-b last:border-0 hover:bg-muted/50 transition-colors h-7">
                  <TableCell className="py-1 px-2 text-center text-muted-foreground">{bid.qty > 0 ? bid.qty : "-"}</TableCell>
                  <TableCell className="py-1 px-2 text-center border-r border-border font-medium text-green-500">{bid.price > 0 ? bid.price.toFixed(2) : "-"}</TableCell>
                  <TableCell className="py-1 px-2 text-center font-medium text-red-500">{ask.price > 0 ? ask.price.toFixed(2) : "-"}</TableCell>
                  <TableCell className="py-1 px-2 text-center text-muted-foreground">{ask.qty > 0 ? ask.qty : "-"}</TableCell>
                </TableRow>
              );
            })}
          </TableBody>
        </Table>
      </CardContent>
    </Card>
  );
}
